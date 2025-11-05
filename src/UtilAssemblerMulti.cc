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
#include "src/helpers.hh"
#include <iostream>
#include <fstream>

Error UtilAssemblerMulti::setArgs(const ArgMap& map)
{
        const auto input = argToIter(map, { "--input", "-i", "input" });
        if (!input)
                return input.error();
        if (const auto ptr = *input; ptr != map.end()) {
                FilesL acc = nullptr;
                for (auto p = ptr->second; p; p = p->next)
                        if (const auto part = toPath(p->val); part)
                                acc = push(*part, acc);
                        else
                                part.error();
                files_ = reverseN(acc);
        } else {
                return "Missing Input";
        }
        const auto output = setPath(map, { "--output", "-o", "output" }, out_);
        if (output)
                return *output;
        return None;
}

Error UtilAssemblerMulti::run() const
{
        if (!silence_)
                std::cout << util::banner << "\nAssembling\n";
        std::ofstream output(out_);
        if (!output)
                return "Failed to open: " + out_;
        const auto bytes = writeStripe(files_, output);
        if (!bytes)
                return bytes.error();
        if (!silence_)
                std::cout << "Wrote " << out_ << " " << *bytes << " bytes\n";
        return None;
}

Error UtilAssemblerMulti::setFlags(const ArgMap& map)
{
        const auto quiet = validFlag(map, { "--quiet", "-q" });
        if (!quiet)
                return quiet.error();
        if (*quiet)
                silence_ = true;
        return None;
}

std::unordered_set<std::string> UtilAssemblerMulti::validArgs() const
{
        return {
            "--input" , "-i",
            "--output", "-o",
            "--quiet" , "-q",
        };
}
