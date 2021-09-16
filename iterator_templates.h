#pragma once

#include <type_traits>

// default case
template <class T, class = void>
struct is_iterator : std::false_type
{
};


// specialization
template <class T>
struct is_iterator<T,
        std::void_t<typename std::iterator_traits<T>::difference_type,
                typename std::iterator_traits<T>::pointer,
                typename std::iterator_traits<T>::reference,
                typename std::iterator_traits<T>::value_type,
                typename std::iterator_traits<T>::iterator_category>> : std::true_type
{
};

template <class T>
constexpr bool is_iterator_v = is_iterator<T>::value;