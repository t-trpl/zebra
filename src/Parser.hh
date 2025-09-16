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

#include "src/UtilBase.hh"
#include "src/types.hh"
#include <string>
#include <memory>

#ifndef PARSER_HH
#define PARSER_HH

using UtilPtr = std::unique_ptr<UtilBase>;
using OptData = std::pair<ArgN, ArgN>;

class Parser {
private:
     enum class Mode : unsigned char {
          NONE,
          STRIPE,
          ASM,
          ASM_MULTI,
     };
     std::string mode_;
     ArgMapN argMapN_;
     bool isUpper(const char c) const;
     bool isLower(const char c) const;
     bool isMode(const std::string& left) const;
     bool isOpt(const std::string& str) const;
     Mode toMode(const std::string& mode) const;
     template<typename T> Maybe<UtilPtr> createPtr() const;
     ArgN mapOr(const ArgMapN map, const ArgOr& options) const;
     bool leadingHyphen(const std::string& str) const;
     OptData nextOption(const ArgN args) const;
     OptData nextOptionI(const ArgN args, const ArgN acc) const;
public:
     Parser() { }
     ~Parser() { }
     Parser(const Parser&) = delete;
     Maybe<UtilPtr> createUtil() const;
     Error runParse(const ArgN args);
     bool checkHelp() const;
};

template<typename T>
Maybe<UtilPtr> Parser::createPtr() const
{
     auto ptr = std::make_unique<T>();
     const auto unknown = ptr->checkForUnknown(argMapN_);
     if (unknown)
          return make_bad<UtilPtr>(*unknown);
     const auto argErr = ptr->setArgs(argMapN_);
     if (argErr)
          return make_bad<UtilPtr>(*argErr);
     const auto flagErr = ptr->setFlags(argMapN_);
     if (flagErr)
          return make_bad<UtilPtr>(*flagErr);
     return ptr;
}

#endif /// STRIPE_PARSER_HH
