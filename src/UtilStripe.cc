/** 
 * File: UtilStripe.cc
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

#include "src/UtilStripe.hh"
#include "src/Maybe.hh"
#include "src/types.hh"
#include "src/utils.hh"
#include "src/helpers.hh"
#include <cctype>
#include <cstddef>
#include <unordered_map>
#include <algorithm>

std::unordered_set<std::string> UtilStripe::validArgs() const
{
        return {
            "-i" , "--input",
            "-o" , "--output",
            "-s" , "--size",
            "-n" , "--name",
            "-e" , "--extension",
            "-np", "--no-padding",
            "-q" , "--quiet",
            "-ne", "--no-extension",
        };
}

Error UtilStripe::setArgs(const ArgMap& map)
{
        const auto baseError = UtilStripeBase::setArgs(map);
        if (baseError)
                return *baseError;
        const auto maybeSize = argToIter(map, {"--size", "-s", "size"});
        if (!maybeSize)
                return maybeSize.error();
        if (const auto ptr = *maybeSize; ptr != map.end()) {
                const auto args = ptr->second;
                switch (count(args)) {
                case 0:
                        return "No size";
                case 1: {
                        const auto &size = args->val;
                        const auto bytes = stringToBytes(size);
                        if (bytes)
                                stripeSize_ = *bytes;
                        else
                                return bytes.error();
                        break;
                }
                default:
                        return "Too many sizes";
                }
        }
        return None;
}

Maybe<size_t> UtilStripe::stringToBytes(const std::string& size) const
{
        auto it = size.begin();
        while (it != size.end() && (std::isdigit(*it) || *it == '.'))
                it++;
        const std::string num(size.begin(), it);
        if (num.empty())
                return make_bad<size_t>("No size...");
        const auto d = std::count_if(num.begin(), num.end(), [](const auto c) {
                    return c == '.';
        });
        if (d > 1 || (it != size.end() && !std::isalpha(*it)))
                return make_bad<size_t>("Bad byte size");
        const std::unordered_map<std::string, size_t> map = {   
            {"b" , 1},
            {"kb", 1'000},
            {"mb", 1'000'000},
            {"gb", 1'000'000'000},
        };
        const auto suffix = mapv<std::string>(it, size.end(), [](const auto c) {
                return std::tolower(c);
        });
        const auto itr = map.find(suffix);
        const auto found = itr != map.end();
        if (!suffix.empty() && !found)
                return make_bad<size_t>("Bad suffix: " + suffix); 
        const size_t units = found ? itr->second : 1;
        const double dbytes = std::stod(num) * units;
        return static_cast<size_t>(dbytes);
}

size_t UtilStripe::getStripeSize(const size_t&) const
{
        return stripeSize_;
}
