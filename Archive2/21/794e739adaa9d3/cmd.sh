set -x;
clang++ -std=c++14 -O3 -Wall -pedantic -pthread main.cpp -stdlib=libc++ && ./a.out
g++ -std=c++14 -O3 -Wall -pedantic -pthread main.cpp && ./a.out