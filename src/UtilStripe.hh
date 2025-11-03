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

#include "src/UtilStripeBase.hh"
#include "src/types.hh"
#include "src/Maybe.hh"
#include <string>

class UtilStripe final : public UtilStripeBase {
private:
        size_t stripeSize_ = 3'000'000;
        Maybe<size_t> stringToBytes(const std::string& size) const;
        std::unordered_set<std::string> validArgs() const override;
        size_t getStripeSize(const size_t&) const override;
public:
        UtilStripe() = default;
        ~UtilStripe() = default;
        UtilStripe(const UtilStripe&) = delete;
        Error setArgs(const ArgMap& map) override;
};

#endif /// UTIL_STRIPE_HH
