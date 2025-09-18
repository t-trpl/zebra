/**
 * File: UtilBaseSingle.cc
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

Error UtilBaseSingle::setArgs(const ArgMap& map)
{
     const auto errInput = setMemberPath(map, {"--input", "-i", "input"}, in_);
     if (errInput)
          return *errInput;
     const auto errOutput = setMemberPath(map, {"--output", "-o", "output"},
               out_);
     if (errOutput)
          return *errOutput;
     return None;
}
