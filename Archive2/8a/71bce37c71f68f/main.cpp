#include <iostream>
#include <nmmintrin.h>

int main()
{
    int s=0;
    for(int i=0;i<1000000000;i++){
		s+=__builtin_popcountll(i);
        //s+=_mm_popcnt_u64(x);
	}
    std::cout<<s;
}
