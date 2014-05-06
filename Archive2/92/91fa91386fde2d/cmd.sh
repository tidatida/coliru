ln -s /Archive2/41/6ed1526614ed8e/main.cpp malloc_hook.cpp
clang++ -std=c++11 -O2 -fPIC -c malloc_hook.cpp
clang++ -shared -Wl,-soname,malloc_hook -ldl -o libmalloc_hook.so malloc_hook.o
clang++ -std=c++11 -O2 -Wall -pedantic main.cpp
LD_PRELOAD=./libmalloc_hook.so ./a.out