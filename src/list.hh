/** 
 * File: list.hh
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

#ifndef LIST_HH
#define LIST_HH

namespace ty {

template <typename T>
struct node;

template <typename T>
using list = std::shared_ptr<node<T>>;

template <typename T>
struct node {
     T val;
     list<T> next = nullptr;
     node(const T& v) : val(v) { }
     node(const T& v, list<T> head) : val(v), next(head) { }
};

template <typename T>
list<T> cons(const T& val, const list<T> top)
{
     return std::make_shared<node<T>>(val, top);
}

template <typename T>
list<T> cdr(const list<T> head)
{
     return head ? head->next : nullptr;
}

template <typename T>
T car(const list<T> head)
{
     return head->val;
}

template <typename T>
list<T> duplicate(const list<T> head)
{
     if (!head)
          return nullptr;
     return std::make_shared<node<T>>(head->val, duplicate(head->next));
}

template <typename T>
void sort(const list<T> head)
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
list<T> sortN(const list<T> head)
{
     const auto newHead = duplicate(head);
     sort(newHead);
     return newHead;
}

template <typename T>
void reverse(list<T>& head)
{
    if (!head || !head->next)
        return;
    auto first = head;
    auto rest = head->next;
    reverse(rest);
    first->next->next = first;
    first->next = nullptr;
    head = rest;
}

template <typename T>
list<T> reverseN(const list<T> head)
{
     auto newHead = duplicate(head);
     reverse(newHead);
     return newHead;
}

template <typename T>
int count(const list<T> head)
{
     return head ? 1 + count(head->next) : 0;
}

} /// ty

#endif // LIST_HH
