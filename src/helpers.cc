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

Maybe<bool> validFlag(const ArgMap& map, const ArgOr& arg)
{
        const auto& [full, abrv] = arg;
        const auto it1 = map.find(full);
        const auto it2 = map.find(abrv);
        const auto end = map.end();
        if (it1 != end && it2 != end)
                return makeBad<bool>("Duplicate flags");
        if (it1 == end && it2 == end)
                return false;
        const auto c = it1 != end ? it1 : it2;
        if (c->second)
                return makeBad<bool>("Flag with args " + c->first);
        return true;
}

Maybe<MapIt> argToIter(const ArgMap& map, const ArgT& arg)
{
        const auto& [full, abrv, name] = arg;
        const auto it1 = map.find(full);
        const auto it2 = map.find(std::get<1>(arg));
        const auto end = map.end();
        if (it1 != end && it2 != end)
                return makeBad<MapIt>("Duplicate option: " + name);
        if (it1 != end)
                return it1;
        if (it2 != end)
                return it2;
        return end;
}
