/**
 * File: UtilStripeBase.hh
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

#include "src/types.hh"
#include "src/Maybe.hh"
#include "src/UtilBaseSingle.hh"
#include <string>
#include <fstream>
#include <vector>

#ifndef UTIL_STRIPE_BASE_HH
#define UTIL_STRIPE_BASE_HH

class UtilStripeBase : public UtilBaseSingle {
protected:
     std::string name_ = "";
     std::string ext_ = "stripe";
     bool padding_ = true;
     bool useExt_ = true;
     std::streamsize fileSize(std::ifstream& file) const;
     size_t stripeLength(const std::streamsize& size, const size_t& stripeSize)
               const;
     std::string fileName(const int& number, const size_t& len) const;
     size_t numberLength(const size_t& rem) const;
     std::streamsize readChunk(std::ifstream& file, std::vector<char>& buffer)
               const;
     std::string stripePath(const size_t& num, const size_t& max,
               const std::string& out) const;
     virtual size_t getStripeSize(const size_t& fsize) const = 0;

     std::streamsize chunk(std::ifstream& input, std::ofstream& output,
               std::streamsize remaining) const;
public:
     UtilStripeBase() { }
     virtual ~UtilStripeBase() { }
     UtilStripeBase(const UtilStripeBase&) = delete;
     Error run() const override;
     Error setFlags(const ArgMap& map) override;
     virtual Error setArgs(const ArgMap& map) override;
};

#endif /// UTIL_STRIPE_BASE_HH
