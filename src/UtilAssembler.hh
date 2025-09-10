/**
 * File: UtilAssembler.hh
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

#include "src/UtilBaseSingle.hh"
#include "src/UtilAssemblerBase.hh"
#include "src/Maybe.hh"
#include "src/list.hh"
#include <filesystem>

#ifndef UTIL_ASSEMBLER_HH
#define UTIL_ASSEMBLER_HH

namespace fs = std::filesystem;

class UtilAssembler final : public UtilBaseSingle
                          , public UtilAssemblerBase {
private:
     std::string ext_ = "stripe";
     bool useExt_ = true;
     std::string name_ = "";
     std::string stemToName(const std::string& stem) const;
     Maybe<FilesL> stripeNames() const;
     std::unordered_set<std::string> validArgs() const override;
     bool matchExt(const fs::directory_entry& ext) const;
     bool matchName(const fs::directory_entry& stem) const;
public:
     UtilAssembler() { }
     ~UtilAssembler() { }
     UtilAssembler(const UtilAssembler&) = delete;
     Error run() const override;
     Error setArgs(const ArgMapN& map) override;
     Error setFlags(const ArgMapN& map) override;
};

#endif /// UTIL_ASSEMBLER_HH
