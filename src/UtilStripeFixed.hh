/**
 * File: UtilStripeFixed.hh
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

#include "src/UtilBaseSingle.hh"
#include "src/UtilStripeBase.hh"

#ifndef UTIL_STRIPE_FIXED_HH
#define UTIL_STRIPE_FIXED_HH

class UtilStripeFixed final : public UtilStripeBase {
private:
        std::unordered_set<std::string> validArgs() const override;
        int parts_ = 0;
        Maybe<int> stringToParts(const std::string& parts) const;
        size_t getStripeSize(const size_t& fsize) const override;
public:
        UtilStripeFixed() = default; 
        virtual ~UtilStripeFixed() = default;
        UtilStripeFixed(const UtilStripeFixed&) = delete;
        Error setArgs(const ArgMap& map) override;
};

#endif /// UTIL_STRIPE_FIXED_HH
