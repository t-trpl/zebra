/**
 * File: UtilAssemblerMulti.hh
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

#include "src/UtilBase.hh"
#include "src/UtilAssemblerFunctions.hh"
#include "src/List.hh"
#include <string>

#ifndef UTIL_ASSEMBLER_MULTI_HH
#define UTIL_ASSEMBLER_MULTI_HH

class UtilAssemblerMulti final : public UtilBase
                               , public UtilAssemblerFunctions {
private:
        FilesL files_;
        std::string out_;
        std::unordered_set<std::string> validArgs() const override;
public:
        UtilAssemblerMulti() { }
        ~UtilAssemblerMulti() { }
        UtilAssemblerMulti(const UtilAssemblerMulti&) = delete;
        Error run() const override;
        Error setFlags(const ArgMap& map) override;
        Error setArgs(const ArgMap& map) override;
};

#endif /// UTIL_ASSEMBLER_MULTI_HH
