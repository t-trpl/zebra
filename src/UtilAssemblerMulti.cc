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
        const auto maybeInput = argToIter(map, {"--input", "-i", "input"});
        if (!maybeInput)
                return maybeInput.error();
        if (const auto ptr = *maybeInput; ptr != map.end()) {
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
        const auto errOutput = setMemberPath(map, {"--output", "-o", "output"},
            out_);
        if (errOutput)
                return *errOutput;
        return None;
}

Error UtilAssemblerMulti::run() const
{
        if (!silence_)
                std::cout << util::banner << "\nAssembling\n";
        std::ofstream outFile(out_);
        if (!outFile)
                return "Failed to open: " + out_;
        const auto maybeSize = writeStripe(files_, outFile);
        if (!maybeSize)
                return maybeSize.error();
        const auto size = *maybeSize;
        if (!silence_)
                std::cout << "Wrote " << out_ << " " << size << " bytes\n";
        return None;
}

Error UtilAssemblerMulti::setFlags(const ArgMap& map)
{
        const auto maybeQuiet = validFlag(map, {"-q", "--quiet"});
        if (!maybeQuiet)
                return maybeQuiet.error();
        if (*maybeQuiet)
                silence_ = true;
        return None;
}

std::unordered_set<std::string> UtilAssemblerMulti::validArgs() const
{
        return {
            "-i", "--input",
            "-o", "--output",
            "-q", "--quiet",
        };
}
