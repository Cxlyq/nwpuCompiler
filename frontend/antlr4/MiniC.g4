grammar MiniC;

// 词法规则名总是以大写字母开头

// 语法规则名总是以小写字母开头

// 每个非终结符尽量多包含闭包、正闭包或可选符等的EBNF范式描述

// 若非终结符由多个产生式组成，则建议在每个产生式的尾部追加# 名称来区分，详细可查看非终结符statement的描述

// 语法规则描述：EBNF范式

// 源文件编译单元定义
compileUnit: (funcDef | decl)* EOF;

//-----------------------函数------------------------
// 函数定义，目前不支持形参，也不支持返回void类型等
funcDef: funcType T_ID T_L_PAREN (funcFParams)? T_R_PAREN block;
// 函数返回类型
funcType: T_INT | T_FLOAT | T_VOID;
//函数形参列表
funcFParams: funcFParam (T_COMMA funcFParam)*;
//函数形参
funcFParam:
	basicType T_ID (
		T_L_SQBRA T_R_SQBRA (T_L_SQBRA expr T_R_SQBRA)*
	)?;

//----------------------语句块---------------------------
// 语句块看用作函数体，这里允许多个语句，并且不含任何语句
block: T_L_BRACE blockItemList? T_R_BRACE;
// 每个ItemList可包含至少一个Item
blockItemList: blockItem+;
// 每个Item可以是一个语句，或者变量声明语句
blockItem: statement | decl;

//-----------------------常量与变量----------------------------
// 声明：变量或常量
decl: constDecl | varDecl;
// 基本类型
basicType: T_INT | T_FLOAT;

// 常量声明
constDecl:
	T_CONST basicType constDef (T_COMMA constDef)* T_SEMICOLON;
// 常量定义
constDef: T_ID (T_L_SQBRA expr T_R_SQBRA)* T_ASSIGN initVal;

// 变量声明，目前不支持变量含有初值
varDecl: basicType varDef (T_COMMA varDef)* T_SEMICOLON;
// 变量定义
varDef: T_ID (T_L_SQBRA expr T_R_SQBRA)* (T_ASSIGN initVal)?;
// 右值（数组{}赋值或单表达式）
initVal:
	expr												# singleVal
	| T_L_BRACE (initVal (T_COMMA initVal)*)? T_R_BRACE	# multiVal;

//-------------------基本语句与表达式---------------------------
// 基本语句
statement:
	T_RETURN expr? T_SEMICOLON										# returnStatement
	| lVal T_ASSIGN expr T_SEMICOLON								# assignStatement
	| block															# blockStatement
	| expr? T_SEMICOLON												# expressionStatement
	| T_IF T_L_PAREN cond T_R_PAREN statement (T_ELSE statement)?	# ifelseStatement
	| T_WHILE T_L_PAREN cond T_R_PAREN statement					# whileStatement
	| T_BREAK T_SEMICOLON											# breakStatement
	| T_CONTINUE T_SEMICOLON										# continueStatement;
// 表达式：多项算术表达式
expr: addExp; // ?表达式是否支持逻辑01 expr: cond;
// 条件表达式：多项逻辑表达式
cond: lOrExp;
// 多项逻辑表达式（或表达式）
lOrExp: lAndExp (T_OR lAndExp)*;
// 单项逻辑表达式（与表达式）（多项相等性判断）
lAndExp: eqExp (T_AND eqExp)*;
// 相等性表达式（多项关系）
eqExp: relExp (eqOp relExp)*;
// 相等性判断运算符
eqOp: T_EQUAL | T_NEQUAL;
// 关系表达式
relExp: addExp (relOp addExp)*;
// 关系运算符
relOp: T_GE | T_GREATER | T_LE | T_LESS;
// 多项算术表达式
addExp: mulExp (addOp mulExp)*;
// 项间运算符
addOp: T_ADD | T_SUB;
// 单项算术表达式
mulExp: unaryExp (mulOp unaryExp)*;
// 项内运算符
mulOp: T_MUL | T_DIV | T_MOD;
// 一元表达式
unaryExp: (unaryOp)* primaryExp;
// 单目运算符
unaryOp: T_ADD | T_SUB | T_NOT;
// 实参列表
funcRParams: expr (T_COMMA expr)*;
// 基本表达式：括号表达式、左值表达式、数值
primaryExp:
	T_L_PAREN expr T_R_PAREN				# parenExpr
	| lVal									# leftValue
	| number								# basicNum
	| T_ID T_L_PAREN funcRParams? T_R_PAREN	# funcCall;
// 左值表达式
lVal: T_ID (T_L_SQBRA expr T_R_SQBRA)*;
// 数值：整数/浮点数
number: T_INT_DIGIT | T_FLOAT_DIGIT;

// 用正规式来进行词法规则的描述

T_SEMICOLON: ';';
T_L_BRACE: '{';
T_R_BRACE: '}';

T_L_PAREN: '(';
T_R_PAREN: ')';
T_L_SQBRA: '[';
T_R_SQBRA: ']';

T_NOT: '!';

T_MUL: '*';
T_DIV: '/';
T_MOD: '%';

T_ADD: '+';
T_SUB: '-';

T_GE: '>=';
T_LE: '<=';
T_GREATER: '>';
T_LESS: '<';

T_EQUAL: '==';
T_NEQUAL: '!=';

T_ASSIGN: '=';
T_COMMA: ',';

T_AND: '&&';
T_OR: '||';
// 要注意关键字同样也属于T_ID，因此必须放在T_ID的前面，否则会识别成T_ID
T_RETURN: 'return';
T_INT: 'int';
T_FLOAT: 'float';
T_VOID: 'void';
T_IF: 'if';
T_ELSE: 'else';
T_CONST: 'const';
T_WHILE: 'while';
T_BREAK: 'break';
T_CONTINUE: 'continue';

T_ID: [a-zA-Z_][a-zA-Z0-9_]*;
T_INT_DIGIT:
	'0' [xX][0-9a-fA-F]+
	| '0' [0-7]*
	| [1-9][0-9]*
	| '0';
T_FLOAT_DIGIT: ([0-9]+ '.' [0-9]* | '.' [0-9]+ | [0-9]+ '.') (
		([eE][+-]? [0-9]+)?
	);

/* 空白符丢弃 */
WS: [ \r\n\t]+ -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;
BLOCK_COMMENT: '/*' .*? '*/' -> skip;