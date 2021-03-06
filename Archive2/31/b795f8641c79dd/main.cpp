#include <iostream>
#include <string>
#include <vector>

using namespace std;


template<typename T0, typename T1>
struct my_pair{
    my_pair(T0 el0, T1 el1): el0(el0), el1(el1) {}
    T0 el0;
    T1 el1; // may be next pair
};

template<typename T>
T get_my(const T& t, int i, int current = 0)
{
    return t;
}

///
/// ERROR occurs here!
///
template<typename T0, typename T1>
auto get_my(const my_pair<T0, T1>& p, int i,  int current = 0)
-> decltype( get_my(p.el1, i, ++current) )
{
    if (current == i) {
        return get_my(p.el0, i, ++current);
    } else {
        return get_my(p.el1, i, ++current);
    }
}

template<typename T0, typename T1>
my_pair<T0, T1> make_my_pair(T0 el0, T1 el1){
    return my_pair<T0, T1>(el0, el1);
}



int main()
{
    my_pair<double, my_pair<double,double>> p1(12.789, {1.5, 3.14} );
    
    auto el1 = get_my(p1, 0);
    auto el2 = get_my(p1, 1);
    cout<< el1 << '\n' << el2;
    return 0;
}
