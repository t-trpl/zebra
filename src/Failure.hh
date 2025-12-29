/**
 * File: Failure.hh
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

#ifndef FAILURE_HH
#define FAILURE_HH

#include <mutex>
#include <atomic>
#include <string>

class Failure {
protected:
        std::mutex fmtx_;
        std::atomic<bool> failure_ = false;
        std::string fmsg_;
        void fail(const std::string& msg);
public:
        Failure() = default;
        virtual ~Failure() = default;
        Failure(const Failure&) = delete;
};

#endif /// FAILURE_HH
