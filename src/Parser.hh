/**
 * File: Parser.hh
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

#ifndef PARSER_HH
#define PARSER_HH

#include "src/UtilBase.hh"
#include "src/types.hh"
#include <string>
#include <memory>

using UtilPtr = std::unique_ptr<UtilBase>;

class Parser {
private:
    enum class Mode : unsigned char {
        NONE,
        STRIPE,
        ASM,
        ASM_MULTI,
    };
    std::string mode_;
    ArgMap argMap_;
    bool isUpper(const char c) const;
    bool isLower(const char c) const;
    bool isMode(const std::string& left) const;
    bool isOpt(const std::string& str) const;
    Mode getMode(const std::string& mode) const;
    template<typename T> Maybe<UtilPtr> createPtr();
    std::vector<std::string> MapOr(const ArgMap map, const ArgOr& options) const;
    bool noLeadingHyphen(const std::string& str) const;
public:
    Parser() { }
    ~Parser() { }
    Parser(const Parser&) = delete;
    Maybe<UtilPtr> createUtil();
    Error runParse(const Args& args);
    void printBanner() const;
    bool printHelper() const;
};

#endif /// STRIPE_PARSER_HH
