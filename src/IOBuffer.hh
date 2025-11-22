/**
 * File: IOBuffer.hh
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

#ifndef IO_BUFFER_HH
#define IO_BUFFER_HH

#include <fstream>
#include <vector>

class IOBuffer {
private:
        const std::streamsize size_ = 1'024 * 64;
        std::vector<char> buffer_;
protected:
        std::streamsize fileSize(std::ifstream& file) const;
        std::streamsize chunk(std::ifstream& input, std::ofstream& output,
            std::streamsize remaining);
public:
        IOBuffer();
        virtual ~IOBuffer() = default;
        IOBuffer(const IOBuffer&) = delete;
};

#endif /// IO_BUFFER_HH
