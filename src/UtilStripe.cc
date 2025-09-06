/** 
 * File: UtilStripe.cc
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

#include "src/UtilStripe.hh"
#include "src/Maybe.hh"
#include "src/types.hh"
#include "src/utils.hh"
#include "src/helpers.hh"
#include <filesystem>
#include <iostream>
#include <cctype>
#include <unordered_map>
#include <algorithm>

namespace fs = std::filesystem;

std::unordered_set<std::string> UtilStripe::getValidOptionsFlags() const
{
    return {
        "-i", "--input",
        "-o", "--output",
        "-s", "--size",
        "-n", "--name",
        "-e", "--extension",
        "-np", "--no-padding",
        "-q", "--quiet",
        "-ne", "--no-extension",
    };
}

Error UtilStripe::setArgs(const ArgMap& map)
{
    const auto baseError = UtilBaseSingle::setArgs(map);
    if (baseError)
        return *baseError;
    const auto maybeSize = argToValue(map, {"--size", "-s", "size"});
    if (!maybeSize)
        return maybeSize.error();
    if (const auto ptr = *maybeSize; ptr != map.end()) {
        if (ptr->second.empty())
            return "No size";
        if (ptr->second.size() > 1)
            return "Too many sizes";
        const auto &size = ptr->second[0];
        const auto bytes = stringToBytes(size);
        if (bytes)
            stripeSize_ = *bytes;
        else
            return bytes.error();
    }
    const auto nameError = setMember(map, {"--name", "-n", "name"}, name_);
    if (nameError)
        return *nameError;
    const auto extensionError = setMember(map,
            {"--extension", "-e", "extension"}, ext_);
    if (extensionError)
        return *extensionError;
    return None;
}

Maybe<size_t> UtilStripe::stringToBytes(const std::string& size) const
{
    std::string num = "";
    auto it = size.begin();
    while (it != size.end() && (std::isdigit(*it) || *it == '.'))
        num += *it++;
    if (num.empty())
        return make_bad<size_t>("No size...");
    const auto decimalCount = std::count_if(num.begin(), num.end(),
            [](const auto& c){ return c == '.'; });
    if (it != size.end() && (!std::isalpha(*it) || decimalCount > 1))
        return make_bad<size_t>("Bad byte size");
    const double s = std::stod(num);
    std::unordered_map<std::string, size_t> pref = {   
        {"b", 1},
        {"kb", 1'000},
        {"mb", 1'000'000},
        {"gb", 1'000'000'000},
    };
    std::string suffix;
    std::transform(it, size.end(), std::back_inserter(suffix),
            [](const auto& c) { return std::tolower(c); });
    const auto suffixIt = pref.find(suffix);
    const auto found = suffixIt != pref.end();
    if (!suffix.empty() && !found)
        return make_bad<size_t>("Bad suffix: " + suffix); 
    const size_t bytes =
            static_cast<size_t>(!found ? s : (s * suffixIt->second));
    if (bytes < 4'000) /// arbitrary, but don't want an accidental 1 byte size 
        return make_bad<size_t>("Stripe size < 4kb");
    return bytes;
}

size_t UtilStripe::stripesStrLen(const size_t& rem) const
{
    return rem > 0 ? 1 + stripesStrLen(rem / 10) : 0;
}

size_t UtilStripe::maxNameLength(const std::streamsize& size) const
{
    const size_t pieces = size / stripeSize_ + (size % stripeSize_ > 0); 
    return stripesStrLen(pieces);
}

std::streamsize UtilStripe::getFileSize(const std::string& location) const
{
    std::ifstream file(location, std::ios::binary | std::ios::ate);
    if (!file)
        return -1;
    return file.tellg(); 
}

std::string UtilStripe::getFileName(const int& number, 
        const size_t& len) const
{
    const std::string strNumber = std::to_string(number);
    const int diff = static_cast<int>(len) - static_cast<int>(strNumber.size());
    const int zeros = padding_ ? std::max(0, diff) : 0; 
    const std::string paddingZeros(zeros, '0');
    return name_ + (!name_.empty() ? "_" : "") + paddingZeros + strNumber;
}

std::streamsize UtilStripe::readChunk(std::ifstream& file,
        std::vector<char>& buffer) const
{
    file.read(buffer.data(), buffer.size());
    return file.gcount();
}

Error UtilStripe::setFlags(const ArgMap& map)
{
    const auto maybePad = validFlag(map, {"-np", "--no-padding"});
    if (!maybePad)
        return maybePad.error();
    else if (*maybePad)
        padding_ = false;

    const auto maybeQuiet = validFlag(map, {"-q", "--quiet"});
    if (!maybeQuiet)
        return maybeQuiet.error();
    else if (*maybeQuiet)
        silence_ = true;

    const auto maybeNoExt = validFlag(map, {"-ne", "--no-extension"});
    if (!maybeNoExt)
        return maybeNoExt.error();
    else if (*maybeNoExt)
        useExt_ = false;
    return None;
}

std::string UtilStripe::getStripePath(const size_t& num, const size_t& max) const
{
    const auto name = getFileName(num, max);
    const std::string fullName = useExt_ ? (name + "." + ext_) : name;
    const std::string path = fs::path(out_) / fullName;
    return path;
}

Error UtilStripe::run() const
{
    if (!silence_)
        std::cout << util::banner << "\nStriping\n";
    if (fs::is_directory(in_))
        return "Cannot run on a directory";
    if (!fs::exists(out_) || !fs::is_directory(out_))
        return "Bad output directory";
    std::ifstream file(in_, std::ios::binary);
    if (!file)
        return "Invalid File";
    const auto fileSize = getFileSize(in_);
    if (fileSize == -1)
        return "Empty file?";
    const auto maxName = maxNameLength(fileSize);
    std::vector<char> buffer(stripeSize_);
    int chunkNumber = 0;
    for (auto bytes = readChunk(file, buffer);
         bytes;
         bytes = readChunk(file, buffer)) {
        const auto path = getStripePath(chunkNumber++, maxName);
        std::ofstream outFile(path);
        if (!outFile)
            return "Failed to write: " + path;
        if (!silence_)
            std::cout << "\033[32m->\033[0m" << path << " " << bytes <<
                    " bytes\n";
        outFile.write(buffer.data(), bytes);
    }
    return None;
}
