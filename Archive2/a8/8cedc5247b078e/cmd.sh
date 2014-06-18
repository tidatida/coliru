##clang++ --version
##g++ --version
##set -x ; g++ -std=c++11 -Wall -Wextra -Wconversion -pedantic  -O0 -pedantic  -pthread main.cpp && ./a.out
##g++ --version
##set -x ; g++-4.8 -std=c++11 -Wall -Wextra -Wconversion -pedantic  -O0  -pedantic  -pthread -S main.cpp && cat main.s
##set -x ; clang++ -std=c++11 -fsanitize=undefined -Wall -Wextra -Wconversion -pedantic -O0  -Wall -pthread  main.cpp && ./a.out
##set -x ; gcc -x c -std=c99 -O0  -W -Wall -Wextra -pedantic -Wconversion  main.cpp && ./a.out
set -x ; gcc -x c -std=c99 -O0 -W -Wall -Wextra -Wconversion -pedantic -S main.cpp && cat main.s | grep log
##set -x ; clang -x c -std=c99 -fsanitize=undefined -Wall -Wextra -Wconversion -Weverything  -O0  -pedantic main.cpp && ./a.out