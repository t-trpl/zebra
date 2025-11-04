/**
 * File: Maybe.hh
 *
 * Contains either a value or an error message.
 *
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

#ifndef MAYBE_HH
#define MAYBE_HH

#include <string>
#include <variant>

template<typename T>
class Maybe {
private:
        struct Dummy { }; /// needed std::variant collision
        using err = std::conditional_t<std::is_same_v<T, std::string>,
                                       Dummy,
                                       std::string>;
        bool hasValue_ = false;
        std::variant<T, err> value_;
        Maybe(const std::string& msg, bool);
        Maybe(std::string&& msg, bool);
public:
        ~Maybe() = default;
        template<typename U> Maybe(const U& val);
        template<typename U> Maybe(U&& val) noexcept;
        template<typename U> Maybe(std::initializer_list<U> il);
        template<typename U> Maybe<T>& operator=(U&& val) noexcept;
        template<typename U> Maybe<T>& operator=(const U& val);
        template<typename U> Maybe<T>& operator=(std::initializer_list<U> il);
        explicit operator bool() const;
        T& operator*();
        const T& operator*() const;
        static Maybe<T> bad(const std::string& err);
        static Maybe<T> bad(std::string&& err);
        std::string& error();
        const std::string& error() const;
        template<typename U> T valueOr(const U& fallback) const;
        template<typename U> T valueOr(U&& fallback) const;
        T&& extract();
        T* operator->();
        const T* operator->() const;
        Maybe<T>& fail(const std::string& err);
        Maybe<T>& fail(std::string&& err);
};

template<typename T>
template<typename U>
Maybe<T>::Maybe(const U& val)
    : hasValue_(true)
    , value_(static_cast<T>(val))
{

}

template<typename T>
template<typename U>
Maybe<T>::Maybe(U&& val) noexcept
    : hasValue_(true)
{
        if constexpr (std::is_constructible_v<T, U&&>)
                value_ = T(std::forward<U>(val));
        else
                value_ = static_cast<T>(std::forward<U>(val));
}

template<typename T>
Maybe<T> makeBad(const std::string& err)
{
        return Maybe<T>::bad(err);
}

template<typename T>
Maybe<T> makeBad(std::string&& err)
{
        return Maybe<T>::bad(std::forward<std::string>(err));
}

template<typename T>
Maybe<T>::Maybe(const std::string& msg, bool)
    : hasValue_(false)
    , value_(msg)
{

}

template<typename T>
Maybe<T>::Maybe(std::string&& msg, bool)
    : hasValue_(false)
    , value_(std::forward<std::string>(msg))
{

}

template<typename T>
template<typename U>
Maybe<T>::Maybe(std::initializer_list<U> il)
    : hasValue_(true)
    , value_(T(il.begin(), il.end()))
{

}

template<typename T>
template<typename U>
Maybe<T>& Maybe<T>::operator=(U&& val) noexcept
{
        hasValue_ = true;
        if constexpr (std::is_constructible_v<T, U&&>)
                value_ = T(std::forward<U>(val));
        else
                value_ = static_cast<T>(std::forward<U>(val));
        return *this;
}

template<typename T>
template<typename U>
Maybe<T>& Maybe<T>::operator=(const U& val)
{
        hasValue_ = true;
        value_ = static_cast<T>(val);
        return *this;
}

template<typename T>
template<typename U>
Maybe<T>& Maybe<T>::operator=(std::initializer_list<U> il)
{
        hasValue_ = true;
        value_ = T(il.begin(), il.end());
        return *this;
}

template<typename T>
Maybe<T>::operator bool() const
{
        return hasValue_;
}

template<typename T>
T& Maybe<T>::operator*()
{
        return std::get<T>(value_);
}

template<typename T>
const T& Maybe<T>::operator*() const
{
        return std::get<T>(value_);
}

template<typename T>
Maybe<T> Maybe<T>::bad(const std::string& err)
{
        return Maybe<T>(err, 0);
}

template<typename T>
Maybe<T> Maybe<T>::bad(std::string&& err)
{
        return Maybe<T>(std::forward<std::string>(err), 0);
}

template<typename T>
std::string& Maybe<T>::error()
{
        return std::get<std::string>(value_);
}

template<typename T>
const std::string& Maybe<T>::error() const
{
        return std::get<std::string>(value_);
}

template<typename T>
template<typename U>
T Maybe<T>::valueOr(const U& fallback) const
{
        if (*this)
                return **this;
        return static_cast<T>(fallback);
}

template<typename T>
template<typename U>
T Maybe<T>::valueOr(U&& fallback) const
{
        if (*this)
                return **this;
        return std::forward<T>(fallback);
}

template<typename T>
T&& Maybe<T>::extract()
{
        return std::move(**this);
}

template<typename T>
T* Maybe<T>::operator->()
{
        return &**this;
}

template<typename T>
const T* Maybe<T>::operator->() const
{
        return &**this;
}

template<typename T>
Maybe<T>& Maybe<T>::fail(const std::string& err)
{
        hasValue_ = false;
        value_ = err;
        return *this;
}

template<typename T>
Maybe<T>& Maybe<T>::fail(std::string&& err)
{
        hasValue_ = false;
        value_ = std::forward<std::string>(err);
        return *this;
}

#endif /// MAYBE_HH
