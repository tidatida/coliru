set -x; clang++ -O3 -std=c++11 -march=native -Wall -Wextra -Winline -pedantic -pthread main.cpp && time ./a.out -pedantic