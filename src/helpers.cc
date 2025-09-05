/**
 * File: helpers.cc
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

#include "src/helpers.hh"

bool containsMap(const ArgMap& map, const ArgOr& arg)
{
    return map.find(arg.first) != map.end() ||
            map.find(arg.second) != map.end();
}

Maybe<ArgMap::const_iterator> argToValue(const ArgMap& map, const ArgT& arg)
{
    const auto leftPtr = map.find(std::get<0>(arg));
    const auto rightPtr = map.find(std::get<1>(arg));
    const auto& name = std::get<2>(arg);
    const auto end = map.end();
    if (leftPtr != end && rightPtr != end)
        return make_bad<ArgMap::const_iterator>("Duplicate option: " + name);
    if (leftPtr != end)
        return leftPtr;
    if (rightPtr != end)
        return rightPtr;
    return end;
}
