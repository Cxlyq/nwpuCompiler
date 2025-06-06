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
## 关键数据结构：`ast_node`
### `ast_node`:
	- is_lvar;局部变量选项
	- node_type;结点类型选项
	- line_no;[叶子结点]行号信息
	- type;结点值的类型（int/float/void）
  	  特别注意对于 Type 与 type_attr 的区分
	- integer_val;[字面量]无符号整数字面量值
	- float_val;[字面量]float 类型字面量值
	- name;变量/函数名
	- parent;父节点
	- sons;[非叶结点]孩子结点向量
	- blockInsts;线性IR指令块
	- val;线性IR指令或者运行产生的Value
	- needScope;作用域管理选项
	- deep_copy();深拷贝函数
	- isLeafNode();是否为叶结点
	- insert_son_node();插入子节点
	- New();
	- Delete();
	TODO: @JEV055 [代码分析] 继续维护
## 整体功能逻辑：
- MiniC.g4 生成->MiniCLexer&amp;MiniCParser&amp;MiniCBaseVisitor;
- Antlr4Executor 调用->Antlr4CSTVisitor:MiniCBaseVisitor 遍历具体语法树生成抽象语法树;
## HACK:[代码重构使用]根据需要完善这个部分

---
# IR/:
## 关键数据结构: `module`
### `module`:
#### private:
    - name;模块名
    - types;类型向量
    - scopeStack;变量作用域栈
    - currentFunc;[过程用]当前处理函数
    - funcMap;函数映射表（函数名-函数）
    - funcVector;函数列表
    - globalVariableMap;变量名映射表（全局变量）
    - globalVariableVector;全局变量列表
    - constIntMap;立即数表-整数
    - constFloatMap;立即数表-浮点数
#### protected:
	- findConstInt(int val);
	- findConstFloat(float val);获取值为val的字面量
	- newGlobalVariable(type,name);新建全局变量
	- findGlobalVariable(name);获取对应名称的全局变量
	- insertFunctionDirectly(func);插入函数到符号表
	- insertGlobalValueDirectly(val);插入全局值到符号表
    - insertConstIntDirectly(val);插入常值到符号表
    - insertConstFloatDirectly(val);插入常值到符号表
    - newGlobalArrayVariable(type,array_name,dims);新建全局数组变量
#### public:
	- toIRString();输出IR代码
	- 
## Generator/: 总生成器
### `IRGenerator.cpp`&amp;`IRGenerator.h`:


## Instructions/: 指令生成与打印

## Types/:

## Values/:

---
# symboltable/:

#