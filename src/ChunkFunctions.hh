/**
 * File: ChunkFunctions.hh
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

#ifndef CHUNK_FUNCTIONS_HH
#define CHUNK_FUNCTIONS_HH

#include <fstream>

class ChunkFunctions {
protected:
        std::streamsize fileSize(std::ifstream& file) const;
        std::streamsize chunk(std::ifstream& input, std::ofstream& output,
            std::streamsize remaining) const;
public:
        ChunkFunctions() = default;
        virtual ~ChunkFunctions() = default;
        ChunkFunctions(const ChunkFunctions&) = delete;
};

#endif /// CHUNK_FUNCTIONS_HH
