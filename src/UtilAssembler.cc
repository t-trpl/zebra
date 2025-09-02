/** 
 * File: UtilAssembler.cc
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

#include "src/UtilAssembler.hh"
#include "src/Maybe.hh"
#include "src/utils.hh"
#include "src/helpers.hh"
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

Maybe<Files> UtilAssembler::loadFileNames() const
{
    Files files;
    if (!fs::exists(in_) || !fs::is_directory(in_))
        return make_bad<Files>("Not a directory: " + in_);
    const std::string completeExt = std::string(".") + ext_;
    for (const auto& file : fs::directory_iterator(in_)) {
        const auto& ext = file.path().extension().string();
        const auto& stem = file.path().stem().string();
        if (((!useExt_ && ext.empty()) || ext == completeExt) &&
                (name_.empty() || name_ == stemToName(stem))) {
            const auto& name = file.path().filename().string();
            files.push_back(name);
        }
    }
    std::sort(files.begin(), files.end());
    return files;
}

std::string UtilAssembler::stemToName(const std::string& stem) const
{
    
    auto ptr = stem.end() - 1;
    while (ptr > stem.begin() && std::isdigit(*ptr))
        ptr--;
    return std::string(stem.begin(), ptr);
}

Error UtilAssembler::setArgs(const ArgMap& map)
{
    const auto baseError = UtilBaseSingle::setArgs(map);
    if (baseError)
        return *baseError;
    const auto errorExtension = setMember(map, {"--extension", "-e", "extension"}, ext_);
    if (errorExtension)
        return *errorExtension;
    const auto errorName = setMember(map, {"--name", "-n", "name"}, name_);
    if (errorName)
        return *errorName;
    return None;
}

Error UtilAssembler::run() const 
{
    if (!silence_) {
      std::cout << util::banner << "\n";
      std::cout << "Assembling\n";
    }
    const auto load = loadFileNames();
    if (!load)
        return load.error();
    const auto& files = *load;
    if (files.empty())
        return "No Pieces";
    std::ofstream outFile(out_);
    if (!outFile)
        return "Failed to open: " + out_;
    if (!silence_)
        std::cout << "\033[32m->\033[0m" << out_ << "\n";
    for (const auto& x : files) {
        const std::string path = fs::path(in_) / x;
        std::ifstream file(path, std::ios::binary);
        if (!file)
            return "Failed to open: " + path + "\nDiscard output";
        outFile << file.rdbuf();
    }
    return None;
}

void UtilAssembler::setFlags(const Flags& flags)
{
    if (containsFlag(flags, {"-q", "--quiet"}))
        silence_ = true;
    if (containsFlag(flags, {"-ne", "--no-extension"}))
        useExt_ = false;
}

std::unordered_set<std::string> UtilAssembler::getValidOptionsFlags() const
{
    return {
        "-i", "--input",
        "-o", "--output",
        "-e", "--extension",
        "-n", "--name",
        "-q", "--quiet",
        "-ne", "--no-extension"
    };
}
