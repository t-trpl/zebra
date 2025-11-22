/**
 * File: StripeAssemblerMulti.cc
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

#include "src/UtilAssemblerMulti.hh"
#include "src/utils.hh"
#include <iostream>
#include <fstream>

Error UtilAssemblerMulti::setArgs(const ArgMap& map)
{
        const auto input = argToIter(map, IN_A);
        if (!input)
                return input.error();
        if (const auto it = *input; it != map.end()) {
                FilesL acc = nullptr;
                for (auto p = it->second; p; p = p->next)
                        acc = push(toPath(p->val), acc);
                files_ = reverseN(acc);
        } else {
                return "Missing Input";
        }
        if (const auto e = setPath(map, OUT_A, out_))
                return *e;
        return NONE;
}

Error UtilAssemblerMulti::run() const
{
        if (!silence_)
                std::cout << util::BANNER << "\nAssembling\n";
        std::ofstream output(out_);
        if (!output)
                return "Failed to open: " + out_;
        const auto bytes = writeStripe(files_, output);
        if (!bytes)
                return bytes.error();
        if (!silence_)
                std::cout << "Wrote " << out_ << " " << *bytes << " bytes\n";
        return NONE;
}

Error UtilAssemblerMulti::setFlags(const ArgMap& map)
{
        if (const auto m = validFlag(map, QUIET_F); m && *m)
                silence_ = true;
        else if (!m)
                return m.error();
        return NONE;
}

std::unordered_set<std::string> UtilAssemblerMulti::validArgs() const
{
        return {
            "--input" , "-i",
            "--output", "-o",
            "--quiet" , "-q",
        };
}
