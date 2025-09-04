/**
 * File: UtilStripe.hh
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

#ifndef UTIL_STRIPE_HH
#define UTIL_STRIPE_HH

#include "src/UtilBaseSingle.hh"
#include "src/types.hh"
#include "src/Maybe.hh"
#include <string>
#include <fstream>
#include <vector>

class UtilStripe : public UtilBaseSingle {
private:
    size_t stripeSize_ = 3'000'000;
    std::string name_ = "";
    std::string ext_ = "stripe";
    bool padding_ = true;
    bool useExt_ = true;
    std::streamsize getFileSize(const std::string& location) const;
    size_t maxNameLength(const std::streamsize& size) const ;
    std::string getFileName(const int& number, const size_t& len) const;
    Maybe<size_t> stringToBytes(const std::string& size) const;
    size_t stripesStrLen(const size_t& rem) const;
    std::streamsize readChunk(std::ifstream& file, std::vector<char>& buffer)
        const;
    std::unordered_set<std::string> getValidOptionsFlags() const override;
    std::string getStripePath(const size_t& num, const size_t& max) const;
public:
    UtilStripe() { }
    ~UtilStripe() { }
    UtilStripe(const UtilStripe&) = delete;
    Error run() const override;
    Error setArgs(const ArgMap& map) override;
    void setFlags(const Flags& flags) override;
};

#endif /// UTIL_STRIPE_HH
