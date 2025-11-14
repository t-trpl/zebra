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
#include <fstream>
#include <iostream>

Maybe<FilesL> UtilAssembler::stripeNames() const
{
        FilesL files;
        if (!fs::exists(in_) || !fs::is_directory(in_))
                return makeBad<FilesL>("Not a directory: " + in_);
        for (const auto& file : fs::directory_iterator(in_)) {
                if (matchExt(file) && matchName(file)) {
                        const auto p = fs::path(in_) / file.path().filename();
                        files = push(p.string(), files);
                }
        }
        return sortN(files);
}

bool UtilAssembler::matchExt(const fs::directory_entry& file) const
{
        const auto& ext = file.path().extension().string();
        const auto expected = "." + ext_;
        return useExt_ ? ext == expected : ext.empty();
}

bool UtilAssembler::matchName(const fs::directory_entry& file) const
{
        const auto& stem = file.path().stem().string();
        const auto name = stemToName(stem);
        return empty_ ? name.empty() : name_.empty() || name_ == name;
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
        if (const auto e = UtilBaseSingle::setArgs(map))
                return *e;
        if (const auto e = setMember(map, EXT_A, ext_))
                return *e;
        if (const auto e = setMember(map, NAME_A, name_))
                return *e;
        return NONE;
}

Error UtilAssembler::run() const
{
        if (!silence_)
                std::cout << util::BANNER << "\nAssembling\n";
        const auto stripes = stripeNames();
        if (!stripes)
                return stripes.error();
        if (!*stripes)
                return "No Pieces";
        std::ofstream outFile(out_);
        if (!outFile)
                return "Failed to open: " + out_;
        const auto bytes = writeStripe(*stripes, outFile);
        if (!bytes)
                return bytes.error();
        if (!silence_)
                std::cout << "Wrote " << out_ << " " << *bytes << " bytes\n";
        return NONE;
}

Error UtilAssembler::setFlags(const ArgMap& map)
{
        if (const auto m = validFlag(map, QUIET_F); m && *m)
                silence_ = true;
        else if (!m)
                return m.error();
        if (const auto m = validFlag(map, NO_EXT_F); m && *m)
                useExt_ = false;
        else if (!m)
                return m.error();
        if (const auto m = validFlag(map, NO_NAME_F); m && *m)
                empty_ = true;
        else if (!m)
                return m.error();
        return NONE;
}

std::unordered_set<std::string> UtilAssembler::validArgs() const
{
        return {
            "--input"       , "-i" ,
            "--output"      , "-o" ,
            "--extension"   , "-e" ,
            "--name"        , "-n" ,
            "--quiet"       , "-q" ,
            "--no-extension", "-ne",
            "--no-name"     , "-nn",
        };
}
