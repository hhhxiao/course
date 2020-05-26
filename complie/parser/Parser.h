//
// Created by xhy on 2020/4/25.
//

#ifndef COMPILER_PARSER_PARSER_H_
#define COMPILER_PARSER_PARSER_H_
#include <utility>
#include <ostream>
#include <stack>
#include <cstdarg>

#include "../lex/LexicalAnalyzer.h"
#include "../def/Token.h"
#include <ostream>
#include "item.h"
#define CALL (depth+1)
//#define DEBUG
/**
 * 采用递归下降分析法的词法分析器，该分析器的文法见grammar.txt
 * 每次判定token后会往后移动指针
 */
namespace xhy {
enum ParseStatus {
  FUNC_NAME,
  VAR_NAME,
  PARA_NAME,
  ID_USE,
  NONE,
};

class Parser {
 private:
  std::stack<FuncItem> functionStack;  //函数栈
  FuncItem currentFunction;  //当前函数,也就是栈顶函数
  ParseStatus status = NONE;  //一些标志位，方便解析的时候判错
  int index = 0; //全局计数器
  std::vector<FuncItem> funcTable;  //函数表
  std::vector<VarItem> varTable; //变量表
  std::vector<Token> tokenList; //token 列表
  int currentOffset = 0; //全局变量地址偏移
  std::string lastName; //成功解析的字符串的名字
  std::ofstream valFile, funFile, errFile; //三个错误流
 public:
  //static void debug(const std::string &msg);

  explicit Parser(std::vector<xhy::Token> tokens) : tokenList(std::move(tokens)) {
    currentFunction.fadr = 0;
    currentFunction.pname = "main";
    currentFunction.ptype = tokenType::FUNC;
    this->functionStack.emplace(currentFunction);
    initFileStream();
  }

  void print();
  bool parse() {
    auto result = StatementBlock();
    if (index != tokenList.size()) {
      error("uncompleted scope(function or expression)");
    }
    return result;
  }
  static const std::map<int, std::string> TYPE_TO_STR;

  ~Parser();

// private:
  void initFileStream();
  //两个辅助函数
  inline Token currentToken() { return tokenList[index]; }
  inline tokenType currentType() const { return tokenList[index].type; }
  //期望当前有一个type型的token,成功了和返回true
  bool tryMatch(tokenType type, bool err);
  void error(const char *msg) { errFile << "***LINE " << currentToken().getLine() << "  " << msg << "\n"; }
  //重复定义检查
  //变量重定义
  //函数重定义
  bool checkDupVarDef(const std::string &name, const FuncItem &fun);
  void checkDupFunDef(const std::string &funName);

  //ArithExp -> Item ArithExp'
  //下面五个是用来捕获一个算数表达式的
  bool ArithExp();
  bool ArithExp2();
  bool Item();
  bool Item2();
  bool FactorExp();
  //捕获一个id
  bool ID(int);
  bool PartFunctionCall();
  //执行语句
  bool Execution();
  bool ReadExp();
  bool WriteExp();
  bool AssignExp();
  bool ConditionExp();
  bool RelationOP();
  bool ConditionStat();
  bool ExecutionList();
  bool ExecutionList2();
  //描述语句
  bool Description();
  bool DescriptionList();
  bool DescriptionList2();
  bool PartFuncDescription();
  //语句块，也就是是完整程序
  bool StatementBlock();
  bool End();
  void checkIDUse(const std::string &id);
  bool FunctionBody();
};
}

#endif //COMPILER_PARSER_PARSER_H_
