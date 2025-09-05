/**
 * File: UtilAssembler.hh
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

#ifndef UTIL_ASSEMBLER_HH
#define UTIL_ASSEMBLER_HH

#include "src/UtilBaseSingle.hh"
#include "src/Maybe.hh"

class UtilAssembler : public UtilBaseSingle {
private:
    std::string ext_ = "stripe";
    bool useExt_ = true;
    std::string name_ = "";
    std::string stemToName(const std::string& stem) const;
    Maybe<Files> loadFileNames() const;
    std::unordered_set<std::string> getValidOptionsFlags() const override;
public:
    UtilAssembler() { }
    ~UtilAssembler() { }
    UtilAssembler(const UtilAssembler&) = delete;
    Error run() const override;
    Error setArgs(const ArgMap& map) override;
    Error setFlags(const ArgMap& map) override;
};

#endif /// STRIPE_ASSEMBLER_HH
