/**
 * File: main.cc
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

#include "src/Parser.hh"
#include "src/utils.hh"
#include <iostream>

Error run(const ArgList args)
{
        Parser p;
        if (const auto e = p.runParse(args))
                return *e;
        if (p.checkHelp())
                return util::HELP;
        const auto util = p.createUtil();
        if (!util)
                return util.error();
        if (const auto e = (*util)->run())
                return *e;
        return NONE;
}

ArgList argsToList(int argc, char* argv[])
{
        ArgList acc = nullptr;
        for (int i = 1; i < argc; i++)
                acc = push(std::string(argv[i]), acc);
        return reverseN(acc);
}

int main(int argc, char* argv[])
{
        const auto args = argsToList(argc, argv);
        if (const auto e = run(args)) {
                std::cout << *e << "\n";
                return 1;
        }
        return 0;
}
