APPFIX=""
WINOS=0

# OS类别
OSKind=$(uname -s)

# 机器位数
OS3264=$(uname -m)

OK_NUM=0
NG_NUM=0
TIMEOUT_NUM=0  # 新增：记录超时（死循环）的个数
function dotestcase()
{
    CFILELIST=$(ls tests/official/2023_function/2023_func_${1}_*.c 2>/dev/null)

    CFILE=""
    n=0
    for cfilename in $CFILELIST; do
        CFILE=$cfilename
        n=$(expr $n + 1)
    done

    if [ $n -ne 1 ]; then
        return
    fi

    TESTNAME=${CFILE%%.*}

    INFILE="${TESTNAME}.in"
    OUTFILE="${TESTNAME}.out"
    SFILE="${TESTNAME}.s"
    RESULTFILE="${TESTNAME}.result"
    EXEFILE="${TESTNAME}.riscv.elf"
    STD_C_PATH="./tests/std.c"     # 请修改为你真实路径

    # 1. 使用 minic 生成 RISC-V 汇编
    CMDNAME="./build/minic${APPFIX} -S -o ${SFILE} ${CFILE}"
    OUTSTR=$(eval ${CMDNAME} 2>&1)
    echo "${OUTSTR}" > ${RESULTFILE}

    if [ ! -f ${SFILE} ]; then
        echo "${SFILE} not generated"
        return
    fi

    # 2. 用 riscv64-unknown-elf-gcc 编译汇编文件和 std.c
	riscv64-linux-gnu-gcc -march=rv64imafdc -mabi=lp64d -static -O2 -o ${EXEFILE} ${SFILE} ${STD_C_PATH}

    if [ $? -ne 0 ]; then
        echo "RISC-V gcc compile failed"
        return
    fi

    # 3. 使用 QEMU 运行 RISC-V ELF 可执行文件（带超时控制）
    TIMEOUT_SECONDS=15  # 设置5秒超时
    if [ -f ${INFILE} ]; then
        # 添加timeout命令，检测超时
        OUTSTR=$(timeout $TIMEOUT_SECONDS qemu-riscv64-static ${EXEFILE} < ${INFILE} 2>&1)
        RUNRESULT=$?
    else
        OUTSTR=$(timeout $TIMEOUT_SECONDS qemu-riscv64-static ${EXEFILE} 2>&1)
        RUNRESULT=$?
    fi

    # 检测是否超时（timeout命令返回124表示超时）
    if [ $RUNRESULT -eq 124 ]; then
        echo "${CFILE} TIMEOUT (possible infinite loop)"
        TIMEOUT_NUM=$(expr ${TIMEOUT_NUM} + 1)
        TIMEOUT_LIST+=("${CFILE}")  # 记录超时测试用例
        return
    fi

    # 4. 写入输出结果
    if [ -n "${OUTSTR}" ]; then
        OUTSTR="${OUTSTR}\n"
    fi

    echo -e "${OUTSTR}${RUNRESULT}" > ${RESULTFILE}

    # 5. 对比输出
    diff -a --strip-trailing-cr "${RESULTFILE}" "${OUTFILE}" > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo "${CFILE} NG"
        NG_NUM=$(expr ${NG_NUM} + 1)
        NG_LIST+=("${CFILE}")
    else
        OK_NUM=$(expr ${OK_NUM} + 1)
        echo "${CFILE} OK"
        rm -f ${RESULTFILE} ${SFILE} ${EXEFILE}
		OK_LIST+=("${CFILE}")
    fi
}

TestCaseNum_From=0
TestCaseNum_To=144

if [ $# -eq 1 ]; then
    TestCaseNum_From=$1
    TestCaseNum_To=$1
fi

if [ $# -ge 2 ]; then
    TestCaseNum_From=$1
    TestCaseNum_To=$2
fi

PlatformOS=$(uname -a)

if [[ ${PlatformOS} =~ "MINGW" ]] || [[ ${PlatformOS} =~ "MSYS" ]]; then
    APPFIX=".exe"
    OSKind="MINGW"
    WINOS=1
fi

# 初始化数组
OK_LIST=()
NG_LIST=()
TIMEOUT_LIST=()  # 新增：存储超时测试用例

for ((i = $TestCaseNum_From; i <= $TestCaseNum_To; i ++))
do
    TestCaseNo=$(printf '%02d' $i)
    dotestcase $TestCaseNo
done

echo "OK number=${OK_NUM}, NG number=${NG_NUM}, TIMEOUT number=${TIMEOUT_NUM}"

if [ ${OK_NUM} -ne 0 ]; then
    echo "OK test cases:"
    for f in "${OK_LIST[@]}"; do
        echo "  $f"
    done
fi

if [ ${NG_NUM} -ne 0 ]; then
    echo "NG test cases:"
    for f in "${NG_LIST[@]}"; do
        echo "  $f"
    done
fi

# 新增：输出超时（死循环）测试用例
if [ ${TIMEOUT_NUM} -ne 0 ]; then
    echo "TIMEOUT (infinite loop) test cases:"
    for f in "${TIMEOUT_LIST[@]}"; do
        echo "  $f"
    done
fi