/**
 * File: types.hh
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

#ifndef TYPES_HH
#define TYPES_HH

#include "src/List.hh"
#include <unordered_map>
#include <string>
#include <optional>
#include <vector>

using ArgList = List<std::string>;

using ArgMap = std::unordered_map<std::string, ArgList>;

using Error = std::optional<std::string>;

using ArgT = std::tuple<std::string, std::string, std::string>;

using ArgOr = std::pair<std::string, std::string>;

using MapIt = ArgMap::const_iterator;

using FilesL = List<std::string>;

using Conflict = std::vector<std::tuple<ArgOr, ArgOr, std::string>>;

constexpr auto NONE = std::nullopt;

#define EXT_A                                   \
        { "--extension", "-e", "extension" }

#define NAME_A                                  \
        { "--name", "-n", "name" }

#define OUT_A                                   \
        { "--output", "-o", "output" }

#define IN_A                                    \
        { "--input", "-i", "input" }

#define SIZE_A                                  \
        { "--size", "-s", "size" }

#define PARTS_A                                 \
        { "--parts", "-p", "parts" }

#define QUIET_F                                 \
        { "--quiet", "-q" }

#define NO_EXT_F                                \
        { "--no-extension", "-ne" }

#define NO_NAME_F                               \
        { "--no-name", "-nn" }

#define NO_PAD_F                                \
        { "--no-padding", "-np" }

#endif /// TYPES_HH
