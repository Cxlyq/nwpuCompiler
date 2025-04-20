
// Generated from /home/cx/文档/codes/nwpuCompiler/frontend/antlr4/MiniC.g4 by ANTLR 4.12.0
// Generated from /home/code/exp04-minic-expr/frontend/antlr4/MiniC.g4 by ANTLR 4.12.0


#include "MiniCVisitor.h"

#include "MiniCParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct MiniCParserStaticData final {
  MiniCParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  MiniCParserStaticData(const MiniCParserStaticData&) = delete;
  MiniCParserStaticData(MiniCParserStaticData&&) = delete;
  MiniCParserStaticData& operator=(const MiniCParserStaticData&) = delete;
  MiniCParserStaticData& operator=(MiniCParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag minicParserOnceFlag;
MiniCParserStaticData *minicParserStaticData = nullptr;

void minicParserInitialize() {
  assert(minicParserStaticData == nullptr);
  auto staticData = std::make_unique<MiniCParserStaticData>(
    std::vector<std::string>{
      "compileUnit", "funcDef", "funcType", "funcFParams", "funcFParam", 
      "block", "blockItemList", "blockItem", "decl", "basicType", "constDecl", 
      "constDef", "constInitVal", "varDecl", "varDef", "initVal", "statement", 
      "expr", "cond", "lVal", "primaryExp", "number", "unaryExp", "unaryOp", 
      "funcRParams", "mulExp", "mulOp", "addExp", "addOp", "relExp", "relOp", 
      "eqExp", "eqOp", "lAndExp", "lOrExp", "constExp"
    },
    std::vector<std::string>{
      "", "';'", "'{'", "'}'", "'('", "')'", "'['", "']'", "'!'", "'*'", 
      "'/'", "'%'", "'+'", "'-'", "'>='", "'<='", "'>'", "'<'", "'=='", 
      "'!='", "'='", "','", "'&&'", "'||'", "'return'", "'int'", "'float'", 
      "'void'", "'if'", "'else'", "'const'", "'while'", "'break'", "'continue'"
    },
    std::vector<std::string>{
      "", "T_SEMICOLON", "T_L_BRACE", "T_R_BRACE", "T_L_PAREN", "T_R_PAREN", 
      "T_L_SQBRA", "T_R_SQBRA", "T_NOT", "T_MUL", "T_DIV", "T_MOD", "T_ADD", 
      "T_SUB", "T_GE", "T_LE", "T_GREATER", "T_LESS", "T_EQUAL", "T_NEQUAL", 
      "T_ASSIGN", "T_COMMA", "T_AND", "T_OR", "T_RETURN", "T_INT", "T_FLOAT", 
      "T_VOID", "T_IF", "T_ELSE", "T_CONST", "T_WHILE", "T_BREAK", "T_CONTINUE", 
      "T_ID", "T_DIGIT", "T_FLOAT_LITERAL", "WS", "LINE_COMMENT", "BLOCK_COMMENT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,39,345,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,1,0,1,0,5,0,75,8,0,10,0,12,0,78,9,0,1,0,1,0,1,1,1,1,1,1,1,1,3,1,86,
  	8,1,1,1,1,1,1,1,1,2,1,2,1,2,3,2,94,8,2,1,3,1,3,1,3,5,3,99,8,3,10,3,12,
  	3,102,9,3,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,5,4,112,8,4,10,4,12,4,115,9,
  	4,3,4,117,8,4,1,5,1,5,3,5,121,8,5,1,5,1,5,1,6,4,6,126,8,6,11,6,12,6,127,
  	1,7,1,7,3,7,132,8,7,1,8,1,8,3,8,136,8,8,1,9,1,9,1,10,1,10,1,10,1,10,1,
  	10,5,10,145,8,10,10,10,12,10,148,9,10,1,10,1,10,1,11,1,11,1,11,1,11,1,
  	11,5,11,157,8,11,10,11,12,11,160,9,11,1,11,1,11,1,11,1,12,1,12,1,12,1,
  	12,1,12,5,12,170,8,12,10,12,12,12,173,9,12,3,12,175,8,12,3,12,177,8,12,
  	1,13,1,13,1,13,1,13,5,13,183,8,13,10,13,12,13,186,9,13,1,13,1,13,1,14,
  	1,14,1,14,1,14,1,14,5,14,195,8,14,10,14,12,14,198,9,14,1,14,1,14,3,14,
  	202,8,14,1,15,1,15,1,15,1,15,1,15,5,15,209,8,15,10,15,12,15,212,9,15,
  	3,15,214,8,15,1,15,3,15,217,8,15,1,16,1,16,3,16,221,8,16,1,16,1,16,1,
  	16,1,16,1,16,1,16,1,16,1,16,3,16,231,8,16,1,16,1,16,1,16,1,16,1,16,1,
  	16,1,16,1,16,3,16,241,8,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,
  	16,1,16,3,16,253,8,16,1,17,1,17,1,18,1,18,1,19,1,19,1,19,1,19,1,19,5,
  	19,264,8,19,10,19,12,19,267,9,19,1,20,1,20,1,20,1,20,1,20,1,20,3,20,275,
  	8,20,1,21,1,21,1,22,1,22,1,22,1,22,3,22,283,8,22,1,22,1,22,1,22,1,22,
  	3,22,289,8,22,1,23,1,23,1,24,1,24,1,24,5,24,296,8,24,10,24,12,24,299,
  	9,24,1,25,1,25,1,25,1,25,3,25,305,8,25,1,26,1,26,1,27,1,27,1,27,1,27,
  	3,27,313,8,27,1,28,1,28,1,29,1,29,1,29,1,29,3,29,321,8,29,1,30,1,30,1,
  	31,1,31,1,31,1,31,3,31,329,8,31,1,32,1,32,1,33,1,33,1,33,3,33,336,8,33,
  	1,34,1,34,1,34,3,34,341,8,34,1,35,1,35,1,35,0,0,36,0,2,4,6,8,10,12,14,
  	16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,
  	62,64,66,68,70,0,7,1,0,25,26,1,0,35,36,2,0,8,8,12,13,1,0,9,11,1,0,12,
  	13,1,0,14,17,1,0,18,19,354,0,76,1,0,0,0,2,81,1,0,0,0,4,93,1,0,0,0,6,95,
  	1,0,0,0,8,103,1,0,0,0,10,118,1,0,0,0,12,125,1,0,0,0,14,131,1,0,0,0,16,
  	135,1,0,0,0,18,137,1,0,0,0,20,139,1,0,0,0,22,151,1,0,0,0,24,176,1,0,0,
  	0,26,178,1,0,0,0,28,189,1,0,0,0,30,216,1,0,0,0,32,252,1,0,0,0,34,254,
  	1,0,0,0,36,256,1,0,0,0,38,258,1,0,0,0,40,274,1,0,0,0,42,276,1,0,0,0,44,
  	288,1,0,0,0,46,290,1,0,0,0,48,292,1,0,0,0,50,300,1,0,0,0,52,306,1,0,0,
  	0,54,308,1,0,0,0,56,314,1,0,0,0,58,316,1,0,0,0,60,322,1,0,0,0,62,324,
  	1,0,0,0,64,330,1,0,0,0,66,332,1,0,0,0,68,337,1,0,0,0,70,342,1,0,0,0,72,
  	75,3,2,1,0,73,75,3,16,8,0,74,72,1,0,0,0,74,73,1,0,0,0,75,78,1,0,0,0,76,
  	74,1,0,0,0,76,77,1,0,0,0,77,79,1,0,0,0,78,76,1,0,0,0,79,80,5,0,0,1,80,
  	1,1,0,0,0,81,82,3,4,2,0,82,83,5,34,0,0,83,85,5,4,0,0,84,86,3,6,3,0,85,
  	84,1,0,0,0,85,86,1,0,0,0,86,87,1,0,0,0,87,88,5,5,0,0,88,89,3,10,5,0,89,
  	3,1,0,0,0,90,94,5,25,0,0,91,94,5,26,0,0,92,94,5,27,0,0,93,90,1,0,0,0,
  	93,91,1,0,0,0,93,92,1,0,0,0,94,5,1,0,0,0,95,100,3,8,4,0,96,97,5,21,0,
  	0,97,99,3,8,4,0,98,96,1,0,0,0,99,102,1,0,0,0,100,98,1,0,0,0,100,101,1,
  	0,0,0,101,7,1,0,0,0,102,100,1,0,0,0,103,104,3,18,9,0,104,116,5,34,0,0,
  	105,106,5,6,0,0,106,113,5,7,0,0,107,108,5,6,0,0,108,109,3,34,17,0,109,
  	110,5,7,0,0,110,112,1,0,0,0,111,107,1,0,0,0,112,115,1,0,0,0,113,111,1,
  	0,0,0,113,114,1,0,0,0,114,117,1,0,0,0,115,113,1,0,0,0,116,105,1,0,0,0,
  	116,117,1,0,0,0,117,9,1,0,0,0,118,120,5,2,0,0,119,121,3,12,6,0,120,119,
  	1,0,0,0,120,121,1,0,0,0,121,122,1,0,0,0,122,123,5,3,0,0,123,11,1,0,0,
  	0,124,126,3,14,7,0,125,124,1,0,0,0,126,127,1,0,0,0,127,125,1,0,0,0,127,
  	128,1,0,0,0,128,13,1,0,0,0,129,132,3,32,16,0,130,132,3,16,8,0,131,129,
  	1,0,0,0,131,130,1,0,0,0,132,15,1,0,0,0,133,136,3,20,10,0,134,136,3,26,
  	13,0,135,133,1,0,0,0,135,134,1,0,0,0,136,17,1,0,0,0,137,138,7,0,0,0,138,
  	19,1,0,0,0,139,140,5,30,0,0,140,141,3,18,9,0,141,146,3,22,11,0,142,143,
  	5,21,0,0,143,145,3,22,11,0,144,142,1,0,0,0,145,148,1,0,0,0,146,144,1,
  	0,0,0,146,147,1,0,0,0,147,149,1,0,0,0,148,146,1,0,0,0,149,150,5,1,0,0,
  	150,21,1,0,0,0,151,158,5,34,0,0,152,153,5,6,0,0,153,154,3,70,35,0,154,
  	155,5,7,0,0,155,157,1,0,0,0,156,152,1,0,0,0,157,160,1,0,0,0,158,156,1,
  	0,0,0,158,159,1,0,0,0,159,161,1,0,0,0,160,158,1,0,0,0,161,162,5,18,0,
  	0,162,163,3,24,12,0,163,23,1,0,0,0,164,177,3,70,35,0,165,174,5,2,0,0,
  	166,171,3,24,12,0,167,168,5,21,0,0,168,170,3,24,12,0,169,167,1,0,0,0,
  	170,173,1,0,0,0,171,169,1,0,0,0,171,172,1,0,0,0,172,175,1,0,0,0,173,171,
  	1,0,0,0,174,166,1,0,0,0,174,175,1,0,0,0,175,177,1,0,0,0,176,164,1,0,0,
  	0,176,165,1,0,0,0,177,25,1,0,0,0,178,179,3,18,9,0,179,184,3,28,14,0,180,
  	181,5,21,0,0,181,183,3,28,14,0,182,180,1,0,0,0,183,186,1,0,0,0,184,182,
  	1,0,0,0,184,185,1,0,0,0,185,187,1,0,0,0,186,184,1,0,0,0,187,188,5,1,0,
  	0,188,27,1,0,0,0,189,196,5,34,0,0,190,191,5,6,0,0,191,192,3,70,35,0,192,
  	193,5,7,0,0,193,195,1,0,0,0,194,190,1,0,0,0,195,198,1,0,0,0,196,194,1,
  	0,0,0,196,197,1,0,0,0,197,201,1,0,0,0,198,196,1,0,0,0,199,200,5,18,0,
  	0,200,202,3,30,15,0,201,199,1,0,0,0,201,202,1,0,0,0,202,29,1,0,0,0,203,
  	217,3,34,17,0,204,213,5,2,0,0,205,210,3,30,15,0,206,207,5,21,0,0,207,
  	209,3,30,15,0,208,206,1,0,0,0,209,212,1,0,0,0,210,208,1,0,0,0,210,211,
  	1,0,0,0,211,214,1,0,0,0,212,210,1,0,0,0,213,205,1,0,0,0,213,214,1,0,0,
  	0,214,215,1,0,0,0,215,217,5,3,0,0,216,203,1,0,0,0,216,204,1,0,0,0,217,
  	31,1,0,0,0,218,220,5,24,0,0,219,221,3,34,17,0,220,219,1,0,0,0,220,221,
  	1,0,0,0,221,222,1,0,0,0,222,253,5,1,0,0,223,224,3,38,19,0,224,225,5,20,
  	0,0,225,226,3,34,17,0,226,227,5,1,0,0,227,253,1,0,0,0,228,253,3,10,5,
  	0,229,231,3,34,17,0,230,229,1,0,0,0,230,231,1,0,0,0,231,232,1,0,0,0,232,
  	253,5,1,0,0,233,234,5,28,0,0,234,235,5,4,0,0,235,236,3,36,18,0,236,237,
  	5,5,0,0,237,240,3,32,16,0,238,239,5,29,0,0,239,241,3,32,16,0,240,238,
  	1,0,0,0,240,241,1,0,0,0,241,253,1,0,0,0,242,243,5,31,0,0,243,244,5,4,
  	0,0,244,245,3,36,18,0,245,246,5,5,0,0,246,247,3,32,16,0,247,253,1,0,0,
  	0,248,249,5,32,0,0,249,253,5,1,0,0,250,251,5,33,0,0,251,253,5,1,0,0,252,
  	218,1,0,0,0,252,223,1,0,0,0,252,228,1,0,0,0,252,230,1,0,0,0,252,233,1,
  	0,0,0,252,242,1,0,0,0,252,248,1,0,0,0,252,250,1,0,0,0,253,33,1,0,0,0,
  	254,255,3,36,18,0,255,35,1,0,0,0,256,257,3,68,34,0,257,37,1,0,0,0,258,
  	265,5,34,0,0,259,260,5,6,0,0,260,261,3,34,17,0,261,262,5,7,0,0,262,264,
  	1,0,0,0,263,259,1,0,0,0,264,267,1,0,0,0,265,263,1,0,0,0,265,266,1,0,0,
  	0,266,39,1,0,0,0,267,265,1,0,0,0,268,269,5,4,0,0,269,270,3,34,17,0,270,
  	271,5,5,0,0,271,275,1,0,0,0,272,275,3,38,19,0,273,275,3,42,21,0,274,268,
  	1,0,0,0,274,272,1,0,0,0,274,273,1,0,0,0,275,41,1,0,0,0,276,277,7,1,0,
  	0,277,43,1,0,0,0,278,289,3,40,20,0,279,280,5,34,0,0,280,282,5,4,0,0,281,
  	283,3,48,24,0,282,281,1,0,0,0,282,283,1,0,0,0,283,284,1,0,0,0,284,289,
  	5,5,0,0,285,286,3,46,23,0,286,287,3,44,22,0,287,289,1,0,0,0,288,278,1,
  	0,0,0,288,279,1,0,0,0,288,285,1,0,0,0,289,45,1,0,0,0,290,291,7,2,0,0,
  	291,47,1,0,0,0,292,297,3,34,17,0,293,294,5,21,0,0,294,296,3,34,17,0,295,
  	293,1,0,0,0,296,299,1,0,0,0,297,295,1,0,0,0,297,298,1,0,0,0,298,49,1,
  	0,0,0,299,297,1,0,0,0,300,304,3,44,22,0,301,302,3,52,26,0,302,303,3,50,
  	25,0,303,305,1,0,0,0,304,301,1,0,0,0,304,305,1,0,0,0,305,51,1,0,0,0,306,
  	307,7,3,0,0,307,53,1,0,0,0,308,312,3,50,25,0,309,310,3,56,28,0,310,311,
  	3,54,27,0,311,313,1,0,0,0,312,309,1,0,0,0,312,313,1,0,0,0,313,55,1,0,
  	0,0,314,315,7,4,0,0,315,57,1,0,0,0,316,320,3,54,27,0,317,318,3,60,30,
  	0,318,319,3,58,29,0,319,321,1,0,0,0,320,317,1,0,0,0,320,321,1,0,0,0,321,
  	59,1,0,0,0,322,323,7,5,0,0,323,61,1,0,0,0,324,328,3,58,29,0,325,326,3,
  	64,32,0,326,327,3,62,31,0,327,329,1,0,0,0,328,325,1,0,0,0,328,329,1,0,
  	0,0,329,63,1,0,0,0,330,331,7,6,0,0,331,65,1,0,0,0,332,335,3,62,31,0,333,
  	334,5,22,0,0,334,336,3,66,33,0,335,333,1,0,0,0,335,336,1,0,0,0,336,67,
  	1,0,0,0,337,340,3,66,33,0,338,339,5,23,0,0,339,341,3,68,34,0,340,338,
  	1,0,0,0,340,341,1,0,0,0,341,69,1,0,0,0,342,343,3,54,27,0,343,71,1,0,0,
  	0,37,74,76,85,93,100,113,116,120,127,131,135,146,158,171,174,176,184,
  	196,201,210,213,216,220,230,240,252,265,274,282,288,297,304,312,320,328,
  	335,340
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  minicParserStaticData = staticData.release();
}

}

MiniCParser::MiniCParser(TokenStream *input) : MiniCParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

MiniCParser::MiniCParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  MiniCParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *minicParserStaticData->atn, minicParserStaticData->decisionToDFA, minicParserStaticData->sharedContextCache, options);
}

MiniCParser::~MiniCParser() {
  delete _interpreter;
}

const atn::ATN& MiniCParser::getATN() const {
  return *minicParserStaticData->atn;
}

std::string MiniCParser::getGrammarFileName() const {
  return "MiniC.g4";
}

const std::vector<std::string>& MiniCParser::getRuleNames() const {
  return minicParserStaticData->ruleNames;
}

const dfa::Vocabulary& MiniCParser::getVocabulary() const {
  return minicParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView MiniCParser::getSerializedATN() const {
  return minicParserStaticData->serializedATN;
}


//----------------- CompileUnitContext ------------------------------------------------------------------

MiniCParser::CompileUnitContext::CompileUnitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::CompileUnitContext::EOF() {
  return getToken(MiniCParser::EOF, 0);
}

std::vector<MiniCParser::FuncDefContext *> MiniCParser::CompileUnitContext::funcDef() {
  return getRuleContexts<MiniCParser::FuncDefContext>();
}

MiniCParser::FuncDefContext* MiniCParser::CompileUnitContext::funcDef(size_t i) {
  return getRuleContext<MiniCParser::FuncDefContext>(i);
}

std::vector<MiniCParser::DeclContext *> MiniCParser::CompileUnitContext::decl() {
  return getRuleContexts<MiniCParser::DeclContext>();
}

MiniCParser::DeclContext* MiniCParser::CompileUnitContext::decl(size_t i) {
  return getRuleContext<MiniCParser::DeclContext>(i);
}


size_t MiniCParser::CompileUnitContext::getRuleIndex() const {
  return MiniCParser::RuleCompileUnit;
}


std::any MiniCParser::CompileUnitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitCompileUnit(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::CompileUnitContext* MiniCParser::compileUnit() {
  CompileUnitContext *_localctx = _tracker.createInstance<CompileUnitContext>(_ctx, getState());
  enterRule(_localctx, 0, MiniCParser::RuleCompileUnit);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(76);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1308622848) != 0)) {
      setState(74);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
      case 1: {
        setState(72);
        funcDef();
        break;
      }

      case 2: {
        setState(73);
        decl();
        break;
      }

      default:
        break;
      }
      setState(78);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(79);
    match(MiniCParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncDefContext ------------------------------------------------------------------

MiniCParser::FuncDefContext::FuncDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::FuncTypeContext* MiniCParser::FuncDefContext::funcType() {
  return getRuleContext<MiniCParser::FuncTypeContext>(0);
}

tree::TerminalNode* MiniCParser::FuncDefContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

tree::TerminalNode* MiniCParser::FuncDefContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

tree::TerminalNode* MiniCParser::FuncDefContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

MiniCParser::BlockContext* MiniCParser::FuncDefContext::block() {
  return getRuleContext<MiniCParser::BlockContext>(0);
}

MiniCParser::FuncFParamsContext* MiniCParser::FuncDefContext::funcFParams() {
  return getRuleContext<MiniCParser::FuncFParamsContext>(0);
}


size_t MiniCParser::FuncDefContext::getRuleIndex() const {
  return MiniCParser::RuleFuncDef;
}


std::any MiniCParser::FuncDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitFuncDef(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::FuncDefContext* MiniCParser::funcDef() {
  FuncDefContext *_localctx = _tracker.createInstance<FuncDefContext>(_ctx, getState());
  enterRule(_localctx, 2, MiniCParser::RuleFuncDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(81);
    funcType();
    setState(82);
    match(MiniCParser::T_ID);
    setState(83);
    match(MiniCParser::T_L_PAREN);
    setState(85);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_INT

    || _la == MiniCParser::T_FLOAT) {
      setState(84);
      funcFParams();
    }
    setState(87);
    match(MiniCParser::T_R_PAREN);
    setState(88);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncTypeContext ------------------------------------------------------------------

MiniCParser::FuncTypeContext::FuncTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MiniCParser::FuncTypeContext::getRuleIndex() const {
  return MiniCParser::RuleFuncType;
}

void MiniCParser::FuncTypeContext::copyFrom(FuncTypeContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- FloatReturnContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::FloatReturnContext::T_FLOAT() {
  return getToken(MiniCParser::T_FLOAT, 0);
}

MiniCParser::FloatReturnContext::FloatReturnContext(FuncTypeContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::FloatReturnContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitFloatReturn(this);
  else
    return visitor->visitChildren(this);
}
//----------------- VoidReturnContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::VoidReturnContext::T_VOID() {
  return getToken(MiniCParser::T_VOID, 0);
}

MiniCParser::VoidReturnContext::VoidReturnContext(FuncTypeContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::VoidReturnContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitVoidReturn(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IntegerReturnContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::IntegerReturnContext::T_INT() {
  return getToken(MiniCParser::T_INT, 0);
}

MiniCParser::IntegerReturnContext::IntegerReturnContext(FuncTypeContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::IntegerReturnContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitIntegerReturn(this);
  else
    return visitor->visitChildren(this);
}
MiniCParser::FuncTypeContext* MiniCParser::funcType() {
  FuncTypeContext *_localctx = _tracker.createInstance<FuncTypeContext>(_ctx, getState());
  enterRule(_localctx, 4, MiniCParser::RuleFuncType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(93);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniCParser::T_INT: {
        _localctx = _tracker.createInstance<MiniCParser::IntegerReturnContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(90);
        match(MiniCParser::T_INT);
        break;
      }

      case MiniCParser::T_FLOAT: {
        _localctx = _tracker.createInstance<MiniCParser::FloatReturnContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(91);
        match(MiniCParser::T_FLOAT);
        break;
      }

      case MiniCParser::T_VOID: {
        _localctx = _tracker.createInstance<MiniCParser::VoidReturnContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(92);
        match(MiniCParser::T_VOID);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncFParamsContext ------------------------------------------------------------------

MiniCParser::FuncFParamsContext::FuncFParamsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::FuncFParamContext *> MiniCParser::FuncFParamsContext::funcFParam() {
  return getRuleContexts<MiniCParser::FuncFParamContext>();
}

MiniCParser::FuncFParamContext* MiniCParser::FuncFParamsContext::funcFParam(size_t i) {
  return getRuleContext<MiniCParser::FuncFParamContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::FuncFParamsContext::T_COMMA() {
  return getTokens(MiniCParser::T_COMMA);
}

tree::TerminalNode* MiniCParser::FuncFParamsContext::T_COMMA(size_t i) {
  return getToken(MiniCParser::T_COMMA, i);
}


size_t MiniCParser::FuncFParamsContext::getRuleIndex() const {
  return MiniCParser::RuleFuncFParams;
}


std::any MiniCParser::FuncFParamsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitFuncFParams(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::FuncFParamsContext* MiniCParser::funcFParams() {
  FuncFParamsContext *_localctx = _tracker.createInstance<FuncFParamsContext>(_ctx, getState());
  enterRule(_localctx, 6, MiniCParser::RuleFuncFParams);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(95);
    funcFParam();
    setState(100);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_COMMA) {
      setState(96);
      match(MiniCParser::T_COMMA);
      setState(97);
      funcFParam();
      setState(102);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncFParamContext ------------------------------------------------------------------

MiniCParser::FuncFParamContext::FuncFParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::BasicTypeContext* MiniCParser::FuncFParamContext::basicType() {
  return getRuleContext<MiniCParser::BasicTypeContext>(0);
}

tree::TerminalNode* MiniCParser::FuncFParamContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

std::vector<tree::TerminalNode *> MiniCParser::FuncFParamContext::T_L_SQBRA() {
  return getTokens(MiniCParser::T_L_SQBRA);
}

tree::TerminalNode* MiniCParser::FuncFParamContext::T_L_SQBRA(size_t i) {
  return getToken(MiniCParser::T_L_SQBRA, i);
}

std::vector<tree::TerminalNode *> MiniCParser::FuncFParamContext::T_R_SQBRA() {
  return getTokens(MiniCParser::T_R_SQBRA);
}

tree::TerminalNode* MiniCParser::FuncFParamContext::T_R_SQBRA(size_t i) {
  return getToken(MiniCParser::T_R_SQBRA, i);
}

std::vector<MiniCParser::ExprContext *> MiniCParser::FuncFParamContext::expr() {
  return getRuleContexts<MiniCParser::ExprContext>();
}

MiniCParser::ExprContext* MiniCParser::FuncFParamContext::expr(size_t i) {
  return getRuleContext<MiniCParser::ExprContext>(i);
}


size_t MiniCParser::FuncFParamContext::getRuleIndex() const {
  return MiniCParser::RuleFuncFParam;
}


std::any MiniCParser::FuncFParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitFuncFParam(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::FuncFParamContext* MiniCParser::funcFParam() {
  FuncFParamContext *_localctx = _tracker.createInstance<FuncFParamContext>(_ctx, getState());
  enterRule(_localctx, 8, MiniCParser::RuleFuncFParam);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(103);
    basicType();
    setState(104);
    match(MiniCParser::T_ID);
    setState(116);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_L_SQBRA) {
      setState(105);
      match(MiniCParser::T_L_SQBRA);
      setState(106);
      match(MiniCParser::T_R_SQBRA);
      setState(113);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MiniCParser::T_L_SQBRA) {
        setState(107);
        match(MiniCParser::T_L_SQBRA);
        setState(108);
        expr();
        setState(109);
        match(MiniCParser::T_R_SQBRA);
        setState(115);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

MiniCParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::BlockContext::T_L_BRACE() {
  return getToken(MiniCParser::T_L_BRACE, 0);
}

tree::TerminalNode* MiniCParser::BlockContext::T_R_BRACE() {
  return getToken(MiniCParser::T_R_BRACE, 0);
}

MiniCParser::BlockItemListContext* MiniCParser::BlockContext::blockItemList() {
  return getRuleContext<MiniCParser::BlockItemListContext>(0);
}


size_t MiniCParser::BlockContext::getRuleIndex() const {
  return MiniCParser::RuleBlock;
}


std::any MiniCParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::BlockContext* MiniCParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 10, MiniCParser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(118);
    match(MiniCParser::T_L_BRACE);
    setState(120);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 136751100182) != 0)) {
      setState(119);
      blockItemList();
    }
    setState(122);
    match(MiniCParser::T_R_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockItemListContext ------------------------------------------------------------------

MiniCParser::BlockItemListContext::BlockItemListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::BlockItemContext *> MiniCParser::BlockItemListContext::blockItem() {
  return getRuleContexts<MiniCParser::BlockItemContext>();
}

MiniCParser::BlockItemContext* MiniCParser::BlockItemListContext::blockItem(size_t i) {
  return getRuleContext<MiniCParser::BlockItemContext>(i);
}


size_t MiniCParser::BlockItemListContext::getRuleIndex() const {
  return MiniCParser::RuleBlockItemList;
}


std::any MiniCParser::BlockItemListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBlockItemList(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::BlockItemListContext* MiniCParser::blockItemList() {
  BlockItemListContext *_localctx = _tracker.createInstance<BlockItemListContext>(_ctx, getState());
  enterRule(_localctx, 12, MiniCParser::RuleBlockItemList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(125); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(124);
      blockItem();
      setState(127); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 136751100182) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockItemContext ------------------------------------------------------------------

MiniCParser::BlockItemContext::BlockItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::StatementContext* MiniCParser::BlockItemContext::statement() {
  return getRuleContext<MiniCParser::StatementContext>(0);
}

MiniCParser::DeclContext* MiniCParser::BlockItemContext::decl() {
  return getRuleContext<MiniCParser::DeclContext>(0);
}


size_t MiniCParser::BlockItemContext::getRuleIndex() const {
  return MiniCParser::RuleBlockItem;
}


std::any MiniCParser::BlockItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBlockItem(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::BlockItemContext* MiniCParser::blockItem() {
  BlockItemContext *_localctx = _tracker.createInstance<BlockItemContext>(_ctx, getState());
  enterRule(_localctx, 14, MiniCParser::RuleBlockItem);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(131);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniCParser::T_SEMICOLON:
      case MiniCParser::T_L_BRACE:
      case MiniCParser::T_L_PAREN:
      case MiniCParser::T_NOT:
      case MiniCParser::T_ADD:
      case MiniCParser::T_SUB:
      case MiniCParser::T_RETURN:
      case MiniCParser::T_IF:
      case MiniCParser::T_WHILE:
      case MiniCParser::T_BREAK:
      case MiniCParser::T_CONTINUE:
      case MiniCParser::T_ID:
      case MiniCParser::T_DIGIT:
      case MiniCParser::T_FLOAT_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(129);
        statement();
        break;
      }

      case MiniCParser::T_INT:
      case MiniCParser::T_FLOAT:
      case MiniCParser::T_CONST: {
        enterOuterAlt(_localctx, 2);
        setState(130);
        decl();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclContext ------------------------------------------------------------------

MiniCParser::DeclContext::DeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::ConstDeclContext* MiniCParser::DeclContext::constDecl() {
  return getRuleContext<MiniCParser::ConstDeclContext>(0);
}

MiniCParser::VarDeclContext* MiniCParser::DeclContext::varDecl() {
  return getRuleContext<MiniCParser::VarDeclContext>(0);
}


size_t MiniCParser::DeclContext::getRuleIndex() const {
  return MiniCParser::RuleDecl;
}


std::any MiniCParser::DeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitDecl(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::DeclContext* MiniCParser::decl() {
  DeclContext *_localctx = _tracker.createInstance<DeclContext>(_ctx, getState());
  enterRule(_localctx, 16, MiniCParser::RuleDecl);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(135);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniCParser::T_CONST: {
        enterOuterAlt(_localctx, 1);
        setState(133);
        constDecl();
        break;
      }

      case MiniCParser::T_INT:
      case MiniCParser::T_FLOAT: {
        enterOuterAlt(_localctx, 2);
        setState(134);
        varDecl();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BasicTypeContext ------------------------------------------------------------------

MiniCParser::BasicTypeContext::BasicTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::BasicTypeContext::T_INT() {
  return getToken(MiniCParser::T_INT, 0);
}

tree::TerminalNode* MiniCParser::BasicTypeContext::T_FLOAT() {
  return getToken(MiniCParser::T_FLOAT, 0);
}


size_t MiniCParser::BasicTypeContext::getRuleIndex() const {
  return MiniCParser::RuleBasicType;
}


std::any MiniCParser::BasicTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBasicType(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::BasicTypeContext* MiniCParser::basicType() {
  BasicTypeContext *_localctx = _tracker.createInstance<BasicTypeContext>(_ctx, getState());
  enterRule(_localctx, 18, MiniCParser::RuleBasicType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(137);
    _la = _input->LA(1);
    if (!(_la == MiniCParser::T_INT

    || _la == MiniCParser::T_FLOAT)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstDeclContext ------------------------------------------------------------------

MiniCParser::ConstDeclContext::ConstDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::ConstDeclContext::T_CONST() {
  return getToken(MiniCParser::T_CONST, 0);
}

MiniCParser::BasicTypeContext* MiniCParser::ConstDeclContext::basicType() {
  return getRuleContext<MiniCParser::BasicTypeContext>(0);
}

std::vector<MiniCParser::ConstDefContext *> MiniCParser::ConstDeclContext::constDef() {
  return getRuleContexts<MiniCParser::ConstDefContext>();
}

MiniCParser::ConstDefContext* MiniCParser::ConstDeclContext::constDef(size_t i) {
  return getRuleContext<MiniCParser::ConstDefContext>(i);
}

tree::TerminalNode* MiniCParser::ConstDeclContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

std::vector<tree::TerminalNode *> MiniCParser::ConstDeclContext::T_COMMA() {
  return getTokens(MiniCParser::T_COMMA);
}

tree::TerminalNode* MiniCParser::ConstDeclContext::T_COMMA(size_t i) {
  return getToken(MiniCParser::T_COMMA, i);
}


size_t MiniCParser::ConstDeclContext::getRuleIndex() const {
  return MiniCParser::RuleConstDecl;
}


std::any MiniCParser::ConstDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitConstDecl(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::ConstDeclContext* MiniCParser::constDecl() {
  ConstDeclContext *_localctx = _tracker.createInstance<ConstDeclContext>(_ctx, getState());
  enterRule(_localctx, 20, MiniCParser::RuleConstDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(139);
    match(MiniCParser::T_CONST);
    setState(140);
    basicType();
    setState(141);
    constDef();
    setState(146);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_COMMA) {
      setState(142);
      match(MiniCParser::T_COMMA);
      setState(143);
      constDef();
      setState(148);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(149);
    match(MiniCParser::T_SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstDefContext ------------------------------------------------------------------

MiniCParser::ConstDefContext::ConstDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::ConstDefContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

tree::TerminalNode* MiniCParser::ConstDefContext::T_EQUAL() {
  return getToken(MiniCParser::T_EQUAL, 0);
}

MiniCParser::ConstInitValContext* MiniCParser::ConstDefContext::constInitVal() {
  return getRuleContext<MiniCParser::ConstInitValContext>(0);
}

std::vector<tree::TerminalNode *> MiniCParser::ConstDefContext::T_L_SQBRA() {
  return getTokens(MiniCParser::T_L_SQBRA);
}

tree::TerminalNode* MiniCParser::ConstDefContext::T_L_SQBRA(size_t i) {
  return getToken(MiniCParser::T_L_SQBRA, i);
}

std::vector<MiniCParser::ConstExpContext *> MiniCParser::ConstDefContext::constExp() {
  return getRuleContexts<MiniCParser::ConstExpContext>();
}

MiniCParser::ConstExpContext* MiniCParser::ConstDefContext::constExp(size_t i) {
  return getRuleContext<MiniCParser::ConstExpContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::ConstDefContext::T_R_SQBRA() {
  return getTokens(MiniCParser::T_R_SQBRA);
}

tree::TerminalNode* MiniCParser::ConstDefContext::T_R_SQBRA(size_t i) {
  return getToken(MiniCParser::T_R_SQBRA, i);
}


size_t MiniCParser::ConstDefContext::getRuleIndex() const {
  return MiniCParser::RuleConstDef;
}


std::any MiniCParser::ConstDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitConstDef(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::ConstDefContext* MiniCParser::constDef() {
  ConstDefContext *_localctx = _tracker.createInstance<ConstDefContext>(_ctx, getState());
  enterRule(_localctx, 22, MiniCParser::RuleConstDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(151);
    match(MiniCParser::T_ID);
    setState(158);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_L_SQBRA) {
      setState(152);
      match(MiniCParser::T_L_SQBRA);
      setState(153);
      constExp();
      setState(154);
      match(MiniCParser::T_R_SQBRA);
      setState(160);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(161);
    match(MiniCParser::T_EQUAL);
    setState(162);
    constInitVal();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstInitValContext ------------------------------------------------------------------

MiniCParser::ConstInitValContext::ConstInitValContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MiniCParser::ConstInitValContext::getRuleIndex() const {
  return MiniCParser::RuleConstInitVal;
}

void MiniCParser::ConstInitValContext::copyFrom(ConstInitValContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- SingleConstValContext ------------------------------------------------------------------

MiniCParser::ConstExpContext* MiniCParser::SingleConstValContext::constExp() {
  return getRuleContext<MiniCParser::ConstExpContext>(0);
}

MiniCParser::SingleConstValContext::SingleConstValContext(ConstInitValContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::SingleConstValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitSingleConstVal(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MultiConstValContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::MultiConstValContext::T_L_BRACE() {
  return getToken(MiniCParser::T_L_BRACE, 0);
}

std::vector<MiniCParser::ConstInitValContext *> MiniCParser::MultiConstValContext::constInitVal() {
  return getRuleContexts<MiniCParser::ConstInitValContext>();
}

MiniCParser::ConstInitValContext* MiniCParser::MultiConstValContext::constInitVal(size_t i) {
  return getRuleContext<MiniCParser::ConstInitValContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::MultiConstValContext::T_COMMA() {
  return getTokens(MiniCParser::T_COMMA);
}

tree::TerminalNode* MiniCParser::MultiConstValContext::T_COMMA(size_t i) {
  return getToken(MiniCParser::T_COMMA, i);
}

MiniCParser::MultiConstValContext::MultiConstValContext(ConstInitValContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::MultiConstValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitMultiConstVal(this);
  else
    return visitor->visitChildren(this);
}
MiniCParser::ConstInitValContext* MiniCParser::constInitVal() {
  ConstInitValContext *_localctx = _tracker.createInstance<ConstInitValContext>(_ctx, getState());
  enterRule(_localctx, 24, MiniCParser::RuleConstInitVal);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    setState(176);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniCParser::T_L_PAREN:
      case MiniCParser::T_NOT:
      case MiniCParser::T_ADD:
      case MiniCParser::T_SUB:
      case MiniCParser::T_ID:
      case MiniCParser::T_DIGIT:
      case MiniCParser::T_FLOAT_LITERAL: {
        _localctx = _tracker.createInstance<MiniCParser::SingleConstValContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(164);
        constExp();
        break;
      }

      case MiniCParser::T_L_BRACE: {
        _localctx = _tracker.createInstance<MiniCParser::MultiConstValContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(165);
        match(MiniCParser::T_L_BRACE);
        setState(174);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 120259096852) != 0)) {
          setState(166);
          constInitVal();
          setState(171);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
          while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
              setState(167);
              match(MiniCParser::T_COMMA);
              setState(168);
              constInitVal(); 
            }
            setState(173);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
          }
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDeclContext ------------------------------------------------------------------

MiniCParser::VarDeclContext::VarDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::BasicTypeContext* MiniCParser::VarDeclContext::basicType() {
  return getRuleContext<MiniCParser::BasicTypeContext>(0);
}

std::vector<MiniCParser::VarDefContext *> MiniCParser::VarDeclContext::varDef() {
  return getRuleContexts<MiniCParser::VarDefContext>();
}

MiniCParser::VarDefContext* MiniCParser::VarDeclContext::varDef(size_t i) {
  return getRuleContext<MiniCParser::VarDefContext>(i);
}

tree::TerminalNode* MiniCParser::VarDeclContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

std::vector<tree::TerminalNode *> MiniCParser::VarDeclContext::T_COMMA() {
  return getTokens(MiniCParser::T_COMMA);
}

tree::TerminalNode* MiniCParser::VarDeclContext::T_COMMA(size_t i) {
  return getToken(MiniCParser::T_COMMA, i);
}


size_t MiniCParser::VarDeclContext::getRuleIndex() const {
  return MiniCParser::RuleVarDecl;
}


std::any MiniCParser::VarDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitVarDecl(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::VarDeclContext* MiniCParser::varDecl() {
  VarDeclContext *_localctx = _tracker.createInstance<VarDeclContext>(_ctx, getState());
  enterRule(_localctx, 26, MiniCParser::RuleVarDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(178);
    basicType();
    setState(179);
    varDef();
    setState(184);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_COMMA) {
      setState(180);
      match(MiniCParser::T_COMMA);
      setState(181);
      varDef();
      setState(186);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(187);
    match(MiniCParser::T_SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDefContext ------------------------------------------------------------------

MiniCParser::VarDefContext::VarDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::VarDefContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

std::vector<tree::TerminalNode *> MiniCParser::VarDefContext::T_L_SQBRA() {
  return getTokens(MiniCParser::T_L_SQBRA);
}

tree::TerminalNode* MiniCParser::VarDefContext::T_L_SQBRA(size_t i) {
  return getToken(MiniCParser::T_L_SQBRA, i);
}

std::vector<MiniCParser::ConstExpContext *> MiniCParser::VarDefContext::constExp() {
  return getRuleContexts<MiniCParser::ConstExpContext>();
}

MiniCParser::ConstExpContext* MiniCParser::VarDefContext::constExp(size_t i) {
  return getRuleContext<MiniCParser::ConstExpContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::VarDefContext::T_R_SQBRA() {
  return getTokens(MiniCParser::T_R_SQBRA);
}

tree::TerminalNode* MiniCParser::VarDefContext::T_R_SQBRA(size_t i) {
  return getToken(MiniCParser::T_R_SQBRA, i);
}

tree::TerminalNode* MiniCParser::VarDefContext::T_EQUAL() {
  return getToken(MiniCParser::T_EQUAL, 0);
}

MiniCParser::InitValContext* MiniCParser::VarDefContext::initVal() {
  return getRuleContext<MiniCParser::InitValContext>(0);
}


size_t MiniCParser::VarDefContext::getRuleIndex() const {
  return MiniCParser::RuleVarDef;
}


std::any MiniCParser::VarDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitVarDef(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::VarDefContext* MiniCParser::varDef() {
  VarDefContext *_localctx = _tracker.createInstance<VarDefContext>(_ctx, getState());
  enterRule(_localctx, 28, MiniCParser::RuleVarDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(189);
    match(MiniCParser::T_ID);
    setState(196);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_L_SQBRA) {
      setState(190);
      match(MiniCParser::T_L_SQBRA);
      setState(191);
      constExp();
      setState(192);
      match(MiniCParser::T_R_SQBRA);
      setState(198);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(201);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_EQUAL) {
      setState(199);
      match(MiniCParser::T_EQUAL);
      setState(200);
      initVal();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InitValContext ------------------------------------------------------------------

MiniCParser::InitValContext::InitValContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MiniCParser::InitValContext::getRuleIndex() const {
  return MiniCParser::RuleInitVal;
}

void MiniCParser::InitValContext::copyFrom(InitValContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- MultiValContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::MultiValContext::T_L_BRACE() {
  return getToken(MiniCParser::T_L_BRACE, 0);
}

tree::TerminalNode* MiniCParser::MultiValContext::T_R_BRACE() {
  return getToken(MiniCParser::T_R_BRACE, 0);
}

std::vector<MiniCParser::InitValContext *> MiniCParser::MultiValContext::initVal() {
  return getRuleContexts<MiniCParser::InitValContext>();
}

MiniCParser::InitValContext* MiniCParser::MultiValContext::initVal(size_t i) {
  return getRuleContext<MiniCParser::InitValContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::MultiValContext::T_COMMA() {
  return getTokens(MiniCParser::T_COMMA);
}

tree::TerminalNode* MiniCParser::MultiValContext::T_COMMA(size_t i) {
  return getToken(MiniCParser::T_COMMA, i);
}

MiniCParser::MultiValContext::MultiValContext(InitValContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::MultiValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitMultiVal(this);
  else
    return visitor->visitChildren(this);
}
//----------------- SingleValContext ------------------------------------------------------------------

MiniCParser::ExprContext* MiniCParser::SingleValContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

MiniCParser::SingleValContext::SingleValContext(InitValContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::SingleValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitSingleVal(this);
  else
    return visitor->visitChildren(this);
}
MiniCParser::InitValContext* MiniCParser::initVal() {
  InitValContext *_localctx = _tracker.createInstance<InitValContext>(_ctx, getState());
  enterRule(_localctx, 30, MiniCParser::RuleInitVal);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(216);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniCParser::T_L_PAREN:
      case MiniCParser::T_NOT:
      case MiniCParser::T_ADD:
      case MiniCParser::T_SUB:
      case MiniCParser::T_ID:
      case MiniCParser::T_DIGIT:
      case MiniCParser::T_FLOAT_LITERAL: {
        _localctx = _tracker.createInstance<MiniCParser::SingleValContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(203);
        expr();
        break;
      }

      case MiniCParser::T_L_BRACE: {
        _localctx = _tracker.createInstance<MiniCParser::MultiValContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(204);
        match(MiniCParser::T_L_BRACE);
        setState(213);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 120259096852) != 0)) {
          setState(205);
          initVal();
          setState(210);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == MiniCParser::T_COMMA) {
            setState(206);
            match(MiniCParser::T_COMMA);
            setState(207);
            initVal();
            setState(212);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
        }
        setState(215);
        match(MiniCParser::T_R_BRACE);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

MiniCParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MiniCParser::StatementContext::getRuleIndex() const {
  return MiniCParser::RuleStatement;
}

void MiniCParser::StatementContext::copyFrom(StatementContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- WhileStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::WhileStatementContext::T_WHILE() {
  return getToken(MiniCParser::T_WHILE, 0);
}

tree::TerminalNode* MiniCParser::WhileStatementContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

MiniCParser::CondContext* MiniCParser::WhileStatementContext::cond() {
  return getRuleContext<MiniCParser::CondContext>(0);
}

tree::TerminalNode* MiniCParser::WhileStatementContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

MiniCParser::StatementContext* MiniCParser::WhileStatementContext::statement() {
  return getRuleContext<MiniCParser::StatementContext>(0);
}

MiniCParser::WhileStatementContext::WhileStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::WhileStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitWhileStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BlockStatementContext ------------------------------------------------------------------

MiniCParser::BlockContext* MiniCParser::BlockStatementContext::block() {
  return getRuleContext<MiniCParser::BlockContext>(0);
}

MiniCParser::BlockStatementContext::BlockStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::BlockStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBlockStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IfelseStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::IfelseStatementContext::T_IF() {
  return getToken(MiniCParser::T_IF, 0);
}

tree::TerminalNode* MiniCParser::IfelseStatementContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

MiniCParser::CondContext* MiniCParser::IfelseStatementContext::cond() {
  return getRuleContext<MiniCParser::CondContext>(0);
}

tree::TerminalNode* MiniCParser::IfelseStatementContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

std::vector<MiniCParser::StatementContext *> MiniCParser::IfelseStatementContext::statement() {
  return getRuleContexts<MiniCParser::StatementContext>();
}

MiniCParser::StatementContext* MiniCParser::IfelseStatementContext::statement(size_t i) {
  return getRuleContext<MiniCParser::StatementContext>(i);
}

tree::TerminalNode* MiniCParser::IfelseStatementContext::T_ELSE() {
  return getToken(MiniCParser::T_ELSE, 0);
}

MiniCParser::IfelseStatementContext::IfelseStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::IfelseStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitIfelseStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BreakstatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::BreakstatementContext::T_BREAK() {
  return getToken(MiniCParser::T_BREAK, 0);
}

tree::TerminalNode* MiniCParser::BreakstatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::BreakstatementContext::BreakstatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::BreakstatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBreakstatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ContinuestatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::ContinuestatementContext::T_CONTINUE() {
  return getToken(MiniCParser::T_CONTINUE, 0);
}

tree::TerminalNode* MiniCParser::ContinuestatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::ContinuestatementContext::ContinuestatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::ContinuestatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitContinuestatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AssignStatementContext ------------------------------------------------------------------

MiniCParser::LValContext* MiniCParser::AssignStatementContext::lVal() {
  return getRuleContext<MiniCParser::LValContext>(0);
}

tree::TerminalNode* MiniCParser::AssignStatementContext::T_ASSIGN() {
  return getToken(MiniCParser::T_ASSIGN, 0);
}

MiniCParser::ExprContext* MiniCParser::AssignStatementContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

tree::TerminalNode* MiniCParser::AssignStatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::AssignStatementContext::AssignStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::AssignStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitAssignStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExpressionStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::ExpressionStatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::ExprContext* MiniCParser::ExpressionStatementContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

MiniCParser::ExpressionStatementContext::ExpressionStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::ExpressionStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitExpressionStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReturnStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::ReturnStatementContext::T_RETURN() {
  return getToken(MiniCParser::T_RETURN, 0);
}

tree::TerminalNode* MiniCParser::ReturnStatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::ExprContext* MiniCParser::ReturnStatementContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

MiniCParser::ReturnStatementContext::ReturnStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::ReturnStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitReturnStatement(this);
  else
    return visitor->visitChildren(this);
}
MiniCParser::StatementContext* MiniCParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 32, MiniCParser::RuleStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(252);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MiniCParser::ReturnStatementContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(218);
      match(MiniCParser::T_RETURN);
      setState(220);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 120259096848) != 0)) {
        setState(219);
        expr();
      }
      setState(222);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MiniCParser::AssignStatementContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(223);
      lVal();
      setState(224);
      match(MiniCParser::T_ASSIGN);
      setState(225);
      expr();
      setState(226);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MiniCParser::BlockStatementContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(228);
      block();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MiniCParser::ExpressionStatementContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(230);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 120259096848) != 0)) {
        setState(229);
        expr();
      }
      setState(232);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<MiniCParser::IfelseStatementContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(233);
      match(MiniCParser::T_IF);
      setState(234);
      match(MiniCParser::T_L_PAREN);
      setState(235);
      cond();
      setState(236);
      match(MiniCParser::T_R_PAREN);
      setState(237);
      statement();
      setState(240);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
      case 1: {
        setState(238);
        match(MiniCParser::T_ELSE);
        setState(239);
        statement();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<MiniCParser::WhileStatementContext>(_localctx);
      enterOuterAlt(_localctx, 6);
      setState(242);
      match(MiniCParser::T_WHILE);
      setState(243);
      match(MiniCParser::T_L_PAREN);
      setState(244);
      cond();
      setState(245);
      match(MiniCParser::T_R_PAREN);
      setState(246);
      statement();
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<MiniCParser::BreakstatementContext>(_localctx);
      enterOuterAlt(_localctx, 7);
      setState(248);
      match(MiniCParser::T_BREAK);
      setState(249);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<MiniCParser::ContinuestatementContext>(_localctx);
      enterOuterAlt(_localctx, 8);
      setState(250);
      match(MiniCParser::T_CONTINUE);
      setState(251);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

MiniCParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::CondContext* MiniCParser::ExprContext::cond() {
  return getRuleContext<MiniCParser::CondContext>(0);
}


size_t MiniCParser::ExprContext::getRuleIndex() const {
  return MiniCParser::RuleExpr;
}


std::any MiniCParser::ExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitExpr(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::ExprContext* MiniCParser::expr() {
  ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, getState());
  enterRule(_localctx, 34, MiniCParser::RuleExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(254);
    cond();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CondContext ------------------------------------------------------------------

MiniCParser::CondContext::CondContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::LOrExpContext* MiniCParser::CondContext::lOrExp() {
  return getRuleContext<MiniCParser::LOrExpContext>(0);
}


size_t MiniCParser::CondContext::getRuleIndex() const {
  return MiniCParser::RuleCond;
}


std::any MiniCParser::CondContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitCond(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::CondContext* MiniCParser::cond() {
  CondContext *_localctx = _tracker.createInstance<CondContext>(_ctx, getState());
  enterRule(_localctx, 36, MiniCParser::RuleCond);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(256);
    lOrExp();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LValContext ------------------------------------------------------------------

MiniCParser::LValContext::LValContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::LValContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

std::vector<tree::TerminalNode *> MiniCParser::LValContext::T_L_SQBRA() {
  return getTokens(MiniCParser::T_L_SQBRA);
}

tree::TerminalNode* MiniCParser::LValContext::T_L_SQBRA(size_t i) {
  return getToken(MiniCParser::T_L_SQBRA, i);
}

std::vector<MiniCParser::ExprContext *> MiniCParser::LValContext::expr() {
  return getRuleContexts<MiniCParser::ExprContext>();
}

MiniCParser::ExprContext* MiniCParser::LValContext::expr(size_t i) {
  return getRuleContext<MiniCParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::LValContext::T_R_SQBRA() {
  return getTokens(MiniCParser::T_R_SQBRA);
}

tree::TerminalNode* MiniCParser::LValContext::T_R_SQBRA(size_t i) {
  return getToken(MiniCParser::T_R_SQBRA, i);
}


size_t MiniCParser::LValContext::getRuleIndex() const {
  return MiniCParser::RuleLVal;
}


std::any MiniCParser::LValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitLVal(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::LValContext* MiniCParser::lVal() {
  LValContext *_localctx = _tracker.createInstance<LValContext>(_ctx, getState());
  enterRule(_localctx, 38, MiniCParser::RuleLVal);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(258);
    match(MiniCParser::T_ID);
    setState(265);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_L_SQBRA) {
      setState(259);
      match(MiniCParser::T_L_SQBRA);
      setState(260);
      expr();
      setState(261);
      match(MiniCParser::T_R_SQBRA);
      setState(267);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimaryExpContext ------------------------------------------------------------------

MiniCParser::PrimaryExpContext::PrimaryExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::PrimaryExpContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

MiniCParser::ExprContext* MiniCParser::PrimaryExpContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

tree::TerminalNode* MiniCParser::PrimaryExpContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

MiniCParser::LValContext* MiniCParser::PrimaryExpContext::lVal() {
  return getRuleContext<MiniCParser::LValContext>(0);
}

MiniCParser::NumberContext* MiniCParser::PrimaryExpContext::number() {
  return getRuleContext<MiniCParser::NumberContext>(0);
}


size_t MiniCParser::PrimaryExpContext::getRuleIndex() const {
  return MiniCParser::RulePrimaryExp;
}


std::any MiniCParser::PrimaryExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitPrimaryExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::PrimaryExpContext* MiniCParser::primaryExp() {
  PrimaryExpContext *_localctx = _tracker.createInstance<PrimaryExpContext>(_ctx, getState());
  enterRule(_localctx, 40, MiniCParser::RulePrimaryExp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(274);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniCParser::T_L_PAREN: {
        enterOuterAlt(_localctx, 1);
        setState(268);
        match(MiniCParser::T_L_PAREN);
        setState(269);
        expr();
        setState(270);
        match(MiniCParser::T_R_PAREN);
        break;
      }

      case MiniCParser::T_ID: {
        enterOuterAlt(_localctx, 2);
        setState(272);
        lVal();
        break;
      }

      case MiniCParser::T_DIGIT:
      case MiniCParser::T_FLOAT_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(273);
        number();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumberContext ------------------------------------------------------------------

MiniCParser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::NumberContext::T_DIGIT() {
  return getToken(MiniCParser::T_DIGIT, 0);
}

tree::TerminalNode* MiniCParser::NumberContext::T_FLOAT_LITERAL() {
  return getToken(MiniCParser::T_FLOAT_LITERAL, 0);
}


size_t MiniCParser::NumberContext::getRuleIndex() const {
  return MiniCParser::RuleNumber;
}


std::any MiniCParser::NumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitNumber(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::NumberContext* MiniCParser::number() {
  NumberContext *_localctx = _tracker.createInstance<NumberContext>(_ctx, getState());
  enterRule(_localctx, 42, MiniCParser::RuleNumber);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(276);
    _la = _input->LA(1);
    if (!(_la == MiniCParser::T_DIGIT

    || _la == MiniCParser::T_FLOAT_LITERAL)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryExpContext ------------------------------------------------------------------

MiniCParser::UnaryExpContext::UnaryExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MiniCParser::UnaryExpContext::getRuleIndex() const {
  return MiniCParser::RuleUnaryExp;
}

void MiniCParser::UnaryExpContext::copyFrom(UnaryExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- MonoOpContext ------------------------------------------------------------------

MiniCParser::UnaryOpContext* MiniCParser::MonoOpContext::unaryOp() {
  return getRuleContext<MiniCParser::UnaryOpContext>(0);
}

MiniCParser::UnaryExpContext* MiniCParser::MonoOpContext::unaryExp() {
  return getRuleContext<MiniCParser::UnaryExpContext>(0);
}

MiniCParser::MonoOpContext::MonoOpContext(UnaryExpContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::MonoOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitMonoOp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FuncCallContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::FuncCallContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

tree::TerminalNode* MiniCParser::FuncCallContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

tree::TerminalNode* MiniCParser::FuncCallContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

MiniCParser::FuncRParamsContext* MiniCParser::FuncCallContext::funcRParams() {
  return getRuleContext<MiniCParser::FuncRParamsContext>(0);
}

MiniCParser::FuncCallContext::FuncCallContext(UnaryExpContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::FuncCallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitFuncCall(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PrimaryContext ------------------------------------------------------------------

MiniCParser::PrimaryExpContext* MiniCParser::PrimaryContext::primaryExp() {
  return getRuleContext<MiniCParser::PrimaryExpContext>(0);
}

MiniCParser::PrimaryContext::PrimaryContext(UnaryExpContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::PrimaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitPrimary(this);
  else
    return visitor->visitChildren(this);
}
MiniCParser::UnaryExpContext* MiniCParser::unaryExp() {
  UnaryExpContext *_localctx = _tracker.createInstance<UnaryExpContext>(_ctx, getState());
  enterRule(_localctx, 44, MiniCParser::RuleUnaryExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(288);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MiniCParser::PrimaryContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(278);
      primaryExp();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MiniCParser::FuncCallContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(279);
      match(MiniCParser::T_ID);
      setState(280);
      match(MiniCParser::T_L_PAREN);
      setState(282);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 120259096848) != 0)) {
        setState(281);
        funcRParams();
      }
      setState(284);
      match(MiniCParser::T_R_PAREN);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MiniCParser::MonoOpContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(285);
      unaryOp();
      setState(286);
      unaryExp();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryOpContext ------------------------------------------------------------------

MiniCParser::UnaryOpContext::UnaryOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::UnaryOpContext::T_ADD() {
  return getToken(MiniCParser::T_ADD, 0);
}

tree::TerminalNode* MiniCParser::UnaryOpContext::T_SUB() {
  return getToken(MiniCParser::T_SUB, 0);
}

tree::TerminalNode* MiniCParser::UnaryOpContext::T_NOT() {
  return getToken(MiniCParser::T_NOT, 0);
}


size_t MiniCParser::UnaryOpContext::getRuleIndex() const {
  return MiniCParser::RuleUnaryOp;
}


std::any MiniCParser::UnaryOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitUnaryOp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::UnaryOpContext* MiniCParser::unaryOp() {
  UnaryOpContext *_localctx = _tracker.createInstance<UnaryOpContext>(_ctx, getState());
  enterRule(_localctx, 46, MiniCParser::RuleUnaryOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(290);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 12544) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncRParamsContext ------------------------------------------------------------------

MiniCParser::FuncRParamsContext::FuncRParamsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::ExprContext *> MiniCParser::FuncRParamsContext::expr() {
  return getRuleContexts<MiniCParser::ExprContext>();
}

MiniCParser::ExprContext* MiniCParser::FuncRParamsContext::expr(size_t i) {
  return getRuleContext<MiniCParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::FuncRParamsContext::T_COMMA() {
  return getTokens(MiniCParser::T_COMMA);
}

tree::TerminalNode* MiniCParser::FuncRParamsContext::T_COMMA(size_t i) {
  return getToken(MiniCParser::T_COMMA, i);
}


size_t MiniCParser::FuncRParamsContext::getRuleIndex() const {
  return MiniCParser::RuleFuncRParams;
}


std::any MiniCParser::FuncRParamsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitFuncRParams(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::FuncRParamsContext* MiniCParser::funcRParams() {
  FuncRParamsContext *_localctx = _tracker.createInstance<FuncRParamsContext>(_ctx, getState());
  enterRule(_localctx, 48, MiniCParser::RuleFuncRParams);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(292);
    expr();
    setState(297);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_COMMA) {
      setState(293);
      match(MiniCParser::T_COMMA);
      setState(294);
      expr();
      setState(299);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MulExpContext ------------------------------------------------------------------

MiniCParser::MulExpContext::MulExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::UnaryExpContext* MiniCParser::MulExpContext::unaryExp() {
  return getRuleContext<MiniCParser::UnaryExpContext>(0);
}

MiniCParser::MulOpContext* MiniCParser::MulExpContext::mulOp() {
  return getRuleContext<MiniCParser::MulOpContext>(0);
}

MiniCParser::MulExpContext* MiniCParser::MulExpContext::mulExp() {
  return getRuleContext<MiniCParser::MulExpContext>(0);
}


size_t MiniCParser::MulExpContext::getRuleIndex() const {
  return MiniCParser::RuleMulExp;
}


std::any MiniCParser::MulExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitMulExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::MulExpContext* MiniCParser::mulExp() {
  MulExpContext *_localctx = _tracker.createInstance<MulExpContext>(_ctx, getState());
  enterRule(_localctx, 50, MiniCParser::RuleMulExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(300);
    unaryExp();
    setState(304);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3584) != 0)) {
      setState(301);
      mulOp();
      setState(302);
      mulExp();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MulOpContext ------------------------------------------------------------------

MiniCParser::MulOpContext::MulOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::MulOpContext::T_MUL() {
  return getToken(MiniCParser::T_MUL, 0);
}

tree::TerminalNode* MiniCParser::MulOpContext::T_DIV() {
  return getToken(MiniCParser::T_DIV, 0);
}

tree::TerminalNode* MiniCParser::MulOpContext::T_MOD() {
  return getToken(MiniCParser::T_MOD, 0);
}


size_t MiniCParser::MulOpContext::getRuleIndex() const {
  return MiniCParser::RuleMulOp;
}


std::any MiniCParser::MulOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitMulOp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::MulOpContext* MiniCParser::mulOp() {
  MulOpContext *_localctx = _tracker.createInstance<MulOpContext>(_ctx, getState());
  enterRule(_localctx, 52, MiniCParser::RuleMulOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(306);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3584) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AddExpContext ------------------------------------------------------------------

MiniCParser::AddExpContext::AddExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::MulExpContext* MiniCParser::AddExpContext::mulExp() {
  return getRuleContext<MiniCParser::MulExpContext>(0);
}

MiniCParser::AddOpContext* MiniCParser::AddExpContext::addOp() {
  return getRuleContext<MiniCParser::AddOpContext>(0);
}

MiniCParser::AddExpContext* MiniCParser::AddExpContext::addExp() {
  return getRuleContext<MiniCParser::AddExpContext>(0);
}


size_t MiniCParser::AddExpContext::getRuleIndex() const {
  return MiniCParser::RuleAddExp;
}


std::any MiniCParser::AddExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitAddExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::AddExpContext* MiniCParser::addExp() {
  AddExpContext *_localctx = _tracker.createInstance<AddExpContext>(_ctx, getState());
  enterRule(_localctx, 54, MiniCParser::RuleAddExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(308);
    mulExp();
    setState(312);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_ADD

    || _la == MiniCParser::T_SUB) {
      setState(309);
      addOp();
      setState(310);
      addExp();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AddOpContext ------------------------------------------------------------------

MiniCParser::AddOpContext::AddOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::AddOpContext::T_ADD() {
  return getToken(MiniCParser::T_ADD, 0);
}

tree::TerminalNode* MiniCParser::AddOpContext::T_SUB() {
  return getToken(MiniCParser::T_SUB, 0);
}


size_t MiniCParser::AddOpContext::getRuleIndex() const {
  return MiniCParser::RuleAddOp;
}


std::any MiniCParser::AddOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitAddOp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::AddOpContext* MiniCParser::addOp() {
  AddOpContext *_localctx = _tracker.createInstance<AddOpContext>(_ctx, getState());
  enterRule(_localctx, 56, MiniCParser::RuleAddOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(314);
    _la = _input->LA(1);
    if (!(_la == MiniCParser::T_ADD

    || _la == MiniCParser::T_SUB)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelExpContext ------------------------------------------------------------------

MiniCParser::RelExpContext::RelExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::AddExpContext* MiniCParser::RelExpContext::addExp() {
  return getRuleContext<MiniCParser::AddExpContext>(0);
}

MiniCParser::RelOpContext* MiniCParser::RelExpContext::relOp() {
  return getRuleContext<MiniCParser::RelOpContext>(0);
}

MiniCParser::RelExpContext* MiniCParser::RelExpContext::relExp() {
  return getRuleContext<MiniCParser::RelExpContext>(0);
}


size_t MiniCParser::RelExpContext::getRuleIndex() const {
  return MiniCParser::RuleRelExp;
}


std::any MiniCParser::RelExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitRelExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::RelExpContext* MiniCParser::relExp() {
  RelExpContext *_localctx = _tracker.createInstance<RelExpContext>(_ctx, getState());
  enterRule(_localctx, 58, MiniCParser::RuleRelExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(316);
    addExp();
    setState(320);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 245760) != 0)) {
      setState(317);
      relOp();
      setState(318);
      relExp();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelOpContext ------------------------------------------------------------------

MiniCParser::RelOpContext::RelOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::RelOpContext::T_GE() {
  return getToken(MiniCParser::T_GE, 0);
}

tree::TerminalNode* MiniCParser::RelOpContext::T_GREATER() {
  return getToken(MiniCParser::T_GREATER, 0);
}

tree::TerminalNode* MiniCParser::RelOpContext::T_LE() {
  return getToken(MiniCParser::T_LE, 0);
}

tree::TerminalNode* MiniCParser::RelOpContext::T_LESS() {
  return getToken(MiniCParser::T_LESS, 0);
}


size_t MiniCParser::RelOpContext::getRuleIndex() const {
  return MiniCParser::RuleRelOp;
}


std::any MiniCParser::RelOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitRelOp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::RelOpContext* MiniCParser::relOp() {
  RelOpContext *_localctx = _tracker.createInstance<RelOpContext>(_ctx, getState());
  enterRule(_localctx, 60, MiniCParser::RuleRelOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(322);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 245760) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EqExpContext ------------------------------------------------------------------

MiniCParser::EqExpContext::EqExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::RelExpContext* MiniCParser::EqExpContext::relExp() {
  return getRuleContext<MiniCParser::RelExpContext>(0);
}

MiniCParser::EqOpContext* MiniCParser::EqExpContext::eqOp() {
  return getRuleContext<MiniCParser::EqOpContext>(0);
}

MiniCParser::EqExpContext* MiniCParser::EqExpContext::eqExp() {
  return getRuleContext<MiniCParser::EqExpContext>(0);
}


size_t MiniCParser::EqExpContext::getRuleIndex() const {
  return MiniCParser::RuleEqExp;
}


std::any MiniCParser::EqExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitEqExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::EqExpContext* MiniCParser::eqExp() {
  EqExpContext *_localctx = _tracker.createInstance<EqExpContext>(_ctx, getState());
  enterRule(_localctx, 62, MiniCParser::RuleEqExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(324);
    relExp();
    setState(328);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_EQUAL

    || _la == MiniCParser::T_NEQUAL) {
      setState(325);
      eqOp();
      setState(326);
      eqExp();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EqOpContext ------------------------------------------------------------------

MiniCParser::EqOpContext::EqOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::EqOpContext::T_EQUAL() {
  return getToken(MiniCParser::T_EQUAL, 0);
}

tree::TerminalNode* MiniCParser::EqOpContext::T_NEQUAL() {
  return getToken(MiniCParser::T_NEQUAL, 0);
}


size_t MiniCParser::EqOpContext::getRuleIndex() const {
  return MiniCParser::RuleEqOp;
}


std::any MiniCParser::EqOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitEqOp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::EqOpContext* MiniCParser::eqOp() {
  EqOpContext *_localctx = _tracker.createInstance<EqOpContext>(_ctx, getState());
  enterRule(_localctx, 64, MiniCParser::RuleEqOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(330);
    _la = _input->LA(1);
    if (!(_la == MiniCParser::T_EQUAL

    || _la == MiniCParser::T_NEQUAL)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LAndExpContext ------------------------------------------------------------------

MiniCParser::LAndExpContext::LAndExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::EqExpContext* MiniCParser::LAndExpContext::eqExp() {
  return getRuleContext<MiniCParser::EqExpContext>(0);
}

tree::TerminalNode* MiniCParser::LAndExpContext::T_AND() {
  return getToken(MiniCParser::T_AND, 0);
}

MiniCParser::LAndExpContext* MiniCParser::LAndExpContext::lAndExp() {
  return getRuleContext<MiniCParser::LAndExpContext>(0);
}


size_t MiniCParser::LAndExpContext::getRuleIndex() const {
  return MiniCParser::RuleLAndExp;
}


std::any MiniCParser::LAndExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitLAndExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::LAndExpContext* MiniCParser::lAndExp() {
  LAndExpContext *_localctx = _tracker.createInstance<LAndExpContext>(_ctx, getState());
  enterRule(_localctx, 66, MiniCParser::RuleLAndExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(332);
    eqExp();
    setState(335);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_AND) {
      setState(333);
      match(MiniCParser::T_AND);
      setState(334);
      lAndExp();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LOrExpContext ------------------------------------------------------------------

MiniCParser::LOrExpContext::LOrExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::LAndExpContext* MiniCParser::LOrExpContext::lAndExp() {
  return getRuleContext<MiniCParser::LAndExpContext>(0);
}

tree::TerminalNode* MiniCParser::LOrExpContext::T_OR() {
  return getToken(MiniCParser::T_OR, 0);
}

MiniCParser::LOrExpContext* MiniCParser::LOrExpContext::lOrExp() {
  return getRuleContext<MiniCParser::LOrExpContext>(0);
}


size_t MiniCParser::LOrExpContext::getRuleIndex() const {
  return MiniCParser::RuleLOrExp;
}


std::any MiniCParser::LOrExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitLOrExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::LOrExpContext* MiniCParser::lOrExp() {
  LOrExpContext *_localctx = _tracker.createInstance<LOrExpContext>(_ctx, getState());
  enterRule(_localctx, 68, MiniCParser::RuleLOrExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(337);
    lAndExp();
    setState(340);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_OR) {
      setState(338);
      match(MiniCParser::T_OR);
      setState(339);
      lOrExp();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstExpContext ------------------------------------------------------------------

MiniCParser::ConstExpContext::ConstExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::AddExpContext* MiniCParser::ConstExpContext::addExp() {
  return getRuleContext<MiniCParser::AddExpContext>(0);
}


size_t MiniCParser::ConstExpContext::getRuleIndex() const {
  return MiniCParser::RuleConstExp;
}


std::any MiniCParser::ConstExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitConstExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::ConstExpContext* MiniCParser::constExp() {
  ConstExpContext *_localctx = _tracker.createInstance<ConstExpContext>(_ctx, getState());
  enterRule(_localctx, 70, MiniCParser::RuleConstExp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(342);
    addExp();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void MiniCParser::initialize() {
  ::antlr4::internal::call_once(minicParserOnceFlag, minicParserInitialize);
}
