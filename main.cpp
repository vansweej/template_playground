#include <iostream>

#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <string_view>

#include "iterator_templates.h"

namespace typeName {
    template <typename T>
    constexpr std::string_view wrapped_type_name () {
#ifdef __clang__
        return __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
        return  __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
        return  __FUNCSIG__;
#endif
    }

    class probe_type;
    constexpr std::string_view probe_type_name ("typeName::probe_type");
    constexpr std::string_view probe_type_name_elaborated ("class typeName::probe_type");
    constexpr std::string_view probe_type_name_used (wrapped_type_name<probe_type> ().find (probe_type_name_elaborated) != -1 ? probe_type_name_elaborated : probe_type_name);

    constexpr size_t prefix_size () {
        return wrapped_type_name<probe_type> ().find (probe_type_name_used);
    }

    constexpr size_t suffix_size () {
        return wrapped_type_name<probe_type> ().length () - prefix_size () - probe_type_name_used.length ();
    }

    template <typename T>
    std::string_view type_name () {
        constexpr auto type_name = wrapped_type_name<T> ();

        return type_name.substr (prefix_size (), type_name.length () - prefix_size () - suffix_size ());
    }
}

using typeName::type_name;
using typeName::probe_type;

template<typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &input)
{
    for (auto const &i: input) {
        os << i << " ";
    }
    return os;
}

template<typename Seq, typename Pred>
Seq filter(Seq s, Pred p) {
    using f = std::function<bool(typename Seq::value_type)>;

    Seq result;

    std::remove_copy_if(std::begin(s), std::end(s),
                        std::back_inserter(result),
                        std::not1(f(p)));

    return result;
}

template<typename Seq, typename Unary>
Seq map(Seq s,  Unary u) {
    using TResult = typename std::result_of<Unary&(typename Seq::value_type&)>::type;

    std::cout << type_name<TResult> () << std::endl;

    using f = std::function<TResult(typename Seq::value_type)>;

    Seq result;

    std::transform(std::begin(s), std::end(s), std::back_inserter(result), f(u));

    return result;
}

int main() {
    std::vector<int> test = {1, 2, 3 , 4, 5, 6, 7, 8, 9};

    auto is_even = [](int x) {return x % 2 == 0;};

    auto result1 = filter(test, is_even);

    auto square = [](int x) {return x*x;};

    auto result2 = map(result1, square);

    std::cout << result2 << std::endl;

    auto result3 = map(result2, [](int x) {return x*10.0f;});

    std::cout << result3 << std::endl;

    return 0;
}
