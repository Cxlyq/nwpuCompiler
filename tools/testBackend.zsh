
./build/minic -S -o tests/test1-1-1.s tests/test1-1.c
#使用clang生成汇编
clang -target riscv64 -march=rv64imafdc -mabi=lp64d -S tests/test1-1.c -o tests/test1-1-2.s -O0
