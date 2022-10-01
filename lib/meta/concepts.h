#pragma once

namespace utils {
    template <typename T>
    concept is_container = requires(T t)
    {
        { t.get() };
        { t->size() };
    };

    template <typename T>
    concept is_pointer = requires(T t)
    {
        { *t };
        { t[0] };
    };

    template <typename T>
    concept is_normal = !is_pointer<T> && !is_container<T>;
}