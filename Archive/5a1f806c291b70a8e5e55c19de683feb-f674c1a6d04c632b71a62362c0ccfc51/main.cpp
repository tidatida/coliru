#include <iostream>
#include <string>
#include <vector>


template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
    for (auto& el : vec)
    {
        os << el << ' ';
    }
    return os;
}


int main()
{
    std::vector<std::string> words = {
        "C'est", "plate", "en", "crisse", "icitte", "!"
    };
    std::cout << words << std::endl;
}
