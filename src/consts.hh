/**
 * File: consts.hh
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

#ifndef CONSTS_HH
#define CONSTS_HH

#include "src/types.hh"

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

#endif /// CONSTS_HH
