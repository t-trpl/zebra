/** 
 * File: Maybe.hh
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

#include <string>
#include <variant>

#ifndef MAYBE_HH
#define MAYBE_HH

template<typename T>
class Maybe {
private:
     struct Dummy { }; /// needed std::variant collision
     using err = std::conditional_t<
          std::is_same_v<T, std::string>,
          Dummy,
          std::string
          >;
     bool hasValue_ = false;
     std::variant<T, err> value_;
     Maybe(const std::string& msg, bool);
     Maybe(std::string&& msg, bool);
public:
     Maybe(const T& val);
     ~Maybe();
     Maybe(T&& val) noexcept;
     Maybe(const Maybe<T>& other);
     template<typename U> Maybe(std::initializer_list<U> il);
     template<typename U> Maybe(const Maybe<U>& other);
     template<typename U> Maybe(Maybe<U>&& other);
     template<typename U> Maybe(const U& val);
     template<typename U> Maybe(U&& val);
     Maybe<T>& operator=(Maybe<T>&& other) noexcept;
     Maybe<T>& operator=(const Maybe<T>& other);
     explicit operator bool() const;
     T& operator*();
     const T& operator*() const;
     static Maybe<T> bad(const std::string& err);
     static Maybe<T> bad(std::string&& err);
     std::string error() const;
     template<typename U> T valueOr(const U& fallback) const;
     T extract();
     T* operator->();
     const T* operator->() const;
};

template<typename T>
Maybe<T> make_bad(const std::string& err)
{
     return Maybe<T>::bad(err);
}

template<typename T>
Maybe<T> make_bad(std::string&& err)
{
     return Maybe<T>::bad(std::forward<std::string>(err));
}

template<typename T>
Maybe<T>::Maybe(const T& val)
     : hasValue_(true)
     , value_(val)
{

}

template<typename T>
Maybe<T>::~Maybe() 
{

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
Maybe<T>::Maybe(T&& val) noexcept 
     : hasValue_(true)
     , value_(std::move(val))
{

}

template<typename T>
Maybe<T>::Maybe(const Maybe<T>& other) 
     : hasValue_(other.hasValue_)
     , value_(other.value_)
{

}

template<typename T>
Maybe<T>& Maybe<T>::operator=(Maybe<T>&& other) noexcept
{
     if (this != &other) {
          hasValue_ = other.hasValue_;
          value_ = std::move(other.value_);
     }
     return *this;
}

template<typename T>
Maybe<T>& Maybe<T>::operator=(const Maybe<T>& other)
{
     if (this != &other) {
          hasValue_ = other.hasValue_;
          value_ = other.value_;
     }
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
std::string Maybe<T>::error() const
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
Maybe<T>::Maybe(const Maybe<U>& other)
{
     if (other) {
          hasValue_ = true;
          value_ = static_cast<T>(*other);
     }
     else {
          hasValue_ = false;
          value_ = other.error();
     }
}

template<typename T>
template<typename U>
Maybe<T>::Maybe(const U& val)
     : hasValue_(true)
     , value_(static_cast<T>(val))
{

}

template<typename T>
template<typename U>
Maybe<T>::Maybe(Maybe<U>&& other)
{
     if (other) {
          hasValue_ = true;
          if constexpr (std::is_constructible_v<T, U&&>)
               value_ = T(std::forward<U>(*other));
          else 
               value_ = static_cast<T>(std::forward<U>(*other));
     }
     else {
          hasValue_ = false;
          value_ = std::forward<std::string>(other.error());
     }
}

template<typename T>
template<typename U>
Maybe<T>::Maybe(std::initializer_list<U> il)
     : hasValue_(true)
     , value_(il)
{

}

template<typename T>
template<typename U>
Maybe<T>::Maybe(U&& val)
     : hasValue_(true)
{
     if constexpr (std::is_constructible_v<T, U&&>)
          value_ = T(std::forward<U>(val));
     else 
          value_ = static_cast<T>(std::forward<U>(val));
}

template<typename T>
T Maybe<T>::extract()
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

#endif /// MAYBE_HH
