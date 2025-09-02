/**
 * File: UtilBaseSingle.hh
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

#ifndef UTIL_BASE_SINGLE_HH
#define UTIL_BASE_SINGLE_HH

#include "src/UtilBase.hh"
#include <string>

class UtilBaseSingle : public UtilBase {
protected:
    /// input/ output directories
    std::string in_;
    std::string out_;
public:
    UtilBaseSingle() { } 
    virtual ~UtilBaseSingle() { }
    UtilBaseSingle(const UtilBaseSingle&) = delete;
    virtual Error setArgs(const ArgMap& map) override;
};

#endif /// UTIL_BASE_SINGLE_HH
