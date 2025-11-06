/**
 * File: UtilStripeFixed.cc
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

#include "src/UtilStripeFixed.hh"
#include "src/helpers.hh"

std::unordered_set<std::string> UtilStripeFixed::validArgs() const
{
        return {
            "--input"       , "-i" ,
            "--output"      , "-o" ,
            "--name"        , "-n" ,
            "--extension"   , "-e" ,
            "--no-padding"  , "-np",
            "--quiet"       , "-q" ,
            "--no-extension", "-ne",
            "--parts"       , "-p" ,
        };
}

size_t UtilStripeFixed::getStripeSize(const size_t& fsize) const
{
        const auto bsize = fsize / parts_;
        return fsize % parts_ ? bsize + 1 : bsize;
}

Maybe<int> UtilStripeFixed::stringToParts(const std::string& parts) const
{
        for (const auto c : parts)
                if (!isdigit(c))
                        return makeBad<int>("Bad Parts " + parts);
        return std::stoi(parts);
}

Error UtilStripeFixed::setArgs(const ArgMap& map)
{
        if (const auto e = UtilStripeBase::setArgs(map))
                return *e;
        const auto parts = argToIter(map, { "--parts", "-p", "parts" });
        if (!parts)
                return parts.error();
        const auto ptr = (*parts)->second;
        switch (count(ptr)) {
        case 0:
                return "No parts";
        case 1: {
                const auto parts = stringToParts(ptr->val);
                if (!parts)
                        return parts.error();
                if (*parts == 0)
                        return "Can't have zero parts";
                parts_ = *parts;
                break;
        }
        default:
                return "Too many parts";
        }
        return NONE;
}
