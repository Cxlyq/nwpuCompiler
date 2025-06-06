
// Generated from MiniC.g4 by ANTLR 4.12.0


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
      "constDef", "varDecl", "varDef", "initVal", "statement", "expr", "cond", 
      "lOrExp", "lAndExp", "eqExp", "eqOp", "relExp", "relOp", "addExp", 
      "addOp", "mulExp", "mulOp", "unaryExp", "unaryOp", "funcRParams", 
      "primaryExp", "lVal", "number"
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
      "T_SUB", "T_GE", "T_LE", "T_GREATER", "T_LESS", "T_EQ", "T_NEQ", "T_ASSIGN", 
      "T_COMMA", "T_AND", "T_OR", "T_RETURN", "T_INT", "T_FLOAT", "T_VOID", 
      "T_IF", "T_ELSE", "T_CONST", "T_WHILE", "T_BREAK", "T_CONTINUE", "T_INT_DIGIT", 
      "T_FLOAT_DIGIT", "T_ID", "WS", "LINE_COMMENT", "BLOCK_COMMENT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,39,345,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,1,0,1,0,5,0,71,8,
  	0,10,0,12,0,74,9,0,1,0,1,0,1,1,1,1,1,1,1,1,3,1,82,8,1,1,1,1,1,1,1,1,2,
  	1,2,1,3,1,3,1,3,5,3,92,8,3,10,3,12,3,95,9,3,1,4,1,4,1,4,1,4,3,4,101,8,
  	4,1,4,1,4,1,4,1,4,1,4,5,4,108,8,4,10,4,12,4,111,9,4,3,4,113,8,4,1,5,1,
  	5,3,5,117,8,5,1,5,1,5,1,6,4,6,122,8,6,11,6,12,6,123,1,7,1,7,3,7,128,8,
  	7,1,8,1,8,3,8,132,8,8,1,9,1,9,1,10,1,10,1,10,1,10,1,10,5,10,141,8,10,
  	10,10,12,10,144,9,10,1,10,1,10,1,11,1,11,1,11,1,11,1,11,5,11,153,8,11,
  	10,11,12,11,156,9,11,1,11,1,11,1,11,1,12,1,12,1,12,1,12,5,12,165,8,12,
  	10,12,12,12,168,9,12,1,12,1,12,1,13,1,13,1,13,1,13,1,13,5,13,177,8,13,
  	10,13,12,13,180,9,13,1,13,1,13,3,13,184,8,13,1,14,1,14,1,14,1,14,1,14,
  	5,14,191,8,14,10,14,12,14,194,9,14,3,14,196,8,14,1,14,3,14,199,8,14,1,
  	15,1,15,3,15,203,8,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,3,15,213,
  	8,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,3,15,223,8,15,1,15,1,15,
  	1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,3,15,235,8,15,1,16,1,16,1,17,
  	1,17,1,18,1,18,1,18,5,18,244,8,18,10,18,12,18,247,9,18,1,19,1,19,1,19,
  	5,19,252,8,19,10,19,12,19,255,9,19,1,20,1,20,1,20,1,20,5,20,261,8,20,
  	10,20,12,20,264,9,20,1,21,1,21,1,22,1,22,1,22,1,22,5,22,272,8,22,10,22,
  	12,22,275,9,22,1,23,1,23,1,24,1,24,1,24,1,24,5,24,283,8,24,10,24,12,24,
  	286,9,24,1,25,1,25,1,26,1,26,1,26,1,26,5,26,294,8,26,10,26,12,26,297,
  	9,26,1,27,1,27,1,28,5,28,302,8,28,10,28,12,28,305,9,28,1,28,1,28,1,29,
  	1,29,1,30,1,30,1,30,5,30,314,8,30,10,30,12,30,317,9,30,1,31,1,31,1,31,
  	1,31,1,31,1,31,1,31,1,31,1,31,3,31,328,8,31,1,31,3,31,331,8,31,1,32,1,
  	32,1,32,1,32,1,32,5,32,338,8,32,10,32,12,32,341,9,32,1,33,1,33,1,33,0,
  	0,34,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,
  	46,48,50,52,54,56,58,60,62,64,66,0,8,1,0,25,27,1,0,25,26,1,0,18,19,1,
  	0,14,17,1,0,12,13,1,0,9,11,2,0,8,8,12,13,1,0,34,35,352,0,72,1,0,0,0,2,
  	77,1,0,0,0,4,86,1,0,0,0,6,88,1,0,0,0,8,96,1,0,0,0,10,114,1,0,0,0,12,121,
  	1,0,0,0,14,127,1,0,0,0,16,131,1,0,0,0,18,133,1,0,0,0,20,135,1,0,0,0,22,
  	147,1,0,0,0,24,160,1,0,0,0,26,171,1,0,0,0,28,198,1,0,0,0,30,234,1,0,0,
  	0,32,236,1,0,0,0,34,238,1,0,0,0,36,240,1,0,0,0,38,248,1,0,0,0,40,256,
  	1,0,0,0,42,265,1,0,0,0,44,267,1,0,0,0,46,276,1,0,0,0,48,278,1,0,0,0,50,
  	287,1,0,0,0,52,289,1,0,0,0,54,298,1,0,0,0,56,303,1,0,0,0,58,308,1,0,0,
  	0,60,310,1,0,0,0,62,330,1,0,0,0,64,332,1,0,0,0,66,342,1,0,0,0,68,71,3,
  	2,1,0,69,71,3,16,8,0,70,68,1,0,0,0,70,69,1,0,0,0,71,74,1,0,0,0,72,70,
  	1,0,0,0,72,73,1,0,0,0,73,75,1,0,0,0,74,72,1,0,0,0,75,76,5,0,0,1,76,1,
  	1,0,0,0,77,78,3,4,2,0,78,79,5,36,0,0,79,81,5,4,0,0,80,82,3,6,3,0,81,80,
  	1,0,0,0,81,82,1,0,0,0,82,83,1,0,0,0,83,84,5,5,0,0,84,85,3,10,5,0,85,3,
  	1,0,0,0,86,87,7,0,0,0,87,5,1,0,0,0,88,93,3,8,4,0,89,90,5,21,0,0,90,92,
  	3,8,4,0,91,89,1,0,0,0,92,95,1,0,0,0,93,91,1,0,0,0,93,94,1,0,0,0,94,7,
  	1,0,0,0,95,93,1,0,0,0,96,97,3,18,9,0,97,112,5,36,0,0,98,100,5,6,0,0,99,
  	101,3,32,16,0,100,99,1,0,0,0,100,101,1,0,0,0,101,102,1,0,0,0,102,109,
  	5,7,0,0,103,104,5,6,0,0,104,105,3,32,16,0,105,106,5,7,0,0,106,108,1,0,
  	0,0,107,103,1,0,0,0,108,111,1,0,0,0,109,107,1,0,0,0,109,110,1,0,0,0,110,
  	113,1,0,0,0,111,109,1,0,0,0,112,98,1,0,0,0,112,113,1,0,0,0,113,9,1,0,
  	0,0,114,116,5,2,0,0,115,117,3,12,6,0,116,115,1,0,0,0,116,117,1,0,0,0,
  	117,118,1,0,0,0,118,119,5,3,0,0,119,11,1,0,0,0,120,122,3,14,7,0,121,120,
  	1,0,0,0,122,123,1,0,0,0,123,121,1,0,0,0,123,124,1,0,0,0,124,13,1,0,0,
  	0,125,128,3,30,15,0,126,128,3,16,8,0,127,125,1,0,0,0,127,126,1,0,0,0,
  	128,15,1,0,0,0,129,132,3,20,10,0,130,132,3,24,12,0,131,129,1,0,0,0,131,
  	130,1,0,0,0,132,17,1,0,0,0,133,134,7,1,0,0,134,19,1,0,0,0,135,136,5,30,
  	0,0,136,137,3,18,9,0,137,142,3,22,11,0,138,139,5,21,0,0,139,141,3,22,
  	11,0,140,138,1,0,0,0,141,144,1,0,0,0,142,140,1,0,0,0,142,143,1,0,0,0,
  	143,145,1,0,0,0,144,142,1,0,0,0,145,146,5,1,0,0,146,21,1,0,0,0,147,154,
  	5,36,0,0,148,149,5,6,0,0,149,150,3,32,16,0,150,151,5,7,0,0,151,153,1,
  	0,0,0,152,148,1,0,0,0,153,156,1,0,0,0,154,152,1,0,0,0,154,155,1,0,0,0,
  	155,157,1,0,0,0,156,154,1,0,0,0,157,158,5,20,0,0,158,159,3,28,14,0,159,
  	23,1,0,0,0,160,161,3,18,9,0,161,166,3,26,13,0,162,163,5,21,0,0,163,165,
  	3,26,13,0,164,162,1,0,0,0,165,168,1,0,0,0,166,164,1,0,0,0,166,167,1,0,
  	0,0,167,169,1,0,0,0,168,166,1,0,0,0,169,170,5,1,0,0,170,25,1,0,0,0,171,
  	178,5,36,0,0,172,173,5,6,0,0,173,174,3,32,16,0,174,175,5,7,0,0,175,177,
  	1,0,0,0,176,172,1,0,0,0,177,180,1,0,0,0,178,176,1,0,0,0,178,179,1,0,0,
  	0,179,183,1,0,0,0,180,178,1,0,0,0,181,182,5,20,0,0,182,184,3,28,14,0,
  	183,181,1,0,0,0,183,184,1,0,0,0,184,27,1,0,0,0,185,199,3,32,16,0,186,
  	195,5,2,0,0,187,192,3,28,14,0,188,189,5,21,0,0,189,191,3,28,14,0,190,
  	188,1,0,0,0,191,194,1,0,0,0,192,190,1,0,0,0,192,193,1,0,0,0,193,196,1,
  	0,0,0,194,192,1,0,0,0,195,187,1,0,0,0,195,196,1,0,0,0,196,197,1,0,0,0,
  	197,199,5,3,0,0,198,185,1,0,0,0,198,186,1,0,0,0,199,29,1,0,0,0,200,202,
  	5,24,0,0,201,203,3,32,16,0,202,201,1,0,0,0,202,203,1,0,0,0,203,204,1,
  	0,0,0,204,235,5,1,0,0,205,206,3,64,32,0,206,207,5,20,0,0,207,208,3,32,
  	16,0,208,209,5,1,0,0,209,235,1,0,0,0,210,235,3,10,5,0,211,213,3,32,16,
  	0,212,211,1,0,0,0,212,213,1,0,0,0,213,214,1,0,0,0,214,235,5,1,0,0,215,
  	216,5,28,0,0,216,217,5,4,0,0,217,218,3,34,17,0,218,219,5,5,0,0,219,222,
  	3,30,15,0,220,221,5,29,0,0,221,223,3,30,15,0,222,220,1,0,0,0,222,223,
  	1,0,0,0,223,235,1,0,0,0,224,225,5,31,0,0,225,226,5,4,0,0,226,227,3,34,
  	17,0,227,228,5,5,0,0,228,229,3,30,15,0,229,235,1,0,0,0,230,231,5,32,0,
  	0,231,235,5,1,0,0,232,233,5,33,0,0,233,235,5,1,0,0,234,200,1,0,0,0,234,
  	205,1,0,0,0,234,210,1,0,0,0,234,212,1,0,0,0,234,215,1,0,0,0,234,224,1,
  	0,0,0,234,230,1,0,0,0,234,232,1,0,0,0,235,31,1,0,0,0,236,237,3,34,17,
  	0,237,33,1,0,0,0,238,239,3,36,18,0,239,35,1,0,0,0,240,245,3,38,19,0,241,
  	242,5,23,0,0,242,244,3,38,19,0,243,241,1,0,0,0,244,247,1,0,0,0,245,243,
  	1,0,0,0,245,246,1,0,0,0,246,37,1,0,0,0,247,245,1,0,0,0,248,253,3,40,20,
  	0,249,250,5,22,0,0,250,252,3,40,20,0,251,249,1,0,0,0,252,255,1,0,0,0,
  	253,251,1,0,0,0,253,254,1,0,0,0,254,39,1,0,0,0,255,253,1,0,0,0,256,262,
  	3,44,22,0,257,258,3,42,21,0,258,259,3,44,22,0,259,261,1,0,0,0,260,257,
  	1,0,0,0,261,264,1,0,0,0,262,260,1,0,0,0,262,263,1,0,0,0,263,41,1,0,0,
  	0,264,262,1,0,0,0,265,266,7,2,0,0,266,43,1,0,0,0,267,273,3,48,24,0,268,
  	269,3,46,23,0,269,270,3,48,24,0,270,272,1,0,0,0,271,268,1,0,0,0,272,275,
  	1,0,0,0,273,271,1,0,0,0,273,274,1,0,0,0,274,45,1,0,0,0,275,273,1,0,0,
  	0,276,277,7,3,0,0,277,47,1,0,0,0,278,284,3,52,26,0,279,280,3,50,25,0,
  	280,281,3,52,26,0,281,283,1,0,0,0,282,279,1,0,0,0,283,286,1,0,0,0,284,
  	282,1,0,0,0,284,285,1,0,0,0,285,49,1,0,0,0,286,284,1,0,0,0,287,288,7,
  	4,0,0,288,51,1,0,0,0,289,295,3,56,28,0,290,291,3,54,27,0,291,292,3,56,
  	28,0,292,294,1,0,0,0,293,290,1,0,0,0,294,297,1,0,0,0,295,293,1,0,0,0,
  	295,296,1,0,0,0,296,53,1,0,0,0,297,295,1,0,0,0,298,299,7,5,0,0,299,55,
  	1,0,0,0,300,302,3,58,29,0,301,300,1,0,0,0,302,305,1,0,0,0,303,301,1,0,
  	0,0,303,304,1,0,0,0,304,306,1,0,0,0,305,303,1,0,0,0,306,307,3,62,31,0,
  	307,57,1,0,0,0,308,309,7,6,0,0,309,59,1,0,0,0,310,315,3,32,16,0,311,312,
  	5,21,0,0,312,314,3,32,16,0,313,311,1,0,0,0,314,317,1,0,0,0,315,313,1,
  	0,0,0,315,316,1,0,0,0,316,61,1,0,0,0,317,315,1,0,0,0,318,319,5,4,0,0,
  	319,320,3,32,16,0,320,321,5,5,0,0,321,331,1,0,0,0,322,331,3,64,32,0,323,
  	331,3,66,33,0,324,325,5,36,0,0,325,327,5,4,0,0,326,328,3,60,30,0,327,
  	326,1,0,0,0,327,328,1,0,0,0,328,329,1,0,0,0,329,331,5,5,0,0,330,318,1,
  	0,0,0,330,322,1,0,0,0,330,323,1,0,0,0,330,324,1,0,0,0,331,63,1,0,0,0,
  	332,339,5,36,0,0,333,334,5,6,0,0,334,335,3,32,16,0,335,336,5,7,0,0,336,
  	338,1,0,0,0,337,333,1,0,0,0,338,341,1,0,0,0,339,337,1,0,0,0,339,340,1,
  	0,0,0,340,65,1,0,0,0,341,339,1,0,0,0,342,343,7,7,0,0,343,67,1,0,0,0,34,
  	70,72,81,93,100,109,112,116,123,127,131,142,154,166,178,183,192,195,198,
  	202,212,222,234,245,253,262,273,284,295,303,315,327,330,339
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
    setState(72);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1308622848) != 0)) {
      setState(70);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
      case 1: {
        setState(68);
        funcDef();
        break;
      }

      case 2: {
        setState(69);
        decl();
        break;
      }

      default:
        break;
      }
      setState(74);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(75);
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
    setState(77);
    funcType();
    setState(78);
    match(MiniCParser::T_ID);
    setState(79);
    match(MiniCParser::T_L_PAREN);
    setState(81);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_INT

    || _la == MiniCParser::T_FLOAT) {
      setState(80);
      funcFParams();
    }
    setState(83);
    match(MiniCParser::T_R_PAREN);
    setState(84);
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

tree::TerminalNode* MiniCParser::FuncTypeContext::T_INT() {
  return getToken(MiniCParser::T_INT, 0);
}

tree::TerminalNode* MiniCParser::FuncTypeContext::T_FLOAT() {
  return getToken(MiniCParser::T_FLOAT, 0);
}

tree::TerminalNode* MiniCParser::FuncTypeContext::T_VOID() {
  return getToken(MiniCParser::T_VOID, 0);
}


size_t MiniCParser::FuncTypeContext::getRuleIndex() const {
  return MiniCParser::RuleFuncType;
}


std::any MiniCParser::FuncTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitFuncType(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::FuncTypeContext* MiniCParser::funcType() {
  FuncTypeContext *_localctx = _tracker.createInstance<FuncTypeContext>(_ctx, getState());
  enterRule(_localctx, 4, MiniCParser::RuleFuncType);
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
    setState(86);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 234881024) != 0))) {
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
    setState(88);
    funcFParam();
    setState(93);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_COMMA) {
      setState(89);
      match(MiniCParser::T_COMMA);
      setState(90);
      funcFParam();
      setState(95);
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
    setState(96);
    basicType();
    setState(97);
    match(MiniCParser::T_ID);
    setState(112);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_L_SQBRA) {
      setState(98);
      match(MiniCParser::T_L_SQBRA);
      setState(100);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 120259096848) != 0)) {
        setState(99);
        expr();
      }
      setState(102);
      match(MiniCParser::T_R_SQBRA);
      setState(109);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MiniCParser::T_L_SQBRA) {
        setState(103);
        match(MiniCParser::T_L_SQBRA);
        setState(104);
        expr();
        setState(105);
        match(MiniCParser::T_R_SQBRA);
        setState(111);
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
    setState(114);
    match(MiniCParser::T_L_BRACE);
    setState(116);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 136751100182) != 0)) {
      setState(115);
      blockItemList();
    }
    setState(118);
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
    setState(121); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(120);
      blockItem();
      setState(123); 
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
    setState(127);
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
      case MiniCParser::T_INT_DIGIT:
      case MiniCParser::T_FLOAT_DIGIT:
      case MiniCParser::T_ID: {
        enterOuterAlt(_localctx, 1);
        setState(125);
        statement();
        break;
      }

      case MiniCParser::T_INT:
      case MiniCParser::T_FLOAT:
      case MiniCParser::T_CONST: {
        enterOuterAlt(_localctx, 2);
        setState(126);
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
    setState(131);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniCParser::T_CONST: {
        enterOuterAlt(_localctx, 1);
        setState(129);
        constDecl();
        break;
      }

      case MiniCParser::T_INT:
      case MiniCParser::T_FLOAT: {
        enterOuterAlt(_localctx, 2);
        setState(130);
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
    setState(133);
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
    setState(135);
    match(MiniCParser::T_CONST);
    setState(136);
    basicType();
    setState(137);
    constDef();
    setState(142);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_COMMA) {
      setState(138);
      match(MiniCParser::T_COMMA);
      setState(139);
      constDef();
      setState(144);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(145);
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

tree::TerminalNode* MiniCParser::ConstDefContext::T_ASSIGN() {
  return getToken(MiniCParser::T_ASSIGN, 0);
}

MiniCParser::InitValContext* MiniCParser::ConstDefContext::initVal() {
  return getRuleContext<MiniCParser::InitValContext>(0);
}

std::vector<tree::TerminalNode *> MiniCParser::ConstDefContext::T_L_SQBRA() {
  return getTokens(MiniCParser::T_L_SQBRA);
}

tree::TerminalNode* MiniCParser::ConstDefContext::T_L_SQBRA(size_t i) {
  return getToken(MiniCParser::T_L_SQBRA, i);
}

std::vector<MiniCParser::ExprContext *> MiniCParser::ConstDefContext::expr() {
  return getRuleContexts<MiniCParser::ExprContext>();
}

MiniCParser::ExprContext* MiniCParser::ConstDefContext::expr(size_t i) {
  return getRuleContext<MiniCParser::ExprContext>(i);
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
    setState(147);
    match(MiniCParser::T_ID);
    setState(154);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_L_SQBRA) {
      setState(148);
      match(MiniCParser::T_L_SQBRA);
      setState(149);
      expr();
      setState(150);
      match(MiniCParser::T_R_SQBRA);
      setState(156);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(157);
    match(MiniCParser::T_ASSIGN);
    setState(158);
    initVal();
   
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
  enterRule(_localctx, 24, MiniCParser::RuleVarDecl);
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
    setState(160);
    basicType();
    setState(161);
    varDef();
    setState(166);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_COMMA) {
      setState(162);
      match(MiniCParser::T_COMMA);
      setState(163);
      varDef();
      setState(168);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(169);
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

std::vector<MiniCParser::ExprContext *> MiniCParser::VarDefContext::expr() {
  return getRuleContexts<MiniCParser::ExprContext>();
}

MiniCParser::ExprContext* MiniCParser::VarDefContext::expr(size_t i) {
  return getRuleContext<MiniCParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::VarDefContext::T_R_SQBRA() {
  return getTokens(MiniCParser::T_R_SQBRA);
}

tree::TerminalNode* MiniCParser::VarDefContext::T_R_SQBRA(size_t i) {
  return getToken(MiniCParser::T_R_SQBRA, i);
}

tree::TerminalNode* MiniCParser::VarDefContext::T_ASSIGN() {
  return getToken(MiniCParser::T_ASSIGN, 0);
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
  enterRule(_localctx, 26, MiniCParser::RuleVarDef);
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
    setState(171);
    match(MiniCParser::T_ID);
    setState(178);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_L_SQBRA) {
      setState(172);
      match(MiniCParser::T_L_SQBRA);
      setState(173);
      expr();
      setState(174);
      match(MiniCParser::T_R_SQBRA);
      setState(180);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(183);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_ASSIGN) {
      setState(181);
      match(MiniCParser::T_ASSIGN);
      setState(182);
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
  enterRule(_localctx, 28, MiniCParser::RuleInitVal);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(198);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniCParser::T_L_PAREN:
      case MiniCParser::T_NOT:
      case MiniCParser::T_ADD:
      case MiniCParser::T_SUB:
      case MiniCParser::T_INT_DIGIT:
      case MiniCParser::T_FLOAT_DIGIT:
      case MiniCParser::T_ID: {
        _localctx = _tracker.createInstance<MiniCParser::SingleValContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(185);
        expr();
        break;
      }

      case MiniCParser::T_L_BRACE: {
        _localctx = _tracker.createInstance<MiniCParser::MultiValContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(186);
        match(MiniCParser::T_L_BRACE);
        setState(195);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 120259096852) != 0)) {
          setState(187);
          initVal();
          setState(192);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == MiniCParser::T_COMMA) {
            setState(188);
            match(MiniCParser::T_COMMA);
            setState(189);
            initVal();
            setState(194);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
        }
        setState(197);
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
//----------------- BreakStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::BreakStatementContext::T_BREAK() {
  return getToken(MiniCParser::T_BREAK, 0);
}

tree::TerminalNode* MiniCParser::BreakStatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::BreakStatementContext::BreakStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::BreakStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBreakStatement(this);
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
//----------------- ContinueStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::ContinueStatementContext::T_CONTINUE() {
  return getToken(MiniCParser::T_CONTINUE, 0);
}

tree::TerminalNode* MiniCParser::ContinueStatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::ContinueStatementContext::ContinueStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::ContinueStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitContinueStatement(this);
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
  enterRule(_localctx, 30, MiniCParser::RuleStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(234);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MiniCParser::ReturnStatementContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(200);
      match(MiniCParser::T_RETURN);
      setState(202);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 120259096848) != 0)) {
        setState(201);
        expr();
      }
      setState(204);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MiniCParser::AssignStatementContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(205);
      lVal();
      setState(206);
      match(MiniCParser::T_ASSIGN);
      setState(207);
      expr();
      setState(208);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MiniCParser::BlockStatementContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(210);
      block();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MiniCParser::ExpressionStatementContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(212);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 120259096848) != 0)) {
        setState(211);
        expr();
      }
      setState(214);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<MiniCParser::IfelseStatementContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(215);
      match(MiniCParser::T_IF);
      setState(216);
      match(MiniCParser::T_L_PAREN);
      setState(217);
      cond();
      setState(218);
      match(MiniCParser::T_R_PAREN);
      setState(219);
      statement();
      setState(222);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
      case 1: {
        setState(220);
        match(MiniCParser::T_ELSE);
        setState(221);
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
      setState(224);
      match(MiniCParser::T_WHILE);
      setState(225);
      match(MiniCParser::T_L_PAREN);
      setState(226);
      cond();
      setState(227);
      match(MiniCParser::T_R_PAREN);
      setState(228);
      statement();
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<MiniCParser::BreakStatementContext>(_localctx);
      enterOuterAlt(_localctx, 7);
      setState(230);
      match(MiniCParser::T_BREAK);
      setState(231);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<MiniCParser::ContinueStatementContext>(_localctx);
      enterOuterAlt(_localctx, 8);
      setState(232);
      match(MiniCParser::T_CONTINUE);
      setState(233);
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
  enterRule(_localctx, 32, MiniCParser::RuleExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(236);
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
  enterRule(_localctx, 34, MiniCParser::RuleCond);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(238);
    lOrExp();
   
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

std::vector<MiniCParser::LAndExpContext *> MiniCParser::LOrExpContext::lAndExp() {
  return getRuleContexts<MiniCParser::LAndExpContext>();
}

MiniCParser::LAndExpContext* MiniCParser::LOrExpContext::lAndExp(size_t i) {
  return getRuleContext<MiniCParser::LAndExpContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::LOrExpContext::T_OR() {
  return getTokens(MiniCParser::T_OR);
}

tree::TerminalNode* MiniCParser::LOrExpContext::T_OR(size_t i) {
  return getToken(MiniCParser::T_OR, i);
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
  enterRule(_localctx, 36, MiniCParser::RuleLOrExp);
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
    setState(240);
    lAndExp();
    setState(245);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_OR) {
      setState(241);
      match(MiniCParser::T_OR);
      setState(242);
      lAndExp();
      setState(247);
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

//----------------- LAndExpContext ------------------------------------------------------------------

MiniCParser::LAndExpContext::LAndExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::EqExpContext *> MiniCParser::LAndExpContext::eqExp() {
  return getRuleContexts<MiniCParser::EqExpContext>();
}

MiniCParser::EqExpContext* MiniCParser::LAndExpContext::eqExp(size_t i) {
  return getRuleContext<MiniCParser::EqExpContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::LAndExpContext::T_AND() {
  return getTokens(MiniCParser::T_AND);
}

tree::TerminalNode* MiniCParser::LAndExpContext::T_AND(size_t i) {
  return getToken(MiniCParser::T_AND, i);
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
  enterRule(_localctx, 38, MiniCParser::RuleLAndExp);
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
    setState(248);
    eqExp();
    setState(253);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_AND) {
      setState(249);
      match(MiniCParser::T_AND);
      setState(250);
      eqExp();
      setState(255);
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

//----------------- EqExpContext ------------------------------------------------------------------

MiniCParser::EqExpContext::EqExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::RelExpContext *> MiniCParser::EqExpContext::relExp() {
  return getRuleContexts<MiniCParser::RelExpContext>();
}

MiniCParser::RelExpContext* MiniCParser::EqExpContext::relExp(size_t i) {
  return getRuleContext<MiniCParser::RelExpContext>(i);
}

std::vector<MiniCParser::EqOpContext *> MiniCParser::EqExpContext::eqOp() {
  return getRuleContexts<MiniCParser::EqOpContext>();
}

MiniCParser::EqOpContext* MiniCParser::EqExpContext::eqOp(size_t i) {
  return getRuleContext<MiniCParser::EqOpContext>(i);
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
  enterRule(_localctx, 40, MiniCParser::RuleEqExp);
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
    setState(256);
    relExp();
    setState(262);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_EQ

    || _la == MiniCParser::T_NEQ) {
      setState(257);
      eqOp();
      setState(258);
      relExp();
      setState(264);
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

//----------------- EqOpContext ------------------------------------------------------------------

MiniCParser::EqOpContext::EqOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::EqOpContext::T_EQ() {
  return getToken(MiniCParser::T_EQ, 0);
}

tree::TerminalNode* MiniCParser::EqOpContext::T_NEQ() {
  return getToken(MiniCParser::T_NEQ, 0);
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
  enterRule(_localctx, 42, MiniCParser::RuleEqOp);
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
    setState(265);
    _la = _input->LA(1);
    if (!(_la == MiniCParser::T_EQ

    || _la == MiniCParser::T_NEQ)) {
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

std::vector<MiniCParser::AddExpContext *> MiniCParser::RelExpContext::addExp() {
  return getRuleContexts<MiniCParser::AddExpContext>();
}

MiniCParser::AddExpContext* MiniCParser::RelExpContext::addExp(size_t i) {
  return getRuleContext<MiniCParser::AddExpContext>(i);
}

std::vector<MiniCParser::RelOpContext *> MiniCParser::RelExpContext::relOp() {
  return getRuleContexts<MiniCParser::RelOpContext>();
}

MiniCParser::RelOpContext* MiniCParser::RelExpContext::relOp(size_t i) {
  return getRuleContext<MiniCParser::RelOpContext>(i);
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
  enterRule(_localctx, 44, MiniCParser::RuleRelExp);
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
    setState(267);
    addExp();
    setState(273);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 245760) != 0)) {
      setState(268);
      relOp();
      setState(269);
      addExp();
      setState(275);
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
  enterRule(_localctx, 46, MiniCParser::RuleRelOp);
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

//----------------- AddExpContext ------------------------------------------------------------------

MiniCParser::AddExpContext::AddExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::MulExpContext *> MiniCParser::AddExpContext::mulExp() {
  return getRuleContexts<MiniCParser::MulExpContext>();
}

MiniCParser::MulExpContext* MiniCParser::AddExpContext::mulExp(size_t i) {
  return getRuleContext<MiniCParser::MulExpContext>(i);
}

std::vector<MiniCParser::AddOpContext *> MiniCParser::AddExpContext::addOp() {
  return getRuleContexts<MiniCParser::AddOpContext>();
}

MiniCParser::AddOpContext* MiniCParser::AddExpContext::addOp(size_t i) {
  return getRuleContext<MiniCParser::AddOpContext>(i);
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
  enterRule(_localctx, 48, MiniCParser::RuleAddExp);
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
    setState(278);
    mulExp();
    setState(284);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_ADD

    || _la == MiniCParser::T_SUB) {
      setState(279);
      addOp();
      setState(280);
      mulExp();
      setState(286);
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
  enterRule(_localctx, 50, MiniCParser::RuleAddOp);
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
    setState(287);
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

//----------------- MulExpContext ------------------------------------------------------------------

MiniCParser::MulExpContext::MulExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::UnaryExpContext *> MiniCParser::MulExpContext::unaryExp() {
  return getRuleContexts<MiniCParser::UnaryExpContext>();
}

MiniCParser::UnaryExpContext* MiniCParser::MulExpContext::unaryExp(size_t i) {
  return getRuleContext<MiniCParser::UnaryExpContext>(i);
}

std::vector<MiniCParser::MulOpContext *> MiniCParser::MulExpContext::mulOp() {
  return getRuleContexts<MiniCParser::MulOpContext>();
}

MiniCParser::MulOpContext* MiniCParser::MulExpContext::mulOp(size_t i) {
  return getRuleContext<MiniCParser::MulOpContext>(i);
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
  enterRule(_localctx, 52, MiniCParser::RuleMulExp);
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
    setState(289);
    unaryExp();
    setState(295);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3584) != 0)) {
      setState(290);
      mulOp();
      setState(291);
      unaryExp();
      setState(297);
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
  enterRule(_localctx, 54, MiniCParser::RuleMulOp);
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
    setState(298);
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

//----------------- UnaryExpContext ------------------------------------------------------------------

MiniCParser::UnaryExpContext::UnaryExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::PrimaryExpContext* MiniCParser::UnaryExpContext::primaryExp() {
  return getRuleContext<MiniCParser::PrimaryExpContext>(0);
}

std::vector<MiniCParser::UnaryOpContext *> MiniCParser::UnaryExpContext::unaryOp() {
  return getRuleContexts<MiniCParser::UnaryOpContext>();
}

MiniCParser::UnaryOpContext* MiniCParser::UnaryExpContext::unaryOp(size_t i) {
  return getRuleContext<MiniCParser::UnaryOpContext>(i);
}


size_t MiniCParser::UnaryExpContext::getRuleIndex() const {
  return MiniCParser::RuleUnaryExp;
}


std::any MiniCParser::UnaryExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitUnaryExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::UnaryExpContext* MiniCParser::unaryExp() {
  UnaryExpContext *_localctx = _tracker.createInstance<UnaryExpContext>(_ctx, getState());
  enterRule(_localctx, 56, MiniCParser::RuleUnaryExp);
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
    setState(303);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 12544) != 0)) {
      setState(300);
      unaryOp();
      setState(305);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(306);
    primaryExp();
   
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
  enterRule(_localctx, 58, MiniCParser::RuleUnaryOp);
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
  enterRule(_localctx, 60, MiniCParser::RuleFuncRParams);
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
    setState(310);
    expr();
    setState(315);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_COMMA) {
      setState(311);
      match(MiniCParser::T_COMMA);
      setState(312);
      expr();
      setState(317);
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


size_t MiniCParser::PrimaryExpContext::getRuleIndex() const {
  return MiniCParser::RulePrimaryExp;
}

void MiniCParser::PrimaryExpContext::copyFrom(PrimaryExpContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- LeftValueContext ------------------------------------------------------------------

MiniCParser::LValContext* MiniCParser::LeftValueContext::lVal() {
  return getRuleContext<MiniCParser::LValContext>(0);
}

MiniCParser::LeftValueContext::LeftValueContext(PrimaryExpContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::LeftValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitLeftValue(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BasicNumContext ------------------------------------------------------------------

MiniCParser::NumberContext* MiniCParser::BasicNumContext::number() {
  return getRuleContext<MiniCParser::NumberContext>(0);
}

MiniCParser::BasicNumContext::BasicNumContext(PrimaryExpContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::BasicNumContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBasicNum(this);
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

MiniCParser::FuncCallContext::FuncCallContext(PrimaryExpContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::FuncCallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitFuncCall(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ParenExprContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::ParenExprContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

MiniCParser::ExprContext* MiniCParser::ParenExprContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

tree::TerminalNode* MiniCParser::ParenExprContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

MiniCParser::ParenExprContext::ParenExprContext(PrimaryExpContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::ParenExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitParenExpr(this);
  else
    return visitor->visitChildren(this);
}
MiniCParser::PrimaryExpContext* MiniCParser::primaryExp() {
  PrimaryExpContext *_localctx = _tracker.createInstance<PrimaryExpContext>(_ctx, getState());
  enterRule(_localctx, 62, MiniCParser::RulePrimaryExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(330);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MiniCParser::ParenExprContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(318);
      match(MiniCParser::T_L_PAREN);
      setState(319);
      expr();
      setState(320);
      match(MiniCParser::T_R_PAREN);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MiniCParser::LeftValueContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(322);
      lVal();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MiniCParser::BasicNumContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(323);
      number();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MiniCParser::FuncCallContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(324);
      match(MiniCParser::T_ID);
      setState(325);
      match(MiniCParser::T_L_PAREN);
      setState(327);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 120259096848) != 0)) {
        setState(326);
        funcRParams();
      }
      setState(329);
      match(MiniCParser::T_R_PAREN);
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
  enterRule(_localctx, 64, MiniCParser::RuleLVal);
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
    match(MiniCParser::T_ID);
    setState(339);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_L_SQBRA) {
      setState(333);
      match(MiniCParser::T_L_SQBRA);
      setState(334);
      expr();
      setState(335);
      match(MiniCParser::T_R_SQBRA);
      setState(341);
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

//----------------- NumberContext ------------------------------------------------------------------

MiniCParser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::NumberContext::T_INT_DIGIT() {
  return getToken(MiniCParser::T_INT_DIGIT, 0);
}

tree::TerminalNode* MiniCParser::NumberContext::T_FLOAT_DIGIT() {
  return getToken(MiniCParser::T_FLOAT_DIGIT, 0);
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
  enterRule(_localctx, 66, MiniCParser::RuleNumber);
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
    setState(342);
    _la = _input->LA(1);
    if (!(_la == MiniCParser::T_INT_DIGIT

    || _la == MiniCParser::T_FLOAT_DIGIT)) {
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

void MiniCParser::initialize() {
  ::antlr4::internal::call_once(minicParserOnceFlag, minicParserInitialize);
}
