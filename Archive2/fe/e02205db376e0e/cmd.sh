ln -s main.cpp main.c 
clang -std=c11 -O3 -Wall -Wextra -pedantic-errors main.c && ./a.out <<< 'Programming 101!' && ./a.out <<< '++n ; B[5] = 0 ; --a ;'