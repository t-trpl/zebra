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
#include "src/utils.hh"
#include "src/helpers.hh"
#include <unordered_map>
#include <vector>
#include <iostream>

Error Parser::runParse(const Args& args)
{
    std::string mode = "";
    for (auto ptr = args.begin(); ptr != args.end(); ptr++) {
        const auto& left = *ptr;
        if (isMode(left)) {
            if (!mode.empty())
                return "Two modes";
            else 
                mode = left;
        }
        else if (isOpt(left)) {
            if (argMap_.find(left) != argMap_.end())
                return "Duplicate " + left;
            argMap_[left];
            ptr++;
            while (ptr != args.end() && noLeadingHyphen(*ptr))
                argMap_[left].push_back(*ptr++);
            ptr--;
        }
        else if (!left.empty())
            return "Bad arg " + left;
        else
            return "Empty arg";
    }
    mode_ = getMode(mode);
    if (mode_ == Mode::NONE) {
        if (mode.empty())
            return "No mode";
        else
            return "Bad mode " + mode;
    }
    return None;
}


void Parser::printHelper() const{ 
    if (containsMap(argMap_, {"-h", "--help"}))
        std::cout << util::help;
}


bool Parser::noLeadingHyphen(const std::string& str) const
{
    return str.size() > 0 && str[0] != '-';
}

std::vector<std::string> Parser::MapOr(const ArgMap map, const ArgOr& options)
    const
{
    if (const auto ptr = map.find(options.first); ptr != map.end())
        return ptr->second;
    if (const auto ptr = map.find(options.second); ptr != map.end())
        return ptr->second;
    return std::vector<std::string>();
}


Parser::Mode Parser::getMode(const std::string& mode) const
{
    if (mode == "-A" || mode == "--Assemble") {
        const auto& val = MapOr(argMap_, {"--input", "-i"});
        return val.size() > 1 ? Mode::ASM_MULTI : Mode::ASM;
    }
    else if (mode == "-S" || mode == "--Stripe")
        return Mode::STRIPE;
    return Mode::NONE;
}

Error Parser::ensureRequired() const
{
    const std::vector<ArgT> required = {
        {"--input", "-i", "input"},
        {"--output", "-o", "output"},
    };
    for (const auto& r : required) {
        const auto maybeRequired = argToValue(argMap_, r);
        if (!maybeRequired)
            return maybeRequired.error();
        if (*maybeRequired == argMap_.end())
            return "Missing " + std::get<2>(r);
    }
    return None;
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

template<typename T>
Maybe<UtilPtr> Parser::createPtr()
{
    auto ptr = std::make_unique<T>();
    const auto unknown = ptr->checkForUnknown(argMap_);
    if (unknown)
        return make_bad<UtilPtr>(*unknown);
    const auto argErr = ptr->setArgs(argMap_);
    if (argErr)
        return make_bad<UtilPtr>(*argErr);
    ptr->setFlags(argMap_);
    return ptr;
}

Maybe<UtilPtr> Parser::createUtil()
{
    switch (mode_) {
    case Mode::STRIPE :
        return createPtr<UtilStripe>();
    case Mode::ASM :
        return createPtr<UtilAssembler>();
    case Mode::ASM_MULTI :
        return createPtr<UtilAssemblerMulti>();
    case Mode::NONE : { }
    }
    return make_bad<UtilPtr>("No type");
}
