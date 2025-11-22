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
#include "types.hh"

Error Parser::runParse(const ArgList args)
{
        if (!args)
                return NONE;
        const auto& arg = args->val;
        if (isMode(arg)) {
                if(!mode_.empty())
                        return "Two Modes";
                else
                        mode_ = arg;
        } else if (isOpt(arg)) {
                if (argMap_.find(arg) != argMap_.end())
                        return "Duplicate " + arg;
                const auto [next, acc] = nextOption(args->next);
                if (emptyArg(acc))
                        return arg + " empty option";
                argMap_[arg] = acc;
                return runParse(next);
        } else if (!arg.empty()) {
                return "Bad arg " + arg;
        } else {
                return "Empty arg";
        }
        return runParse(args->next);
}

bool Parser::emptyArg(ArgList args) const
{
        if (!args)
                return false;
        if (args->val.empty())
                return true;
        return emptyArg(args->next);
}

OptData Parser::nextOption(ArgList args) const
{
        ArgList acc = nullptr;
        while (args && !leadingHyphen(args->val)) {
                acc = push(args->val, acc);
                args = args->next;
        }
        return { args, reverseN(acc) };
}

bool Parser::checkHelp() const
{
        return contains({ "-h", "--help" });
}

bool Parser::leadingHyphen(const std::string& str) const
{
        return str.size() > 0 && str[0] == '-';
}

ArgList Parser::mapOr(const ArgOr& options) const
{
        if (const auto it = argMap_.find(options.first); it != argMap_.end())
                return it->second;
        if (const auto it = argMap_.find(options.second); it != argMap_.end())
                return it->second;
        return nullptr;
}

bool Parser::contains(const ArgOr& options) const
{
        if (argMap_.find(options.first) != argMap_.end())
                return true;
        if (argMap_.find(options.second) != argMap_.end())
                return true;
        return false;
}

Error Parser::conflict() const
{
        const Conflict conflicting = {
            {
                { "--no-extension", "-ne" },
                { "--extension", "-e" },
                "No-extension and Extension not possible"
            },
            {
                { "--parts", "-p" },
                { "--size", "-s" },
                "Parts and size flag not possible"
            },
        };
        for (const auto& [opt1, opt2, err] : conflicting)
                if (contains(opt1) && contains(opt2))
                        return err;
        return NONE;
}

Parser::Mode Parser::toMode(const std::string& mode) const
{
        if (mode == "-A" || mode == "--Assemble") {
                const auto& val = mapOr({ "--input", "-i" });
                return count(val) > 1 ? Mode::ASM_MULTI : Mode::ASM;
        }
        if (mode == "-S" || mode == "--Stripe") {
                const auto& p = contains({ "--parts", "-p" });
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
        if (const auto e = conflict())
                return makeBad<UtilPtr>(*e);
        const auto mode = toMode(mode_);
        auto ptr = createPtr(mode);
        if (!ptr)
                return makeBad<UtilPtr>("No mode");
        if (const auto e = ptr->checkForUnknown(argMap_))
                return makeBad<UtilPtr>(*e);
        if (const auto e = ptr->setArgs(argMap_))
                return makeBad<UtilPtr>(*e);
        if (const auto e = ptr->setFlags(argMap_))
                return makeBad<UtilPtr>(*e);
        return ptr;
}
