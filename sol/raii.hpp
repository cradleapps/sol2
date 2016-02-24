// The MIT License (MIT)

// Copyright (c) 2013-2016 Rapptz and contributors

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef SOL_RAII_HPP
#define SOL_RAII_HPP

#include <memory>
#include "traits.hpp"

namespace sol {
namespace detail {
struct default_construct {
    template<typename T, typename... Args>
    void operator()(T&& obj, Args&&... args) const {
        std::allocator<meta::Unqualified<T>> alloc{};
        alloc.construct(obj, std::forward<Args>(args)...);
    }
};
} // detail

template<typename... Args>
using constructors = sol::types<Args...>;

const auto default_constructor = constructors<types<>>{};

template <typename... Functions>
struct constructor_wrapper : std::tuple<Functions...> {
    using std::tuple<Functions...>::tuple;
};

template <typename... Functions>
constructor_wrapper<Functions...> constructor(Functions&&... functions) {
    return constructor_wrapper<Functions...>(std::forward<Functions>(functions)...);
}

template <typename Function>
struct destructor_wrapper {
    Function fx;
    template <typename... Args>
    destructor_wrapper(Args&&... args) : fx(std::forward<Args>(args)...) {}
};

template <>
struct destructor_wrapper<void> {};

const destructor_wrapper<void> default_destructor{};

template <typename Fx>
inline destructor_wrapper<Fx> destructor(Fx&& fx) {
    return destructor_wrapper<Fx>(std::forward<Fx>(fx));
}

} // sol

#endif // SOL_RAII_HPP
