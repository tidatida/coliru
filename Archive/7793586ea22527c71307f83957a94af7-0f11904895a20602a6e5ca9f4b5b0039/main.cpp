#include <iostream>
#include <vector>
#include <iterator>
#include <type_traits>

template<class It>
struct is_special_iterator {
    static constexpr bool value = std::is_void<typename It::value_type>();
};

template<typename T, bool B>
struct iterator_check {};

template<typename T>
struct iterator_check<T, true> { using value_type = typename T::container_type::value_type; };

template<typename T>
struct iterator_check<T, false> { using value_type = typename T::value_type; };

template<typename T>
using IteratorCheck = typename iterator_check<T, is_special_iterator<T>::value>::value_type;

template<typename It>
void f(It begin) {
    using value_type = IteratorCheck<It>;
    std::cout << sizeof(value_type) << '\n';
}

int main() {
    std::vector<int> v = {1,2,3,4,5,6,7,8};
    f( std::back_inserter( v ) );
    f( std::begin( v ) );
}