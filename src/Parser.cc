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
#include "src/UtilStripeFixed.hh"
#include "src/types.hh"
#include "src/helpers.hh"

Error Parser::runParse(const ArgList args)
{
        if (!args)
                return None;
        const auto& left = args->val;
        if (isMode(left)) {
                if(!mode_.empty())
                        return "Two Modes";
                else
                        mode_ = left;
        } else if (isOpt(left)) {
                if (argMap_.find(left) != argMap_.end())
                        return "Duplicate " + left;
                const auto [next, acc] = nextOption(args->next);
                argMap_[left] = acc;
                return runParse(next);
        } else if (!left.empty()) {
                return "Bad arg " + left;
        } else {
                return "Empty arg";
        }
        return runParse(args->next);
}

OptData Parser::nextOption(ArgList args) const
{
        ArgList acc = nullptr;
        while (args && !leadingHyphen(args->val)) {
                acc = push(args->val, acc);
                args = args->next;
        }
        return {args, reverseN(acc)};
}

bool Parser::checkHelp() const
{ 
        return containsMap(argMap_, {"-h", "--help"});
}

bool Parser::leadingHyphen(const std::string& str) const
{
        return str.size() > 0 && str[0] == '-';
}

ArgList Parser::mapOr(const ArgMap& map, const ArgOr& options) const
{
        if (const auto ptr = map.find(options.first); ptr != map.end())
                return ptr->second;
        if (const auto ptr = map.find(options.second); ptr != map.end())
                return ptr->second;
        return nullptr;
}

Parser::Mode Parser::toMode(const std::string& mode) const
{
        if (mode == "-A" || mode == "--Assemble") {
                const auto& val = mapOr(argMap_, {"--input", "-i"});
                return count(val) > 1 ? Mode::ASM_MULTI : Mode::ASM;
        }
        if (mode == "-S" || mode == "--Stripe") {
                const auto& p = mapOr(argMap_, {"--parts", "-p"});
                return p ? Mode::STRIPE_FIXED : Mode::STRIPE;
        }
        return Mode::NONE;
}

bool Parser::isMode(const std::string& left) const
{
        const auto end = left.end();
        if (left.size() < 2)
                return false;
        auto ptr = left.begin();
        for (int hypthen = 0; ptr != end && *ptr == '-'; ptr++)
                if (++hypthen > 2)
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
        for (int hypthen = 0; ptr != end && *ptr == '-'; ptr++)
                if (++hypthen > 2)
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
        const auto mode = toMode(mode_);
        switch (mode) {
        case Mode::STRIPE :
                return createPtr<UtilStripe>();
        case Mode::STRIPE_FIXED:
                return createPtr<UtilStripeFixed>();
        case Mode::ASM :
                return createPtr<UtilAssembler>();
        case Mode::ASM_MULTI :
                return createPtr<UtilAssemblerMulti>();
        case Mode::NONE : { }
        }
        return make_bad<UtilPtr>("No mode");
}
