echo -e '\ng++/libstdc++' && g++ -std=c++11 -O2 -Wall -pedantic-errors main.cpp && ./a.out
echo -e '\nclang++/libc++' && clang++ -std=c++11 -stdlib=libc++ -O2 -Wall -pedantic-errors main.cpp -lsupc++ && ./a.out