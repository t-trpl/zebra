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
#include "src/utils.hh"
#include <iostream>
#include <filesystem>
#include <fstream>

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
        const size_t pieces = size / stripeSize + (size % stripeSize > 0);
        return numberLength(pieces - 1);
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
        const auto fsize = fileSize(file);
        if (fsize == -1)
                return "Empty file?";
        const auto stripeSize = getStripeSize(fsize);
        if (stripeSize < 4'000)
                return "Stripe size too small";
        const auto length = stripeLength(fsize, stripeSize);
        int chunkNumber = 0;
        std::streamsize totalBytes = 0;
        while (totalBytes < fsize) {
                const auto path = stripePath(chunkNumber++, length, out_);
                std::ofstream outFile(path);
                if (!outFile)
                        return "Failed to write: " + path;
                const auto bytes = chunk(file, outFile, stripeSize);
                totalBytes += bytes;
                if (!silence_)
                        std::cout << "\033[32m->\033[0m" << path << " " << bytes
                                  << " bytes\n";
        }
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
        return NONE;
}
