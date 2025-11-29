/**
 * File: List.hh
 *
 * Simple reference counting linked list with some useful functions.
 *
 * Functions take list first and then additional arguments. This is so lambdas
 * can cleanly extend multiple lines.
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

#ifndef LIST_HH
#define LIST_HH

#include <memory>

namespace ty {

template <typename T>
struct Node;

template <typename T>
using List = std::shared_ptr<Node<T>>;

template <typename T>
struct Node {
        T val;
        List<T> next = nullptr;
        Node(const T& v) : val(v) { }
        Node(const T& v, List<T> head) : val(v), next(head) { }
};

/// not to be used directly
namespace detail {

template <typename T>
void sortInPlace(const List<T> head)
{
        if (!head)
                return;
        bool swapped;
        do {
                swapped = false;
                auto curr = head;
                while (curr->next) {
                        if (curr->val > curr->next->val) {
                                std::swap(curr->val, curr->next->val);
                                swapped = true;
                        }
                        curr = curr->next;
                }
        } while (swapped);
}

template <typename T>
List<T> reverseInPlace(List<T> head)
{
        if (!head || !head->next)
                return head;
        auto rh = detail::reverseInPlace(head->next);
        head->next->next = head;
        head->next = nullptr;
        return rh;
}

} /// detail

template <typename T>
List<T> push(const T& val, const List<T> head)
{
        return std::make_shared<Node<T>>(val, head);
}

template <typename T>
List<T> copy(const List<T> head)
{
        if (!head)
                return nullptr;
        return std::make_shared<Node<T>>(head->val, copy(head->next));
}

template <typename T>
List<T> sort(const List<T> head)
{
        const auto newHead = copy(head);
        detail::sortInPlace(newHead);
        return newHead;
}

template <typename T>
List<T> reverse(const List<T> head)
{
        auto c = copy(head);
        return detail::reverseInPlace(c);
}

template <typename T>
int count(const List<T> head)
{
        return head ? 1 + count(head->next) : 0;
}

template <typename T, typename F>
List<T> map(const List<T> head, F&& f)
{
        if (!head)
                return nullptr;
        return push(f(head->val), map(head->next, std::forward<F>(f)));
}

template <typename T, typename F>
bool any(const List<T> head, F&& f)
{
        if (!head)
                return false;
        if (f(head->val))
                return true;
        return any(head->next, std::forward<F>(f));
}

template <typename T>
List<T> drop(const List<T> head, const int cnt)
{
        if (!head)
                return nullptr;
        if (!cnt)
                return head;
        return drop(head->next, cnt - 1);
}

template <typename T, typename F>
List<T> takeWhile(const List<T> head, F&& f)
{
        if (!head || !f(head->val))
                return nullptr;
        return push(head->val, takeWhile(head->next, std::forward<F>(f)));
}

} /// ty

#endif /// LIST_HH
