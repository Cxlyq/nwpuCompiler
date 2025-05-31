# 建议复制粘贴运行，不要直接运行整个文件
# 抽象语法树
# ./build/minic -S -A -T -o ./tests/test1-1.png ./tests/test1-1.c
# IR生成
./build/minic -S -A -I -o ./tests/test1-1-0.ir ./tests/test1-1.c
clang ./tests/test1-1.c -emit-llvm -S -c -o ./tests/test1-1-1.ll
./tools/IRCompiler_df -S -L -o ./tests/test1-1-2.ll ./tests/test1-1.c