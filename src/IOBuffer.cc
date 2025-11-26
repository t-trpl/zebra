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

#include "src/IOBuffer.hh"

IOBuffer::IOBuffer()
    : buffer_(size_, 0)
{ }

std::streamsize IOBuffer::fileSize(std::ifstream& file) const
{
        file.seekg(0, std::ios::end);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        return size;
}

std::streamsize IOBuffer::chunk(std::ifstream& input, std::ofstream& output,
    std::streamsize remaining)
{
        std::streamsize acc = 0;
        while (remaining) {
                const auto use = std::min(size_, remaining);
                input.read(buffer_.data(), use);
                const auto& read = input.gcount();
                if (!read)
                        break;
                acc += read;
                output.write(buffer_.data(), read);
                remaining -= use;
        }
        return acc;
}
