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
        if (str.empty())
                return str;
        auto bIt = str.begin();
        auto eIt = str.end() - 1;
        while (bIt < str.end() && *bIt == ' ')
                bIt++;
        while (eIt > bIt && *eIt == ' ')
                eIt--;
        eIt++;
        return std::string(bIt, eIt);
}

ArgList argsToList(int argc, char* argv[])
{
        ArgList acc = nullptr;
        for (int i = 1; i < argc; i++)
                acc = push(std::string(argv[i]), acc);
        return reverse(map(acc, util::sanitize));
}

bool contains(const ArgMap& argMap, const ArgOr& options)
{
        if (argMap.find(options.first) != argMap.end())
                return true;
        if (argMap.find(options.second) != argMap.end())
                return true;
        return false;
}

ArgList mapOr(const ArgMap& argMap, const ArgOr& options)
{
        if (const auto it = argMap.find(options.first); it != argMap.end())
                return it->second;
        if (const auto it = argMap.find(options.second); it != argMap.end())
                return it->second;
        return nullptr;
}

bool isDigit(const char c)
{
        const auto uc = static_cast<unsigned char>(c);
        return std::isdigit(uc);
}

bool isAlpha(const char c)
{
        const auto uc = static_cast<unsigned char>(c);
        return std::isalpha(uc);
}

} /// util
