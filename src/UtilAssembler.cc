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
#include "src/types.hh"
#include "src/utils.hh"
#include "src/helpers.hh"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

Maybe<ty::list<std::string>> UtilAssembler::loadFileNames() const
{
    ty::list<std::string> files;
    if (!fs::exists(in_) || !fs::is_directory(in_))
        return make_bad<ty::list<std::string>>("Not a directory: " + in_);
    const std::string completeExt = std::string(".") + ext_;
    for (const auto& file : fs::directory_iterator(in_)) {
        const auto& ext = file.path().extension().string();
        const auto& stem = file.path().stem().string();
        if (((!useExt_ && ext.empty()) || ext == completeExt) &&
                (name_.empty() || name_ == stemToName(stem))) {
            const auto& name = file.path().filename().string();
            files = cons(name, files);
        }
    }
    ty::sort(files);
    return files;
}

std::string UtilAssembler::stemToName(const std::string& stem) const
{
    auto ptr = stem.end() - 1;
    while (ptr > stem.begin() && std::isdigit(*ptr))
        ptr--;
    return std::string(stem.begin(), ptr);
}

Error UtilAssembler::setArgs(const ArgMapN& map)
{
    const auto baseError = UtilBaseSingle::setArgs(map);
    if (baseError)
        return *baseError;
    const auto errorExtension = setMember(map,
            {"--extension", "-e", "extension"}, ext_);
    if (errorExtension)
        return *errorExtension;
    const auto errorName = setMember(map, {"--name", "-n", "name"}, name_);
    if (errorName)
        return *errorName;
    return None;
}

Error UtilAssembler::writeAssemble(ty::list<std::string> files,
        std::ofstream& out) const
{
    if (!files)
        return None;
    const std::string path = fs::path(in_) / car(files);
    std::ifstream file(path, std::ios::binary);
    if (!file)
        return "Failed to open: " + path + "\nDiscard output";
    out << file.rdbuf();
    return writeAssemble(files->next, out);
}

Error UtilAssembler::run() const 
{
    if (!silence_)
      std::cout << util::banner << "\nAssembling\n";
    const auto maybeFiles = loadFileNames();
    if (!maybeFiles)
        return maybeFiles.error();
    if (!*maybeFiles)
        return "No Pieces";
    std::ofstream outFile(out_);
    if (!outFile)
        return "Failed to open: " + out_;
    if (!silence_)
        std::cout << "\033[32m->\033[0m" << out_ << "\n";
    return writeAssemble(*maybeFiles, outFile);
}

Error UtilAssembler::setFlags(const ArgMapN& map)
{
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

std::unordered_set<std::string> UtilAssembler::getValidOptionsFlags() const
{
    return {
        "-i", "--input",
        "-o", "--output",
        "-e", "--extension",
        "-n", "--name",
        "-q", "--quiet",
        "-ne", "--no-extension",
    };
}
