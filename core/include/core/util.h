#pragma once
#include <stdlib.h>
#include <algorithm>
#include <type_traits>

namespace cheeky::core {

template <size_t MaxSize, typename Type, typename... Types>
struct GetMaxSize {
    /// @brief Get the size of the largest possible Type in Types...
    /// @tparam Types...
    /// @retval size_t
    constexpr static size_t value() {
        constexpr size_t cur_max = std::max(sizeof(Type), MaxSize);
        if constexpr (sizeof...(Types) == 0) {
            return cur_max;
        } else {
            return GetMaxSize<cur_max, Types...>::value();
        }
    }
};

template <typename... Types>
struct GetMaxSize<0, std::variant<Types...>> {
    /// @brief Get the size of the largest possible Type in std::variant
    /// @tparam std::variant<Types...> 
    /// @retval size_t
    constexpr static size_t value() {
        return GetMaxSize<0, Types...>::value();
    }
};

template <typename T, typename Variant> 
struct IsValidVarType : std::bool_constant<false> {};

template <typename T, typename... Types> 
struct IsValidVarType<T, std::variant<Types...>> {
        constexpr static size_t value = (std::is_same_v<Types, std::decay_t<T>> || ...);
};

constexpr inline uint32_t log2(int32_t data) {
    unsigned res = -1;   
    while (data != 0) {
        data >>= 1;
        res++;
    }
    return res;
};
}