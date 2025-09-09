/**
 * File: Parser.cc
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

#include "src/Parser.hh"
#include "src/UtilAssembler.hh"
#include "src/UtilStripe.hh"
#include "src/UtilAssemblerMulti.hh"
#include "src/types.hh"
#include "src/helpers.hh"
#include <unordered_map>

Error Parser::runParse(const ArgN args)
{
    if (!args)
        return None;
    const auto left = car(args);
    const auto next = cdr(args);
    if (isMode(left)) {
        if(!mode_.empty())
            return "Two Modes";
        else
            mode_ = left;
    }
    else if (isOpt(left)) {
        if (argMapN_.find(left) != argMapN_.end())
            return "Duplicate " + left;
        const auto [next, acc] = getOption(cdr(args), left, nullptr);
        argMapN_[left] = acc;
        return runParse(next);
    }
    else if (!left.empty())
        return "Bad arg " + left;
    else
        return "Empty arg";
    return runParse(next);
}

std::pair<ArgN, ArgN> Parser::getOption(const ArgN args, const std::string& left,
        const ArgN acc)
{
    if (!args || leadingHyphen(car(args)))
        return std::make_pair(args, reverse(acc));
    const auto val = car(args);
    return getOption(cdr(args), left, cons(val, acc));
}

bool Parser::printHelper() const
{ 
    return containsMap(argMapN_, {"-h", "--help"});
}

bool Parser::leadingHyphen(const std::string& str) const
{
    return str.size() > 0 && str[0] == '-';
}

ArgN Parser::MapOr(const ArgMapN map, const ArgOr& options)
    const
{
    if (const auto ptr = map.find(options.first); ptr != map.end())
        return ptr->second;
    if (const auto ptr = map.find(options.second); ptr != map.end())
        return ptr->second;
    return nullptr;
}

Parser::Mode Parser::getMode(const std::string& mode) const
{
    if (mode == "-A" || mode == "--Assemble") {
        const auto& val = MapOr(argMapN_, {"--input", "-i"});
        return count(val) > 1 ? Mode::ASM_MULTI : Mode::ASM;
    }
    else if (mode == "-S" || mode == "--Stripe")
        return Mode::STRIPE;
    return Mode::NONE;
}

bool Parser::isMode(const std::string& left) const
{
    const auto end = left.end();
    if (left.size() < 2)
        return false;
    auto ptr = left.begin();
    for (int hypthen = 0; ptr != end && *ptr == '-'; hypthen++, ptr++)
        if (hypthen >= 2)
            return false;
    if (ptr == end || !isUpper(*ptr++))
        return false;
    for (; ptr != end; ptr++)
        if (!isLower(*ptr) && *ptr != '-')
            return false;
    return true;
}

bool Parser::isOpt(const std::string& left) const
{
    const auto end = left.end();
    if (left.size() < 2)
        return false;
    auto ptr = left.begin();
    for (int hypthen = 0; ptr != end && *ptr == '-'; hypthen++, ptr++)
        if (hypthen >= 2)
            return false;
    if (ptr == end)
        return false;
    for (; ptr != end; ptr++)
        if (!isLower(*ptr) && *ptr != '-')
            return false;
    return true;
}

bool Parser::isUpper(const char c) const
{
    return c >= 'A' && c <= 'Z';
}

bool Parser::isLower(const char c) const
{
    return c >= 'a' && c <= 'z';
}

Maybe<UtilPtr> Parser::createUtil() const
{
    const auto mode = getMode(mode_);
    switch (mode) {
    case Mode::STRIPE :
        return createPtr<UtilStripe>();
    case Mode::ASM :
        return createPtr<UtilAssembler>();
    case Mode::ASM_MULTI :
        return createPtr<UtilAssemblerMulti>();
    case Mode::NONE : { }
    }
    return make_bad<UtilPtr>("No mode");
}
