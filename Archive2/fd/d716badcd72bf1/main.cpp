#include <iostream>
#include <type_traits>

template<typename T>
void processPointer(T* ptr) {
    std::cout << "Hello!\n";
    static_assert(std::is_same<void,T>::value, "Just to drive the point home.");
}

void processPointer(void*) = delete;

int main () {
    processPointer<void>((void*)0);
}
