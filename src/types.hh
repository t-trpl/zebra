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

#include "src/List.hh"
#include <unordered_map>
#include <string>
#include <optional>

#ifndef TYPES_HH
#define TYPES_HH

using ArgList = ty::List<std::string>;

using ArgMap = std::unordered_map<std::string, ArgList>;

using Error = std::optional<std::string>;

using ArgT = std::tuple<std::string, std::string, std::string>;

using ArgOr = std::pair<std::string, std::string>;

constexpr auto None = std::nullopt;

#endif /// TYPES_HH
