/**
 * File: types.hh
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

#ifndef TYPES_HH
#define TYPES_HH

#include "src/List.hh"
#include <unordered_map>
#include <string>
#include <optional>
#include <vector>
#include <mutex>
#include <atomic>

using ArgList = ty::List<std::string>;

using ArgMap = std::unordered_map<std::string, ArgList>;

using Error = std::optional<std::string>;

using ArgT = std::tuple<std::string, std::string, std::string>;

using ArgOr = std::pair<std::string, std::string>;

using MapIt = ArgMap::const_iterator;

using FilesL = ty::List<std::string>;

using Conflict = std::vector<std::tuple<ArgOr, ArgOr, std::string>>;

inline const auto NONE = std::nullopt;

inline const ArgT EXT_A = { "--extension", "-e", "extension" };

inline const ArgT NAME_A = { "--name", "-n", "name" };

inline const ArgT OUT_A = { "--output", "-o", "output" };

inline const ArgT IN_A = { "--input", "-i", "input" };

inline const ArgT SIZE_A = { "--size", "-s", "size" };

inline const ArgT PARTS_A = { "--parts", "-p", "parts" };

inline const ArgT THREADS_A = { "--threads", "-t", "threads" };

inline const ArgOr QUIET_F = { "--quiet", "-q" };

inline const ArgOr NO_EXT_F = { "--no-extension", "-ne" };

inline const ArgOr NO_NAME_F = { "--no-name", "-nn" };

inline const ArgOr NO_PAD_F = { "--no-padding", "-np" };

class Failure {
protected:
        std::mutex fmtx_;
        std::atomic<bool> failure_ = false;
        std::string fmsg_;
public:
        Failure() = default;
        virtual ~Failure() = default;
        Failure(const Failure&) = delete;
};

struct WD {
        int start;
        int end;
        size_t len;
        size_t size;
};

#endif /// TYPES_HH
