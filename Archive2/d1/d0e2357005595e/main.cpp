#include <memory>
#include <type_traits>
#include <utility>

template <typename T>
class remove_pointer_ {
    template <typename U=T>
    static auto test(int) -> typename std::remove_reference<decltype(*std::declval<U>())>::type;
    static auto test(...) -> T;
public:
    using type = decltype(test(0));
};

template <typename T>
using remove_pointer = typename remove_pointer_<T>::type;

int main() {
    static_assert(std::is_same<int,remove_pointer<int*>>::value, "");
    static_assert(std::is_same<int,remove_pointer<int>>::value, "");
    static_assert(std::is_same<int,remove_pointer<std::unique_ptr<int>>>::value, "");
    static_assert(std::is_same<int,remove_pointer<std::shared_ptr<int>>>::value, "");
}
