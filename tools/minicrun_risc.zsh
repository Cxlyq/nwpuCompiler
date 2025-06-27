APPFIX=""
WINOS=0

# OS类别
OSKind=$(uname -s)

# 机器位数
OS3264=$(uname -m)

OK_NUM=0
NG_NUM=0
# # 单个测试用例测试
# function dotestcase()
# {
#     #CFILELIST=$(ls function/$1*.c)
# 	CFILELIST=$(ls tests/official/2023_function/2023_func_${1}_*.c 2>/dev/null)

#     # 要查找的C文件名
#     CFILE=""

#     n=0
#     for cfilename in $CFILELIST
#     do
#         CFILE=$cfilename
#         n=$(expr $n + 1)
#     done

#     if [ $n -ne 1 ]; then
#         return
#     fi

#     # 去掉C文件后缀，保留文件名
#     TESTNAME=${CFILE%%.*}

#     INFILE="${TESTNAME}.in"
#     OUTFILE="${TESTNAME}.out"
#     IRFILE="${TESTNAME}.ir"
#     RESULTFILE="${TESTNAME}.result"

#     # 实验命令格式调整，如有错误请按实验要求修改
#     CMDNAME="./build/minic${APPFIX} -S -I -o ${IRFILE} ${CFILE}"

#     OUTSTR=$(eval ${CMDNAME} 2>&1)
#     echo "${OUTSTR}" > ${RESULTFILE}

#     if [ ! -f ${IRFILE} ]; then
#         echo "${IRFILE} not generated"
#         return
#     fi

#     # Linux发布者
#     if [[ $OSKind == "Linux" ]]; then
#         LinuxDisID=$(lsb_release -i -s)
#         LinuxRelease=$(lsb_release -r -s)
#         CMDNAME="./IRCompiler/${OSKind}-${OS3264}/${LinuxDisID}-${LinuxRelease}/IRCompiler${APPFIX} -R ${IRFILE}"
#     else
#         CMDNAME="./IRCompiler/${OSKind}-${OS3264}/IRCompiler${APPFIX} -R ${IRFILE}"
#     fi

#     if [ -f ${INFILE} ]; then
#         CMDNAME="${CMDNAME} < ${INFILE}"
#     fi

#     OUTSTR=$(eval ${CMDNAME} 2>&1)
#     RUNRESULT=$?

#     if [ -n "${OUTSTR}" ]; then
#         OUTSTR="${OUTSTR}\n"
#     fi

#     if [ ${WINOS} -eq 1 ]; then
#         echo -e "${OUTSTR}${RUNRESULT}" > ${RESULTFILE}
#     elif [[ $OSKind == "Darwin" ]]; then
#         echo "${OUTSTR}${RUNRESULT}" > ${RESULTFILE}
#     else
#         echo -e "${OUTSTR}${RUNRESULT}" > ${RESULTFILE}
#     fi

#     diff -a --strip-trailing-cr "${RESULTFILE}" "${OUTFILE}" > /dev/null 2>&1

#     if [ $? -ne 0 ]; then
#         echo "${CFILE} NG"
#         NG_NUM=$(expr ${NG_NUM} + 1)
#     else
#         OK_NUM=$(expr ${OK_NUM} + 1)
#         echo "${CFILE} OK"
#         rm -f ${RESULTFILE}
#         rm -f ${IRFILE}
#     fi
# }
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

    # 3. 使用 QEMU 运行 RISC-V ELF 可执行文件
    if [ -f ${INFILE} ]; then
        OUTSTR=$(qemu-riscv64 ${EXEFILE} < ${INFILE} 2>&1)
    else
        OUTSTR=$(qemu-riscv64 ${EXEFILE} 2>&1)
    fi
    RUNRESULT=$?

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
    fi
}



TestCaseNum_From=0
TestCaseNum_To=160

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



for ((i = $TestCaseNum_From; i <= $TestCaseNum_To; i ++))
do
    TestCaseNo=$(printf '%02d' $i)
    dotestcase $TestCaseNo
done

echo "OK number=${OK_NUM}, NG number=${NG_NUM}"

if [ ${NG_NUM} -ne 0 ]; then
    echo "Failed test cases:"
    for f in "${NG_LIST[@]}"; do
        echo "  $f"
    done
fi