/**
 * File: AssemblerIO.hh
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

#ifndef ASSEMBLER_IO_HH
#define ASSEMBLER_IO_HH

#include "src/Maybe.hh"
#include "src/IOBuffer.hh"
#include "src/types.hh"

class AssemblerIO : protected IOBuffer {
protected:
        Maybe<std::streamsize> writeStripe(FilesL files, std::ofstream& out,
            const bool silence);
public:
        AssemblerIO() = default;
        virtual ~AssemblerIO() = default;
        AssemblerIO(const AssemblerIO&) = delete;
};

#endif /// ASSEMBLER_IO_HH
