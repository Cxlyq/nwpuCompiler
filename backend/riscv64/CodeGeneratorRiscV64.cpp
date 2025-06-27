///
/// @file CodeGeneratorRiscV64.cpp
/// @brief RISCV64的后端处理实现
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-11-21
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-11-21 <td>1.0     <td>zenglj  <td>新做
/// </table>
///
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include "LocalVariable.h"
#include "Value.h"
#include "cmath"
#include "Function.h"
#include "Module.h"
#include "PlatformRiscV64.h"
#include "CodeGeneratorRiscV64.h"
#include "InstSelectorRiscV64.h"
#include "SimpleRegisterAllocatorRiscV64.h"
#include "ILocRiscV64.h"
#include "RegVariable.h"
#include "FuncCallInstruction.h"
#include "ArgInstruction.h"
#include "MoveInstruction.h"

/// @brief 构造函数
/// @param tab 符号表
CodeGeneratorRiscV64::CodeGeneratorRiscV64(Module * _module) : CodeGeneratorAsm(_module)
{}

/// @brief 析构函数
CodeGeneratorRiscV64::~CodeGeneratorRiscV64()
{}

/// @brief 产生汇编头部分
void CodeGeneratorRiscV64::genHeader()
{
    // 设置 RISC-V 架构：64-bit, 包含整数、乘法、原子、浮点、压缩指令扩展
    fprintf(fp, ".text\n");
    fprintf(fp, ".attribute 4, 15\n"); // lp64d ABI，或者根据你的工具链确认
    fprintf(fp, ".attribute 5, \"rv64i2p0_m2p0_a2p0_f2p0_d2p0_c2p0\"\n"); // 包含 d 扩展
    // TODO 是否需要输出源文件
    //  fprintf(fp, ".file \"generated_code.c\"\n");
}

/// @brief 全局变量Section，主要包含初始化的和未初始化过的
void CodeGeneratorRiscV64::genDataSection()
{
    // 生成代码段
    fprintf(fp, ".text\n");

    // 可直接操作文件指针fp进行写操作

    // 目前不支持全局变量和静态变量，以及字符串常量
    // 全局变量分两种情况：初始化的全局变量和未初始化的全局变量
    // TODO 数组输出还有问题
    for (auto var: module->getGlobalVariables()) {
        const std::string & name = var->getName();
        int                 size = var->getType()->getSize();
        int                 align = var->getAlignment();

        if (var->isInBSSSection()) {
            // 未初始化的全局变量，放sbss段
            fprintf(fp, "\n\t.type\t%s,@object\n", name.c_str());
            fprintf(fp, "\t.section\t.data,\"aw\",@progbits\n");
            fprintf(fp, "\t.globl\t%s\n", name.c_str());
            fprintf(fp, "\t.p2align\t%d\n", (int) std::log2(align));
            fprintf(fp, "%s:\n", name.c_str());
            if (var->getType()->isIntegerType()) {
                fprintf(fp, "\t.word\t0\n");
            } else if (var->getType()->isFloatType()) {
                fprintf(fp, "\t.word\t0x00000000\n");
            } else if (var->getType()->isArrayType()) {
                fprintf(fp, "\t.zero\t%d\n", size);
            }
            fprintf(fp, "\t.size\t%s, %d\n", name.c_str(), size);
        } else {
            // 初始化的全局变量，放rodata（只读）或data段，这里默认data
            fprintf(fp, "\n\t.type\t%s,@object\n", name.c_str());
            if (var->getValueCategory() == ValueCategory::CONSTANT) {
                fprintf(fp, "\t.section\t.rodata,\"a\",@progbits\n");

            } else {
                fprintf(fp, "\t.section\t.data,\"aw\",@progbits\n");
            }
            fprintf(fp, "\t.globl\t%s\n", name.c_str());
            fprintf(fp, "\t.p2align\t%d\n", (int) std::log2(align));
            fprintf(fp, "%s:\n", name.c_str());

            if (var->getType()->isIntegerType()) {
                fprintf(fp, "\t.word\t%d\n", var->getIntInitVal()); // 你需要提供这个方法返回初始化值
            } else if (var->getType()->isFloatType()) {
                //使用union，输出float型变量的IEEE 754 二进制形式
                float fval = var->getFloatInitVal();
                union {
                    float    f;
                    uint32_t u;
                } tmp = {fval};
                fprintf(fp, "\t.word\t0x%x\n", tmp.u);
            }
            if (var->getType()->isArrayType()) {
                auto                     arrayType = dynamic_cast<ArrayType *>(var->getType());
                const std::vector<int> & dims = arrayType->getDimensions();
                int                      totalElements = 1;
                for (int d: dims)
                    totalElements *= d;

                int elemSize = arrayType->getBaseElementType()->getSize();

                if (arrayType->getBaseElementType()->isIntegerType()) {
                    auto initVals = var->getInitIntVal();
                    int  initSize = initVals ? initVals->size() : 0;

                    // 最高维度slice大小 = 剩余维度乘积
                    int sliceSize = 1;
                    for (int j = 1; j < dims.size(); ++j)
                        sliceSize *= dims[j];

                    for (int i = 0; i < dims[0]; ++i) {
                        // 判断该slice是否全部为0
                        bool allZero = true;
                        for (int k = 0; k < sliceSize; ++k) {
                            int idx = i * sliceSize + k;
                            int val = (idx < initSize) ? (*initVals)[idx] : 0;
                            if (val != 0) {
                                allZero = false;
                                break;
                            }
                        }

                        if (allZero) {
                            fprintf(fp, "\t.zero\t%d\n", sliceSize * elemSize);
                        } else {
                            for (int k = 0; k < sliceSize; ++k) {
                                int idx = i * sliceSize + k;
                                int val = (idx < initSize) ? (*initVals)[idx] : 0;
                                fprintf(fp, "\t.word\t%d\n", val);
                            }
                        }
                    }

                    // 防御尾部未覆盖的情况，通常不会出现
                    if (totalElements > initSize) {
                        int remain = totalElements - initSize;
                        fprintf(fp, "\t.zero\t%d\n", remain * elemSize);
                    }
                } else if (arrayType->getBaseElementType()->isFloatType()) {
                    auto initVals = var->getInitFloatVal();
                    int  initSize = initVals ? initVals->size() : 0;

                    int sliceSize = 1;
                    for (int j = 1; j < dims.size(); ++j)
                        sliceSize *= dims[j];

                    for (int i = 0; i < dims[0]; ++i) {
                        bool allZero = true;
                        for (int k = 0; k < sliceSize; ++k) {
                            int   idx = i * sliceSize + k;
                            float val = (idx < initSize) ? (*initVals)[idx] : 0.0f;
                            if (val != 0.0f) {
                                allZero = false;
                                break;
                            }
                        }

                        if (allZero) {
                            fprintf(fp, "\t.zero\t%d\n", sliceSize * elemSize);
                        } else {
                            for (int k = 0; k < sliceSize; ++k) {
                                int   idx = i * sliceSize + k;
                                float val = (idx < initSize) ? (*initVals)[idx] : 0.0f;
                                union {
                                    float    f;
                                    uint32_t u;
                                } tmp = {val};
                                fprintf(fp, "\t.word\t0x%08x\n", tmp.u);
                            }
                        }
                    }

                    if (totalElements > initSize) {
                        int remain = totalElements - initSize;
                        fprintf(fp, "\t.zero\t%d\n", remain * elemSize);
                    }
                }
            }

            // 其他类型类似处理

            fprintf(fp, "\t.size\t%s, %d\n", name.c_str(), size);
            // 例如:
            // fprintf(fp, "\t.word\t%d\n", init_int_val);
            // fprintf(fp, "\t.double\t%f\n", init_double_val);
        }
    }
}

///
/// @brief 获取IR变量相关信息字符串
/// @param str
///
void CodeGeneratorRiscV64::getIRValueStr(Value * val, std::string & str)
{
    std::string name = val->getName();
    std::string IRName = val->getIRName();
    int32_t     regId = val->getRegId();
    int32_t     baseRegId;
    int64_t     offset;
    std::string showName;

    if (name.empty() && (!IRName.empty())) {
        showName = IRName;
    } else if ((!name.empty()) && IRName.empty()) {
        showName = IRName;
    } else if ((!name.empty()) && (!IRName.empty())) {
        showName = name + ":" + IRName;
    } else {
        showName = "";
    }

    if (regId != -1) {
        // 寄存器
        str += "\t@ " + showName + ":" + PlatformRiscV64::regName[regId];
    } else if (val->getMemoryAddr(&baseRegId, &offset)) {
        // 栈内寻址，[fp,#4]
        str += "\t@ " + showName + ":[" + PlatformRiscV64::regName[baseRegId] + ",#" + std::to_string(offset) + "]";
    }
}

/// @brief 针对函数进行汇编指令生成，放到.text代码段中
/// @param func 要处理的函数
void CodeGeneratorRiscV64::genCodeSection(Function * func)
{
    // 寄存器分配以及栈内局部变量的站内地址重新分配
    registerAllocation(func);
    // 获取函数的指令列表
    std::vector<Instruction *> & IrInsts = func->getInterCode().getInsts();

    // 汇编指令输出前要确保Label的名字有效，必须是程序级别的唯一，而不是函数内的唯一。要全局编号。
    for (auto inst: IrInsts) {
        if (inst->getOp() == IRInstOperator::IRINST_OP_LABEL) {
            inst->setName(IR_LABEL_PREFIX + std::to_string(labelIndex++));
        }
    }

    // ILOC代码序列
    ILocRiscV64          iloc(module);
    LiveVariableAnalysis lva;
    // printf("LVA addr = %p\n", &lva); // 看是否是 0x50 或其他非法值
    lva.run(func);
    simpleRegisterAllocator.buildGraph(lva); // 构建干涉图
    bool success = simpleRegisterAllocator.allocate();
    if (success) {
        // std::cout << "寄存器分配成功 ✅\n";
    } else {
        // std::cout << "部分变量需要溢出 ❌\n";
    }
    // 指令选择生成汇编指令
    InstSelectorRiscV64 instSelector(IrInsts, iloc, func, simpleRegisterAllocator);
    instSelector.setShowLinearIR(this->showLinearIR);
    instSelector.run();

    // 删除无用的Label指令
    iloc.deleteUnusedLabel();

    // ILOC代码输出为汇编代码

    fprintf(fp, ".globl %s\n", func->getName().c_str());
    fprintf(fp, ".p2align %d\n", func->getAlignment());
    fprintf(fp, ".type %s, @function\n", func->getName().c_str());
    fprintf(fp, "%s:\n", func->getName().c_str());

    // 开启时输出IR指令作为注释
    if (this->showLinearIR) {

        // 输出有关局部变量的注释，便于查找问题
        for (auto localVar: func->getVarValues()) {
            std::string str;
            getIRValueStr(localVar, str);
            if (!str.empty()) {
                fprintf(fp, "%s\n", str.c_str());
            }
        }

        // 输出指令关联的临时变量信息
        for (auto inst: func->getInterCode().getInsts()) {
            if (inst->hasResultValue()) {
                std::string str;
                getIRValueStr(inst, str);
                if (!str.empty()) {
                    fprintf(fp, "%s\n", str.c_str());
                }
            }
        }
    }

    iloc.outPut(fp);
}

/// @brief 寄存器分配
/// @param func 函数指针
void CodeGeneratorRiscV64::registerAllocation(Function * func)
{
    // 内置函数不需要处理
    if (func->isBuiltin()) {
        return;
    }

    // 最简单/朴素的寄存器分配简单，但性能差，具体如下：
    // (1) 局部变量都保存在内存栈中（含简单变量、下标变量等）
    // (2) 全局变量在静态存储.data区中
    // (3) 指令类的临时变量也保存在内存栈中，但是性能很差

    // RISCV64的函数调用约定：
    // R0,R1,R2和R3寄存器不需要保护，可直接使用
    // SP寄存器预留，不需要保护，但需要保证值的正确性
    // R4-R10, fp(11), lx(14)都需要保护，没有函数调用的函数可不用保护lx寄存器
    // 被保留的寄存器主要有：
    //  (1) FP寄存器用于栈寻址，即R11
    //  (2) LX寄存器用于函数调用，即R14。没有函数调用的函数可不用保护lx寄存器
    //  (3) R10寄存器用于立即数过大时要通过寄存器寻址，这里简化处理进行预留

    // 至少有FP和LX寄存器需要保护
    std::vector<int32_t> & protectedRegNo = func->getProtectedReg();
    protectedRegNo.clear();
    protectedRegNo.push_back(RISCV64_RA_REG_NO);
    protectedRegNo.push_back(RISCV64_FP_REG_NO);
    // 按照clang的标准，全都保存ra寄存器

    // if (func->getExistFuncCall()) {
    //     protectedRegNo.push_back(RISCV64_RA_REG_NO);
    // }

    // 调整函数调用指令，主要是前8个寄存器传值，后面用栈传递
    // 为了更好的进行寄存器分配，可以进行对函数调用的指令进行预处理
    // 当然也可以不做处理，不过性能更差。这个处理是可选的。
    // adjustFuncCallInsts(func);

    // 为局部变量和临时变量在栈内分配空间，指定偏移，进行栈空间的分配
    stackAlloc(func);

    // 函数形参要求前8个寄存器分配，后面的参数采用栈传递，实现实参的值传递给形参
    // 这一步是必须的
    adjustFormalParamInsts(func);

#if 0
    // 临时输出调整后的IR指令，用于查看当前的寄存器分配、栈内变量分配、实参入栈等信息的正确性
    std::string irCodeStr;
    func->renameIR();
    func->toString(irCodeStr);
    std::cout << irCodeStr << std::endl;
#endif
}

/// @brief 寄存器分配前对函数内的指令进行调整，以便方便寄存器分配
/// @param func 要处理的函数
void CodeGeneratorRiscV64::adjustFormalParamInsts(Function * func)
{
    // 函数形参的前四个实参值采用的是寄存器传值，后面栈传递

    auto & params = func->getParams();

    // 形参的前四个通过寄存器来传值R0-R3
    for (int k = 0; k < (int) params.size() && k <= 7; k++) {

        // 前四个设置分配寄存器
        params[k]->setRegId(k);
    }

    // 根据ARM版C语言的调用约定，除前8个外的实参进行值传递，逆序入栈
    int64_t fp_esp = func->getProtectedReg().size() * 8;
    for (int k = 8; k < (int) params.size(); k++) {

        params[k]->setMemoryAddr(RISCV64_FP_REG_NO, fp_esp);

        // 增加4字节，目前只支持int类型
        fp_esp += params[k]->getType()->getSize();
    }
}

/// @brief 寄存器分配前对函数内的指令进行调整，以便方便寄存器分配
/// @param func 要处理的函数
void CodeGeneratorRiscV64::adjustFuncCallInsts(Function * func)
{
    std::vector<Instruction *> newInsts;

    // 当前函数的指令列表
    auto & insts = func->getInterCode().getInsts();

    // 函数返回值用R0寄存器，若函数调用有返回值，则赋值R0到对应寄存器
    // 通过栈传递的实参，采用SP + 偏移的方式殉职，偏移肯定非负。
    for (auto pIter = insts.begin(); pIter != insts.end(); pIter++) {

        // 检查是否是函数调用指令，并且含有返回值
        if (Instanceof(callInst, FuncCallInstruction *, *pIter)) {

            // 实参前四个要寄存器传值，其它参数通过栈传递

            int32_t argNum = callInst->getOperandsNum();

            // 除前四个整数寄存器外，后面的参数采用栈传递
            int esp = 0;
            for (int32_t k = 4; k < argNum; k++) {

                // 获取实参的值
                auto arg = callInst->getOperand(k);

                // 栈帧空间（低地址在前，高地址在后）
                // --------------------- sp
                // 实参栈传递的空间（排除寄存器传递的实参空间）
                // ---------------------
                // 需要保存在栈中的局部变量或临时变量或形参对应变量空间
                // --------------------- fp
                // 保护寄存器的空间
                // ---------------------

                // 新建一个内存变量，把实参的值保存到栈中，以便栈传值，其寻址为SP + 非负偏移
                MemVariable * newVal = func->newMemVariable(IntegerType::getTypeInt());
                newVal->setMemoryAddr(RISCV64_SP_REG_NO, esp);
                esp += 8;

                // 引入赋值指令，把实参的值保存到内存变量上
                Instruction * assignInst = new MoveInstruction(func, newVal, arg);

                // 更换实参变量为内存变量
                callInst->setOperand(k, newVal);

                // 赋值指令插入到函数调用指令的前面
                // 函数调用指令前插入后，pIter仍指向函数调用指令
                pIter = insts.insert(pIter, assignInst);
                pIter++;
            }

            // RISCV64的函数调用约定，前四个参数通过寄存器传递
            for (int k = 0; k < argNum && k < 4; k++) {

                // 把实参的值通过move指令传递给寄存器

                auto arg = callInst->getOperand(k);

                Instruction * assignInst = new MoveInstruction(func, PlatformRiscV64::intRegVal[k], arg);

                callInst->setOperand(k, PlatformRiscV64::intRegVal[k]);

                // 函数调用指令前插入后，pIter仍指向函数调用指令
                pIter = insts.insert(pIter, assignInst);
                pIter++;
            }

#if 0
            for (int k = 0; k < callInst->getOperandsNum(); k++) {

                auto arg = callInst->getOperand(k);

                // 产生ARG指令
                pIter = insts.insert(pIter, new ArgInstruction(func, arg));
                pIter++;
            }
#endif

            // 有arg指令后可不用参数，展示不删除
            // args.clear();

            // 赋值指令
            if (callInst->hasResultValue()) {

                if (callInst->getRegId() == 0) {
                    // 结果变量的寄存器和返回值寄存器一样，则什么都不需要做
                    ;
                } else {
                    // 其它情况，需要产生赋值指令
                    // 新建一个赋值操作
                    Instruction * assignInst = new MoveInstruction(func, callInst, PlatformRiscV64::intRegVal[0]);

                    // 函数调用指令的下一个指令的前面插入指令，因为有Exit指令，+1肯定有效
                    pIter = insts.insert(pIter + 1, assignInst);
                }
            }
        }
    }
}

/// @brief 栈空间分配
/// @param func 要处理的函数
void CodeGeneratorRiscV64::stackAlloc(Function * func)
{
    // 栈内分配的空间除了寄存器保护所分配的空间之外，还需要管理如下的空间
    // (1) 没有指派寄存器的局部变量、形参或临时变量的栈内分配
    // (2) 函数调用时需要栈内传递的实参
    // (3) 函数内定义的数组变量需要在栈内分配
    // (4) 函数内定义的静态变量空间分配按静态分配处理

    // 遍历函数内的所有指令，查找没有寄存器分配的变量，然后进行栈内空间分配

    // 栈帧空间
    // --------------------- sp
    // 实参栈传递的空间（排除寄存器传递的实参空间）
    // ---------------------
    // 需要保存在栈中的局部变量或临时变量或形参对应变量空间
    // --------------------- fp
    // 保护寄存器的空间
    // ---------------------

    // 这里对临时变量和局部变量都在栈上进行分配，采用FP+偏移的寻址方式，偏移为负数

    int32_t sp_esp = 16;
    struct VarOffset {
        Value * value;
        int32_t offsetFromSp; // 以 sp 向下增长的偏移
        int32_t size;
    };

    std::vector<VarOffset> varOffsets;

    // 遍历函数变量列表
    for (auto var: func->getVarValues()) {

        // 对于简单类型的寄存器分配策略，假定临时变量和局部变量都保存在栈中，属于内存
        // 而对于图着色等，临时变量一般是寄存器，局部变量也可能修改为寄存器
        // TODO 考虑如何进行分配使得临时变量尽量保存在寄存器中，作为优化点考虑

        // regId不为-1，则说明该变量分配为寄存器
        // baseRegNo不等于-1，则说明该变量肯定在栈上，属于内存变量，之前肯定已经分配过
        if ((var->getRegId() == -1) && (!var->getMemoryAddr())) {

            // 该变量没有分配寄存器
            int32_t size = 0;
            size = var->getType()->getSize();

            // 64位RISC平台按照4字节的大小整数倍分配局部变量
            size = (size + 3) & ~3;

            // 累计当前作用域大小
            sp_esp += size;

            // 这里要注意检查变量栈的偏移范围。一般采用机制寄存器+立即数方式间接寻址
            // 若立即数满足要求，可采用基址寄存器+立即数变量的方式访问变量
            // 否则，需要先把偏移量放到寄存器中，然后机制寄存器+偏移寄存器来寻址
            // 之后需要对所有使用到该Value的指令在寄存器分配前要变换。

            // 局部变量偏移设置
            varOffsets.push_back({var, sp_esp, size});
        }
    }

    // 遍历包含有值的指令，也就是临时变量
    for (auto inst: func->getInterCode().getInsts()) {

        if (inst->hasResultValue() && (inst->getRegId() == -1)) {
            // 有值，并且没有分配寄存器

            int32_t size = inst->getType()->getSize();

            // 64位RISC平台按照4字节的大小整数倍分配局部变量
            size = (size + 3) & ~3;

            // 累计当前作用域大小
            sp_esp += size;

            // 这里要注意检查变量栈的偏移范围。一般采用机制寄存器+立即数方式间接寻址
            // 若立即数满足要求，可采用基址寄存器+立即数变量的方式访问变量
            // 否则，需要先把偏移量放到寄存器中，然后机制寄存器+偏移寄存器来寻址
            // 之后需要对所有使用到该Value的指令在寄存器分配前要变换。

            // 局部变量偏移设置
            varOffsets.push_back({inst, sp_esp, size});
        }
    }

    // 通过栈传递的实参，RISCV64的前四个通过寄存器传递
    int maxFuncCallArgCnt = func->getMaxFuncCallArgCnt();
    if (maxFuncCallArgCnt > 8) {
        sp_esp += (maxFuncCallArgCnt - 8) * 4;
    }

    // 只有int类型时可以4字节对齐，支持浮点或者向量运算时要16字节对齐
    sp_esp = (sp_esp + 15) & ~15;

    // 设置函数的最大栈帧深度，没有考虑寄存器保护的空间大小
    func->setMaxDep(sp_esp);

    // 设置所有变量的地址（相对于 FP）
    for (auto & entry: varOffsets) {
        int offsetFromFp = entry.offsetFromSp - sp_esp;

        if (auto var = dynamic_cast<LocalVariable *>(entry.value)) {
            var->setMemoryAddr(RISCV64_FP_REG_NO, offsetFromFp);
        } else if (auto inst = dynamic_cast<Instruction *>(entry.value)) {
            inst->setMemoryAddr(RISCV64_FP_REG_NO, offsetFromFp);
        } else {
            // 处理不了的类型（安全起见）
            assert(false && "Unsupported Value* type for stack allocation");
        }
        std::cout << entry.value->getName() << "\t" << offsetFromFp << std::endl;
    }
}