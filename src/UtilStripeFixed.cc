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
#include "src/utils.hh"

std::unordered_set<std::string> UtilStripeFixed::validArgs() const
{
     return {
          "-i", "--input",
          "-o", "--output",
          "-n", "--name",
          "-e", "--extension",
          "-np", "--no-padding",
          "-q", "--quiet",
          "-ne", "--no-extension",
          "-p", "--parts",
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
               return make_bad<int>("Bad Parts " + parts); 
     return std::stoi(parts);
}

Error UtilStripeFixed::setArgs(const ArgMap& map)
{
     const auto baseError = UtilStripeBase::setArgs(map);
     if (baseError)
          return *baseError;
     const auto maybeParts = argToIter(map, {"--parts", "-p", "parts"});
     if (!maybeParts)
          return maybeParts.error();
     const auto partsPtr = (*maybeParts)->second;
     switch (count(partsPtr)) {
     case 0:
          return "No parts";
     case 1: {
          const auto maybePartsInt = stringToParts(partsPtr->val);
          if (!maybePartsInt)
               return maybePartsInt.error();
          parts_ = *maybePartsInt;
          break;
     }
     default:
          return "Too many parts";
     }
     return None;
}
