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

Error UtilAssemblerMulti::setArgs(const ArgMapN& map)
{
    const auto maybeInput = argToValue(map, {"--input", "-i", "input"});
    if (!maybeInput)
        return maybeInput.error();
    if (const auto ptr = *maybeInput; ptr != map.end()) {
        auto p = ptr->second;
        for (auto p = ptr->second; p; p = p->next) {
            if (const auto part = getPath(p->val); part)
                parts_.push_back(*part);
            else
                part.error();
        }
    }
    else
        return "Missing Input";
    const auto errorOutput = setMemberReqPath(map, {"--output", "-o", "output"},
            out_);
    if (errorOutput)
        return *errorOutput;
    return None;
}

Error UtilAssemblerMulti::run() const
{
    if (!silence_)
        std::cout << util::banner << "\nAssembling\n";
    std::ofstream outFile(out_);
    if (!outFile)
        return "Failed to open: " + out_;
    if (!silence_)
        std::cout << "\033[32m->\033[0m" << out_ << "\n";
    for (const auto& p : parts_) {
        std::ifstream file(p, std::ios::binary);
        if (!file)
            return "Failed to open: " + p + "\nDiscard output";
        outFile << file.rdbuf();
    }
    return None;
}

Error UtilAssemblerMulti::setFlags(const ArgMapN& map)
{
    const auto maybeQuiet = validFlag(map, {"-q", "--quiet"});
    if (!maybeQuiet)
        return maybeQuiet.error();
    else if (*maybeQuiet)
        silence_ = true;
    return None;
}

std::unordered_set<std::string> UtilAssemblerMulti::getValidOptionsFlags() const
{
    return {
        "-i", "--input",
        "-o", "--output",
        "-q", "--quiet",
    };
}
