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
#include "src/utils.hh"
#include "types.hh"

Error Parser::runParse(const ArgList args)
{
        if (!args)
                return NONE;
        const auto& arg = args->val_;
        if (isMode(arg)) {
                if(!mode_.empty())
                        return "Two Modes";
                else
                        mode_ = arg;
        } else if (isOpt(arg)) {
                if (argMap_.find(arg) != argMap_.end())
                        return "Duplicate " + arg;
                const auto acc = ty::takeWhile(args->next_, [](const auto& s) {
                        return !(s.size() > 0 && s[0] == '-');
                });
                if (ty::any(acc, [](const auto& s) { return s.empty(); }))
                        return arg + " empty option";
                argMap_[arg] = acc;
                return runParse(ty::drop(args, ty::count(acc) + 1));
        } else if (!arg.empty()) {
                return "Bad arg " + arg;
        } else {
                return "Empty arg";
        }
        return runParse(args->next_);
}

bool Parser::checkHelp() const
{
        return util::contains(argMap_, { "-h", "--help" });
}

Parser::Mode Parser::toMode(const std::string& mode) const
{
        if (mode == "-A" || mode == "--Assemble") {
                const auto& val = util::mapOr(argMap_, { "--input", "-i" });
                return ty::count(val) > 1 ? Mode::ASM_MULTI : Mode::ASM;
        }
        if (mode == "-S" || mode == "--Stripe") {
                const auto& p = util::contains(argMap_, { "--parts", "-p" });
                return p ? Mode::STRIPE_FIXED : Mode::STRIPE;
        }
        return Mode::NONE;
}

bool Parser::isMode(const std::string& left) const
{
        const auto end = left.end();
        if (left.size() < 2)
                return false;
        auto it = left.begin();
        for (int hypthen = 0; it != end && *it == '-'; it++)
                if (++hypthen > 2)
                        return false;
        if (it == end || !isUpper(*it++))
                return false;
        for (; it != end; it++)
                if (!isLower(*it) && *it != '-')
                        return false;
        return true;
}

bool Parser::isOpt(const std::string& left) const
{
        const auto end = left.end();
        if (left.size() < 2)
                return false;
        auto it = left.begin();
        for (int hypthen = 0; it != end && *it == '-'; it++)
                if (++hypthen > 2)
                        return false;
        if (it == end)
                return false;
        for (; it != end; it++)
                if (!isLower(*it) && *it != '-')
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

UtilPtr Parser::createPtr(const Mode m) const
{
        switch (m) {
        case Mode::STRIPE :
                return std::make_unique<UtilStripe>();
        case Mode::STRIPE_FIXED :
                return std::make_unique<UtilStripeFixed>();
        case Mode::ASM :
                return std::make_unique<UtilAssembler>();
        case Mode::ASM_MULTI :
                return std::make_unique<UtilAssemblerMulti>();
        default :
                return nullptr;
        }
}

Maybe<UtilPtr> Parser::createUtil() const
{
        const auto mode = toMode(mode_);
        auto ptr = createPtr(mode);
        if (!ptr)
                return makeBad<UtilPtr>("No mode");
        if (const auto e = ptr->conflict(argMap_))
                return makeBad<UtilPtr>(*e);
        if (const auto e = ptr->checkForUnknown(argMap_))
                return makeBad<UtilPtr>(*e);
        if (const auto e = ptr->setArgs(argMap_))
                return makeBad<UtilPtr>(*e);
        if (const auto e = ptr->setFlags(argMap_))
                return makeBad<UtilPtr>(*e);
        return ptr;
}
