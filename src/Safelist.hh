/** 
 * File: Safelist.hh
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

#include <memory>

#ifndef SAFE_LIST_HH
#define SAFE_LIST_HH

namespace ty {

template <typename T>
struct node;

template <typename T>
using Safelist = std::shared_ptr<node<T>>;

template <typename T>
struct node {
    T val;
    Safelist<T> next = nullptr;
    node(const T& v) : val(v) { }
    node(const T& v, Safelist<T> top) : val(v), next(top) { }
};

template <typename T>
Safelist<T> push(const T& val, Safelist<T> top)
{
    return std::make_shared<node<T>>(val, top);
}

template <typename T>
Safelist<T> pop(Safelist<T> top)
{
    return top ? top->next : nullptr;
}

template <typename T>
void sort(Safelist<T> head) {
    if (!head)
        return;
    bool swapped;
    do {
        swapped = false;
        Safelist<T> curr = head;
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
Safelist<T> reverse(Safelist<T> head)
{
    Safelist<T> prev = nullptr;
    Safelist<T> curr = head;
    while (curr) {
        Safelist<T> next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev; 
}

} /// ty

#endif // SAFE_LIST_HH
