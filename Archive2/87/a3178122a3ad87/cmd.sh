for batch_size in 1 2 4 8 16 32 64 128 256 512; do 
  g++-4.8 -DBATCH_SIZE=${batch_size} -std=c++11 -O2 -Wall -pedantic -pthread main.cpp -ltbb
  printf "batch_size=${batch_size}\twork_counter=$(./a.out)\n"
done | column -t