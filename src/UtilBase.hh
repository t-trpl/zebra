/**
 * File: UtilBase.hh
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

#include "src/Maybe.hh"
#include "src/types.hh"
#include <string>
#include <unordered_set>

#ifndef UTIL_BASE_HH
#define UTIL_BASE_HH

class UtilBase {
private:
    Error setMemberBase(const ArgMap& map, const ArgT& opt, std::string& ref,
            bool required);
protected:
    bool silence_ = false;
    Maybe<std::string> getPath(const std::string& loc) const;
    bool isSlash(const char c) const;
    std::string clean(const std::string& path) const;
    Error setMemberReqPath(const ArgMap& map, const ArgT& opt,
            std::string& memRef);
    Error setMember(const ArgMap& map, const ArgT& opt, std::string& memRef);
    virtual std::unordered_set<std::string> getValidOptionsFlags() const = 0;
public:
    UtilBase() { } 
    virtual ~UtilBase() { }
    UtilBase(const UtilBase&) = delete;
    virtual Error run() const = 0;
    virtual Error setFlags(const ArgMap& map) = 0;
    virtual Error setArgs(const ArgMap& map) = 0;
    Error checkForUnknown(const ArgMap& map) const;
};

#endif /// UTIL_BASE_HH
