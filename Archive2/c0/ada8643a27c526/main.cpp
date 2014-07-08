#include <type_traits>
#include <string>
#include <iostream>

struct Ip
{
    std::string ip;
};

template <typename T>
typename std::enable_if<!std::is_integral<T>::value &&
                        !std::is_floating_point<T>::value, bool>::type 
    Equal(const std::string& s, const T& data)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return s == data;
}

template <typename T,
          typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
bool Equal(const std::string& s, const T& data)
{
    //int specific
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return false;
}

template <typename T,
          typename std::enable_if<std::is_floating_point<T>::value, T>::type* = nullptr>
bool Equal(const std::string& s, const T& data)
{
    //Float specific
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return false;
}

//Overload
bool Equal(const std::string& s, const Ip& data)
{
   //Ip specific
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return false;
} 

int main()
{
    Equal("21",21);
    Equal("21",21.0);
    Equal("21","42.5");
}
