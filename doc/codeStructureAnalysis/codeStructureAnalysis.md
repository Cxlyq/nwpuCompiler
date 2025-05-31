# 维护标准示例：
## example/:(如果需要指明的话)目录整体实现功能
- 关键数据结构：
- 整体功能逻辑（文件级）：
### `example.cpp`&amp;`example.h`:文件整体功能
- 关键数据结构：
- 功能逻辑（函数级）：
- example1()：实现功能（逻辑）与数据结构
---
# `main.cpp`:主程序
- `main()`:拼好程序，调用`ArgsAnalysis()`,`compile()`
- `ArgsAnalysis()`:编译选项设置
- `compile()`:编译总过程，在生成最终汇编时主要调用:
  `new Antlr4Executor()`,`Antlr4Executor.run()`,生成抽象语法树;
  `new Module()`,`ast2IR()`,生成符号表与线性IR;
  [`Module->renameIR()`],根据需要将行号与寄存器变量按序列命名;
  `new CodeGeneratorArm32()`,`CodeGenerator.run()`,生成汇编代码;
  [`CodeGenerator->setShowLinearIR()`],根据需要将线性IR作为注释加注到汇编代码中
---
# frontend/:
### 关键数据结构：`ast_node`
### 整体功能逻辑：
- MiniC.g4 生成->MiniCLexer&amp;MiniCParser&amp;MiniCBaseVisitor;
- Antlr4Executor 调用->Antlr4CSTVisitor:MiniCBaseVisitor 遍历具体语法树生成抽象语法树;
## HACK:[代码重构使用]根据需要完善这个部分

---
TODO: @JEV055 [代码阶段性整合] 继续代码结构分析
# IR/:
## Generator/: 总生成器
### `IRGenerator.cpp`&amp;`IRGenerator.h`:


## Instructions/: 指令生成与打印

## Types/:

## Values/:

---
# symboltable/:

#