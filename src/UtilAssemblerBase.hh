/**
 * File: UtilAssemblerBase.hh
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

#include "src/list.hh"
#include "src/types.hh"

#ifndef UTIL_ASSEMBLER_BASE_HH
#define UTIL_ASSEMBLER_BASE_HH

using WriteStatus = std::pair<std::streamsize, Error>;
using FilesL = ty::list<std::string>;

class UtilAssemblerBase {
private:
    WriteStatus writeAssembleI(FilesL files, std::ofstream& out,
            const std::streamsize acc) const;
protected:
    WriteStatus writeAssemble(FilesL files, std::ofstream& out) const;
public:
    UtilAssemblerBase() { }
    ~UtilAssemblerBase() { }
    UtilAssemblerBase(const UtilAssemblerBase&) = delete;
};

#endif /// UTIL_ASSEMBLER_BASE_HH
