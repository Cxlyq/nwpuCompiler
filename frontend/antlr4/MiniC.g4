grammar MiniC;

// 词法规则名总是以大写字母开头

// 语法规则名总是以小写字母开头

// 每个非终结符尽量多包含闭包、正闭包或可选符等的EBNF范式描述

// 若非终结符由多个产生式组成，则建议在每个产生式的尾部追加# 名称来区分，详细可查看非终结符statement的描述

// 语法规则描述：EBNF范式

// 源文件编译单元定义
compileUnit: (funcDef | decl)* EOF;

// 声明：变量或常量
decl: constDecl | varDecl;

// 常量声明
constDecl:
	T_CONST basicType constDef (T_COMMA constDef)* T_SEMICOLON;

constDef:
	T_ID (T_L_SQBRA constExp T_R_SQBRA)* T_EQUAL constInitVal;
constInitVal:
	constExp
	| T_L_BRACE (constInitVal (T_COMMA constInitVal)*)?;

// 函数定义，目前不支持形参，也不支持返回void类型等
funcDef: funcType T_ID T_L_PAREN (funcFParams)? T_R_PAREN block;

funcType: T_INT | T_FLOAT | T_VOID;

funcFParams: funcFParam (T_COMMA funcFParam)*;
funcFParam:
	basicType T_ID (
		T_L_SQBRA T_R_SQBRA (T_L_SQBRA expr T_R_SQBRA)*
	)?;

// 语句块看用作函数体，这里允许多个语句，并且不含任何语句
block: T_L_BRACE blockItemList? T_R_BRACE;

// 每个ItemList可包含至少一个Item
blockItemList: blockItem+;

// 每个Item可以是一个语句，或者变量声明语句
blockItem: statement | decl;

// 变量声明，目前不支持变量含有初值
varDecl: basicType varDef (T_COMMA varDef)* T_SEMICOLON;

// 基本类型
basicType: T_INT | T_FLOAT;

// 变量定义
varDef: T_ID (T_L_SQBRA constExp T_R_SQBRA)* (T_EQUAL initVal)?;

initVal:
	expr
	| T_L_BRACE (initVal (T_COMMA initVal)*)? T_R_BRACE;

// 目前语句支持return和赋值语句
statement:
	T_RETURN expr? T_SEMICOLON			# returnStatement
	| lVal T_ASSIGN expr T_SEMICOLON	# assignStatement
	| block								# blockStatement
	| expr? T_SEMICOLON					# expressionStatement
	| T_IF T_L_PAREN cond T_R_PAREN statement (T_ELSE statement)?
	| T_WHILE T_L_PAREN cond T_R_PAREN statement
	| T_BREAK T_SEMICOLON
	| T_CONTINUE T_SEMICOLON;

// 表达式文法 expr : AddExp 表达式目前只支持加法与减法运算
expr: addExp;
cond: lOrExp;
// 基本表达式：括号表达式、整数、左值表达式
primaryExp: T_L_PAREN expr T_R_PAREN | lVal | number;

number: T_DIGIT | T_FLOAT_LITERAL;

// 加减表达式

// 一元表达式
unaryExp:
	primaryExp
	| T_ID T_L_PAREN funcRParams? T_R_PAREN
	| unaryOp unaryExp;
unaryOp: T_ADD | T_SUB | T_NOT;
// 基本表达式：括号表达式、整数、左值表达式

// 实参列表
funcRParams: expr (T_COMMA expr)*;

mulExp: unaryExp | mulExp mulOp unaryExp;
mulOp: T_MUL | T_DIV | T_MOD;
addExp: mulExp | addExp addOp mulExp;
addOp: T_ADD | T_SUB;
relExp: addExp | relExp relOp addExp;
relOp: T_GE | T_GREATER | T_LE | T_LESS;
eqExp: relExp | eqExp eqOp relExp;
eqOp: T_EQUAL | T_NEQUAL;
lAndExp: eqExp | lAndExp T_AND eqExp;
lOrExp: lAndExp | lOrExp T_OR lAndExp;
constExp: addExp;

// 左值表达式
lVal: T_ID (T_L_SQBRA expr T_R_SQBRA)*;

// 用正规式来进行词法规则的描述

T_L_PAREN: '(';
T_R_PAREN: ')';
T_L_SQBRA: '[';
T_R_SQBRA: ']';

T_SEMICOLON: ';';
T_L_BRACE: '{';
T_R_BRACE: '}';

T_NOT: '!';

T_MUL: '*';
T_DIV: '/';
T_MOD: '%';

T_ADD: '+';
T_SUB: '-';

T_GE: '>=';
T_GREATER: '>';
T_LE: '<=';
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
T_DIGIT:
	'0' [xX][0-9a-fA-F]+
	| '0' [0-7]*
	| [1-9][0-9]*
	| '0';
T_FLOAT_LITERAL: ([0-9]+ '.' [0-9]* | '.' [0-9]+ | [0-9]+ '.') (
		([eE][+-]? [0-9]+)?
	);

/* 空白符丢弃 */
WS: [ \r\n\t]+ -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;
BLOCK_COMMENT: '/*' .*? '*/' -> skip;