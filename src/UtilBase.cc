/**
 * File: UtilBase.cc
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
#include "src/utils.hh"
#include <filesystem>

namespace fs = std::filesystem;

Error UtilBase::setMemberBase(const ArgMap& map, const ArgT& opt,
    std::string& ref, bool required)
{
        const auto mIt = argToIter(map, opt);
        const auto name = std::get<2>(opt);
        if (!mIt)
                return mIt.error();
        if (const auto it = *mIt; it != map.end()) {
                const auto args = it->second;
                switch (ty::count(args)) {
                case 0:
                        return "Unmatched "+ name;
                case 1: {
                        const auto val = args->val;
                        if (!val.empty())
                                ref = val;
                        else
                                return "No " + name;
                        break;
                }
                default:
                        return "Too many "+ name + "s";
                };
        } else if (required) {
                return "Missing " + name;
        }
        return NONE;
}

Error UtilBase::conflict(const ArgMap& map) const
{
        const auto c = conflicting();
        for (const auto& [opt1, opt2, err] : c)
                if (util::contains(map, opt1) && util::contains(map, opt2))
                        return err;
        return NONE;
}

Error UtilBase::setMember(const ArgMap& map, const ArgT& opt,
    std::string& ref)
{
        return setMemberBase(map, opt, ref, false);
}

Error UtilBase::setPath(const ArgMap& map, const ArgT& opt, std::string& ref)
{
        std::string interim;
        if (const auto e = setMemberBase(map, opt, interim, true))
                return e;
        ref = toPath(interim);
        return NONE;
}

bool UtilBase::isSlash(const char c) const
{
        return c == '/' || c == '\\';
}

Error UtilBase::checkForUnknown(const ArgMap& map) const
{
        const auto valid = validArgs();
        for (const auto& p : map)
                if (valid.find(p.first) == valid.end())
                        return "Unrecognized " + p.first;
        return NONE;
}

std::string UtilBase::toPath(const std::string& p) const
{
        const std::string cwd = fs::current_path().string();
        if (p.empty())
                return p;
        if (isSlash(p[0]))
                return p;
        if (p.size() == 1 && p[0] == '.')
                return fs::path(cwd);
        auto it = p.begin();
        while (it + 1 < p.end() && (*it == '.' && isSlash(*(it + 1))))
                it += 2;
        return std::string(fs::path(cwd) / std::string(it, p.end()));
}

Maybe<bool> UtilBase::validFlag(const ArgMap& map, const ArgOr& arg) const
{
        const auto& [full, abrv] = arg;
        const auto it1 = map.find(full);
        const auto it2 = map.find(abrv);
        const auto end = map.end();
        if (it1 != end && it2 != end)
                return makeBad<bool>("Duplicate flags");
        if (it1 == end && it2 == end)
                return false;
        const auto c = it1 != end ? it1 : it2;
        if (c->second)
                return makeBad<bool>("Flag with args " + c->first);
        return true;
}

Maybe<MapIt> UtilBase::argToIter(const ArgMap& map, const ArgT& arg) const
{
        const auto& [full, abrv, name] = arg;
        const auto it1 = map.find(full);
        const auto it2 = map.find(std::get<1>(arg));
        const auto end = map.end();
        if (it1 != end && it2 != end)
                return makeBad<MapIt>("Duplicate option: " + name);
        if (it1 != end)
                return it1;
        if (it2 != end)
                return it2;
        return end;
}
