/**
 * File: UtilStripeBase.cc
 * Copyright (C) 2025 Tyler Triplett
 * License: GNU GPL 3.0 or later <https://www.gnu.org/licenses/gpl-3.0.html>
 *
 * This is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "src/UtilStripeBase.hh"
#include "IOBuffer.hh"
#include "src/utils.hh"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <thread>
#include <algorithm>

namespace fs = std::filesystem;

size_t UtilStripeBase::numberLength(const size_t& rem) const
{
        return rem > 0 ? 1 + numberLength(rem / 10) : 0;
}

Conflict UtilStripeBase::conflicting() const
{
        return {
            {
                { "--no-extension", "-ne" },
                { "--extension", "-e" },
                "No-Extension and Extension not possible"
            },
            {
                { "--parts", "-p" },
                { "--size", "-s" },
                "Parts and Size not possible"
            },
        };
}

size_t UtilStripeBase::stripeLength(const std::streamsize& size,
    const size_t& stripeSize) const
{
        return size / stripeSize + (size % stripeSize > 0);
}

std::string UtilStripeBase::fileName(const int& number, const size_t& len) const
{
        const std::string strn = std::to_string(number);
        const int diff = static_cast<int>(len) - static_cast<int>(strn.size());
        const int zeros = padding_ ? std::max(0, diff) : 0;
        const std::string paddingZeros(zeros, '0');
        return name_ + (!name_.empty() ? "_" : "") + paddingZeros + strn;
}

std::string UtilStripeBase::stripePath(const size_t& num, const size_t& max,
    const std::string& out) const
{
        const auto name = fileName(num, max);
        const std::string fullName = useExt_ ? (name + "." + ext_) : name;
        const std::string path = fs::path(out) / fullName;
        return path;
}

std::vector<int> UtilStripeBase::fileIndex(const size_t& stripes)
{
        const int base = stripes / threadc_;
        const int rem = stripes % threadc_;
        std::vector<int> quant(threadc_, base);
        for (int i = 0; i < rem; i++)
                quant[i]++;
        quant.erase(
            std::remove_if(quant.begin(), quant.end(), [](const auto& x) {
                    return x == 0;
            }),
            quant.end());
        std::vector<int> index = { 0 };
        for (const auto& x : quant)
                index.push_back(index.back() + x);
        return index;
}

Maybe<IFiles> UtilStripeBase::files(const std::vector<int>& start,
    const size_t& s)
{
        std::vector<std::ifstream> descriptors;
        for (const auto& x : start) {
                descriptors.emplace_back(in_, std::ios::binary);
                auto& d = descriptors.back();
                if (!d)
                        return makeBad<IFiles>("Invalid File");
                d.seekg(x * s);
        }
        return descriptors;
}

void UtilStripeBase::worker(std::ifstream& file, const WD& data)
{
        auto [start, end, len, size] = data;
        IOBuffer buffer;
        while (!failure_ && start < end) {
                const auto path = stripePath(start++, len, out_);
                std::ofstream outFile(path);
                if (!outFile) {
                        failure_ = true;
                        std::lock_guard<std::mutex> lock(fmtx_);
                        fmsg_ = "Error" + path;
                }
                const auto bytes = buffer.chunk(file, outFile, size);
                if (!bytes)
                        break;
                if (!silence_) {
                        std::lock_guard<std::mutex> lock(mtx_);
                        std::cout << "\033[32m->\033[0m" << path << " " << bytes
                                  << " bytes\n";
                }
        }
}

Error UtilStripeBase::run()
{
        if (!silence_)
                std::cout << util::BANNER << "\nStriping\n";
        if (fs::is_directory(in_))
                return "Cannot run on a directory";
        if (!fs::exists(out_) || !fs::is_directory(out_))
                return "Bad output directory";
        std::ifstream file(in_, std::ios::binary);
        if (!file)
                return "Invalid File";
        const auto fsize = util::fileSize(file);
        if (fsize == -1)
                return "Empty file?";
        const auto stripeSize = getStripeSize(fsize);
        if (stripeSize < 4'000)
                return "Stripe size too small";
        const auto stripes = stripeLength(fsize, stripeSize);
        const auto length = numberLength(stripes - 1);
        const auto starts = fileIndex(stripes);
        auto descriptors = files(starts, stripeSize);
        if (!descriptors)
                return descriptors.error();
        std::vector<std::thread> threads;
        const int t = std::min(threadc_, static_cast<int>(starts.size()) - 1);
        for (int i = 0; i < t; i++) {
                auto& f = (*descriptors)[i];
                const auto& start = starts[i];
                const auto& end = starts[i + 1];
                threads.emplace_back(
                    &UtilStripeBase::worker,
                    this,
                    std::ref(f),
                    WD{ start, end, length, stripeSize });
        }
        for (auto& t : threads)
                t.join();
        if (failure_)
                return fmsg_;
        return NONE;
}

Error UtilStripeBase::setFlags(const ArgMap& map)
{
        if (const auto m = validFlag(map, NO_PAD_F); m && *m)
                padding_ = false;
        else if (!m)
                return m.error();
        if (const auto m = validFlag(map, QUIET_F); m && *m)
                silence_ = true;
        else if (!m)
                return m.error();
        if (const auto m = validFlag(map, NO_EXT_F); m && *m)
                useExt_ = false;
        else if (!m)
                return m.error();
        return NONE;
}

Error UtilStripeBase::setArgs(const ArgMap& map)
{
        if (const auto e = UtilBaseSingle::setArgs(map))
                return *e;
        if (const auto e = setMember(map, NAME_A, name_))
                return *e;
        if (const auto e = setMember(map, EXT_A, ext_))
                return *e;
        const auto threads = argToIter(map, THREADS_A);
        if (!threads)
                return threads.error();
        if (const auto it = *threads; it != map.end()) {
                const auto ptr = it->second;
                switch (ty::count(ptr)) {
                case 0:
                        return "No threads";
                case 1: {
                        const auto t = std::stoi(ptr->val_);
                        if (t == 0)
                                return "Can't have zero threads";
                        threadc_ = t;
                        break;
                }
                default:
                        return "Too many threads";
                }
        }
        return NONE;
}
