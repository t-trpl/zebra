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
#include "src/helpers.hh"
#include <filesystem>

namespace fs = std::filesystem;

Error UtilBase::setMemberBase(const ArgMap& map, const ArgT& opt,
    std::string& ref, bool required)
{
        const auto maybeOpt = argToIter(map, opt);
        const auto tag = std::get<2>(opt);
        if (!maybeOpt)
                return maybeOpt.error();
        if (const auto ptr = *maybeOpt; ptr != map.end()) {
                const auto args = ptr->second;
                switch (count(args)) {
                case 0:
                        return "Unmatched "+ tag;
                case 1: {
                        const auto val = args->val;
                        if (!val.empty())
                                ref = val;
                        else
                                return "No " + tag;
                        break;
                }
                default:
                        return "Too many "+ tag + "s";
                };
        } else if (required) {
                return "Missing " + tag;
        }
        return None;
}

Error UtilBase::setMember(const ArgMap& map, const ArgT& opt,
    std::string& ref)
{
        return setMemberBase(map, opt, ref, false);
}

Error UtilBase::setMemberPath(const ArgMap& map, const ArgT& opt,
    std::string& ref)
{
        std::string interimPath;
        const auto err = setMemberBase(map, opt, interimPath, true); 
        if (err)
                return err;
        const auto path = toPath(interimPath);
        if (!path)
                return path.error();
        ref = *path;
        return None;
}

bool UtilBase::isSlash(const char c) const
{
        return c == '/' || c == '\\';
}

std::string UtilBase::clean(const std::string& path) const 
{
        const int last = path.size() - 1;
        if (last > 0 && isSlash(path[last]))
                return path.substr(0, last);
        return path;
}

Error UtilBase::checkForUnknown(const ArgMap& map) const
{
        const auto valid = validArgs();
        for (const auto& p : map)
                if (valid.find(p.first) == valid.end())
                        return "Unrecognized " + p.first;
        return None;
}

Maybe<std::string> UtilBase::toPath(const std::string& p) const
{
        if (p.empty())
                return make_bad<std::string>("No path provided");
        const std::string cwd = fs::current_path().string();
        const std::string path = !isSlash(p[0]) ? std::string(fs::path(cwd) / p)
                                                : p;
        const auto cleaned = clean(path);
        return cleaned;
}
