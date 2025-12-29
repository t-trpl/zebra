/**
 * File: Row.hh
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

#ifndef ROW_HH
#define ROW_HH

#include <ios>
#include <unordered_map>
#include <string>

enum Dir { LEFT, RIGHT };

class Row {
private:
        inline static const std::unordered_map<Dir, std::string> arrows_ = {
            { LEFT, "\033[32m<-\033[0m" },
            { RIGHT, "\033[32m->\033[0m" },
        };
public:
        Row() = default;
        ~Row() = default;
        Row(const Row&) = delete;
        static void print(const Dir& d, const std::string& path,
            const std::streamsize& bytes);
};

#endif /// ROW_HH
