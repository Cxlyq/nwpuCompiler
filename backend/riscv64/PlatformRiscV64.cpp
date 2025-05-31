#include "PlatformRiscV64.h"

#include "IntegerType.h"
#include "FloatType.h"

const std::string PlatformRiscV64::regName[PlatformRiscV64::maxRegNum] = {
    "zero", // 类型：常量  作用： 常数 0，硬编码为 0 ,不需要栈保护
    "ra",   // 类型：返回地址  作用： 函数调用返回地址 ,需要栈保护，Caller
    "sp",   // 类型：栈指针  作用： 指向当前栈顶 ,需要栈保护，Callee
    "gp",   // 类型：全局指针  作用： 用于访问全局变量（非通用） ,不需要栈保护
    "tp",   // 类型：线程指针  作用： 线程局部存储 ,不需要栈保护
    "t0",   // 类型：临时  作用： 临时寄存器 ,需要栈保护，Caller
    "t1",   // 类型：临时  作用： 临时寄存器 ,需要栈保护，Caller
    "t2",   // 类型：临时  作用： 临时寄存器 ,需要栈保护，Caller
    "s0/fp", // 类型：保护/帧指针  作用： 保存寄存器 or 栈帧指针（可选） ,需要栈保护，Callee
    "s1",    // 类型：保持  作用： 保存寄存器 ,需要栈保护，Callee
    "a0",    // 类型：参数/返回值1  作用： 参数/返回值1  ,需要栈保护，Caller
    "a1",    // 类型：参数/返回值2  作用： 参数/返回值2  ,需要栈保护，Caller
    "a2",    // 类型：参数3  作用： 参数3 ,需要栈保护，Caller
    "a3",    // 类型：参数4  作用： 参数4 ,需要栈保护，Caller
    "a4",    // 类型：参数5  作用： 参数5 ,需要栈保护，Caller
    "a5",    // 类型：参数6  作用： 参数6 ,需要栈保护，Caller
    "a6",    // 类型：参数7  作用： 参数7 ,需要栈保护，Caller
    "a7",    // 类型：参数8  作用： 参数8 ,需要栈保护，Caller
    "s2",    // 类型：保存  作用： 保存寄存器 ,需要栈保护，Callee
    "s3",    // 类型：保存  作用： 保存寄存器 ,需要栈保护，Callee
    "s4",    // 类型：保存  作用： 保存寄存器 ,需要栈保护，Callee
    "s5",    // 类型：保存  作用： 保存寄存器 ,需要栈保护，Callee
    "s6",    // 类型：保存  作用： 保存寄存器 ,需要栈保护，Callee
    "s7",    // 类型：保存  作用： 保存寄存器 ,需要栈保护，Callee
    "s8",    // 类型：保存  作用： 保存寄存器 ,需要栈保护，Callee
    "s9",    // 类型：保存  作用： 保存寄存器 ,需要栈保护，Callee
    "s10",   // 类型：保存  作用： 保存寄存器 ,需要栈保护，Callee
    "s11",   // 类型：保存  作用： 保存寄存器 ,需要栈保护，Callee
    "t3",    // 类型：临时 作用： 临时寄存器 ,需要栈保护，Caller
    "t4",    // 类型：临时 作用： 临时寄存器 ,需要栈保护，Caller
    "t5",    // 类型：临时 作用： 临时寄存器 ,需要栈保护，Caller
    "t6",    // 类型：临时 作用： 临时寄存器 ,需要栈保护，Caller

    "ft0",  // 类型：临时(浮点) 作用： 临时寄存器 ,需要栈保护，Caller
    "ft1",  // 类型：临时(浮点) 作用： 临时寄存器 ,需要栈保护，Caller
    "ft2",  // 类型：临时(浮点) 作用： 临时寄存器 ,需要栈保护，Caller
    "ft3",  // 类型：临时(浮点) 作用： 临时寄存器 ,需要栈保护，Caller
    "ft4",  // 类型：临时(浮点) 作用： 临时寄存器 ,需要栈保护，Caller
    "ft5",  // 类型：临时(浮点) 作用： 临时寄存器 ,需要栈保护，Caller
    "ft6",  // 类型：临时(浮点) 作用： 临时寄存器 ,需要栈保护，Caller
    "ft7",  // 类型：临时(浮点) 作用： 临时寄存器 ,需要栈保护，Caller
    "fs0",  // 类型：保存(浮点) 作用： 被调用者保存寄存器 ,需要栈保护，Callee
    "fs1",  // 类型：保存(浮点) 作用： 被调用者保存寄存器 ,需要栈保护，Callee
    "fa0",  // 类型：参数/返回值 作用： 第1个浮点参数/返回值 ,需要栈保护，Caller
    "fa1",  // 类型：参数/返回值 作用： 第2个浮点参数/返回值 ,需要栈保护，Caller
    "fa2",  // 类型：参数 作用： 第3个浮点参数 ,需要栈保护，Caller
    "fa3",  // 类型：参数 作用： 第4个浮点参数 ,需要栈保护，Caller
    "fa4",  // 类型：参数 作用： 第5个浮点参数 ,需要栈保护，Caller
    "fa5",  // 类型：参数 作用： 第6个浮点参数 ,需要栈保护，Caller
    "fa6",  // 类型：参数 作用： 第7个浮点参数 ,需要栈保护，Caller
    "fa7",  // 类型：参数 作用： 第8个浮点参数 ,需要栈保护，Caller
    "fs2",  // 类型：保存(浮点) 作用： 被调用者保存寄存器 ,需要栈保护，Callee
    "fs3",  // 类型：保存(浮点) 作用： 被调用者保存寄存器 ,需要栈保护，Callee
    "fs4",  // 类型：保存(浮点) 作用： 被调用者保存寄存器 ,需要栈保护，Callee
    "fs5",  // 类型：保存(浮点) 作用： 被调用者保存寄存器 ,需要栈保护，Callee
    "fs6",  // 类型：保存(浮点) 作用： 被调用者保存寄存器 ,需要栈保护，Callee
    "fs7",  // 类型：保存(浮点) 作用： 被调用者保存寄存器 ,需要栈保护，Callee
    "fs8",  // 类型：保存(浮点) 作用： 被调用者保存寄存器 ,需要栈保护，Callee
    "fs9",  // 类型：保存(浮点) 作用： 被调用者保存寄存器 ,需要栈保护，Callee
    "fs10", // 类型：保存(浮点) 作用： 被调用者保存寄存器 ,需要栈保护，Callee
    "fs11", // 类型：保存(浮点) 作用： 被调用者保存寄存器 ,需要栈保护，Callee
    "ft8",  // 类型：临时(浮点) 作用： 临时寄存器 ,需要栈保护，Caller
    "ft9",  // 类型：临时(浮点) 作用： 临时寄存器 ,需要栈保护，Caller
    "ft10", // 类型：临时(浮点) 作用： 临时寄存器 ,需要栈保护，Caller
    "ft11", // 类型：临时(浮点) 作用： 临时寄存器 ,需要栈保护，Caller

};

RegVariable * PlatformRiscV64::intRegVal[PlatformRiscV64::maxRegNum / 2] = {
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[0], 0),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[1], 1),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[2], 2),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[3], 3),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[4], 4),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[5], 5),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[6], 6),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[7], 7),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[8], 8),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[9], 9),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[10], 10),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[11], 11),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[12], 12),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[13], 13),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[14], 14),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[15], 15),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[16], 16),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[17], 17),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[18], 18),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[19], 19),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[20], 20),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[21], 21),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[22], 22),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[23], 23),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[24], 24),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[25], 25),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[26], 26),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[27], 27),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[28], 28),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[29], 29),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[30], 30),
    new RegVariable(IntegerType::getTypeInt(), PlatformRiscV64::regName[31], 31)};

RegVariable * PlatformRiscV64::floatRegVal[PlatformRiscV64::maxRegNum / 2] = {
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[32], 0),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[33], 1),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[34], 2),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[35], 3),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[36], 4),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[37], 5),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[38], 6),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[39], 7),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[40], 8),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[41], 9),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[42], 10),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[43], 11),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[44], 12),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[45], 13),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[46], 14),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[47], 15),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[48], 16),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[49], 17),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[50], 18),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[51], 19),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[52], 20),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[53], 21),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[54], 22),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[55], 23),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[56], 24),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[57], 25),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[58], 26),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[59], 27),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[60], 28),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[61], 29),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[62], 30),
    new RegVariable(FloatType::getType(), PlatformRiscV64::regName[63], 31)};

/// @brief 循环左移两位
/// @param num
void PlatformRiscV64::roundLeftShiftTwoBit(unsigned int & num)
{
    // 取左移即将溢出的两位
    const unsigned int overFlow = num & 0xc0000000;

    // 将溢出部分追加到尾部
    num = (num << 2) | (overFlow >> 30);
}

/// @brief 判断num是否是常数表达式，8位数字循环右移偶数位得到
/// @param num
/// @return
bool PlatformRiscV64::__constExpr(int num)
{
    unsigned int new_num = (unsigned int) num;

    for (int i = 0; i < 16; i++) {

        if (new_num <= 0xff) {
            // 有效表达式
            return true;
        }

        // 循环左移2位
        roundLeftShiftTwoBit(new_num);
    }

    return false;
}

/// @brief 同时处理正数和负数
/// @param num
/// @return
bool PlatformRiscV64::constExpr(int num)
{
    return __constExpr(num) || __constExpr(-num);
}

/// @brief 判定是否是合法的偏移
/// @param num
/// @return
bool PlatformRiscV64::isDisp(int num)
{
    return num < 4096 && num > -4096;
}

/// @brief 判断是否是合法的寄存器名
/// @param s 寄存器名字
/// @return 是否是
bool PlatformRiscV64::isReg(std::string name)
{
    return name == "zero" || name == "ra" || name == "sp" || name == "gp" || name == "tp" || name == "t0" ||
           name == "t1" || name == "t2" || name == "s0/fp" || name == "s1" || name == "a0" || name == "a1" ||
           name == "a2" || name == "a3" || name == "a4" || name == "a5" || name == "a6" || name == "a7" ||
           name == "s2" || name == "s3" || name == "s4" || name == "s5" || name == "s6" || name == "s7" ||
           name == "s8" || name == "s9" || name == "s10" || name == "s11" || name == "t3" || name == "t4" ||
           name == "t5" || name == "t6" || name == "ft0" || name == "ft1" || name == "ft2" || name == "ft3" ||
           name == "ft4" || name == "ft5" || name == "ft6" || name == "ft7" || name == "fs0" || name == "fs1" ||
           name == "fa0" || name == "fa1" || name == "fa2" || name == "fa3" || name == "fa4" || name == "fa5" ||
           name == "fa6" || name == "fa7" || name == "fs2" || name == "fs3" || name == "fs4" || name == "fs5" ||
           name == "fs6" || name == "fs7" || name == "fs8" || name == "fs9" || name == "fs10" || name == "fs11" ||
           name == "ft8" || name == "ft9" || name == "ft10" || name == "ft11";
}
