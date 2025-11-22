/**
 * File: utils.cc
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

#include "src/utils.hh"

namespace util {

std::string sanitize(const std::string& str)
{
        auto bIt = str.begin();
        auto eIt = str.end() - 1;
        while (*bIt == ' ')
                bIt++;
        while (eIt > bIt && *eIt == ' ')
                eIt--;
        if (bIt < eIt && *eIt == '/')
                eIt--;
        eIt++;
        return std::string(bIt, eIt);
}

ArgList argsToList(int argc, char* argv[])
{
        ArgList acc = nullptr;
        for (int i = 1; i < argc; i++)
                acc = push(std::string(argv[i]), acc);
        return reverseN(apply(acc, util::sanitize));
}

} /// util
