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

Error run(const ArgN args)
{
     Parser p;
     const auto parseError = p.runParse(args);
     if (parseError)
          return *parseError;
     if (p.checkHelp())
          return util::help;
     const auto util = p.createUtil();
     if (!util)
          return util.error();
     const auto error = (*util)->run();
     if (error)
          return *error;
     return None;
}

ArgN argsToArgN(int argc, char* argv[])
{
     ArgN n = nullptr;
     for (int i = 1; i < argc; i++)
          n = cons(std::string(argv[i]), n);
     return reverse(n);
}

int main(int argc, char* argv[])
{
     const ArgN args = argsToArgN(argc, argv);
     const auto error = run(args);
     if (error) {
          std::cout << *error << "\n";
          return 1;
     }
     return 0;
}
