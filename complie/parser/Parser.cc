//
// Created by xhy on 2020/4/25.
//


#include "Parser.h"
#include "item.h"
namespace xhy {
/*********************arithmetic expression********************************/
//ArithExp ->  Item ArithExp2
bool Parser::ArithExp() {
  if (!Item())return false;
  return ArithExp2();
}

//ArithExp2 -> - Item ArithExp2 | empty;
bool Parser::ArithExp2() {
  if (!tryMatch(tokenType::MINUS, false))return true;
  if (!Item())return false;
  return ArithExp2();
}

//Item -> Factor Item2
bool Parser::Item() {
  if (!FactorExp())return false;
  return Item2();
}

//Item2 -> * FactorExp Item2 | empty
bool Parser::Item2() {
  if (!tryMatch(tokenType::MUL, false))return true;
  if (!Item())return false;
  return Item2();
  //不是乘号，空串也能直接返回
}
//Factor -> Cons | ID PartFunction
/*
 * 优先匹配数字，没有数字就匹配变量
 * */
bool Parser::FactorExp() {
  if (tryMatch(tokenType::CONS, false))return true;
  if (!ID(true))return false;
  return PartFunctionCall();
}

bool Parser::PartFunctionCall() {
  if (!tryMatch(tokenType::L_PAR, false))return true;
  if (!ArithExp())return false;
  return tryMatch(tokenType::R_PAR, true);
}

/*********************Execution Expression*************************/
bool Parser::ExecutionList2() {
  if (!tryMatch(tokenType::SEM, false))return true;
  if (!Execution())return false;
  return ExecutionList2();
}

bool Parser::Execution() {
  if (WriteExp())return true;
  if (ReadExp())return true;
  if (AssignExp())return true;
  return ConditionExp();
}
bool Parser::ReadExp() {
  if (!tryMatch(tokenType::READ, false))return false;
  if (!tryMatch(tokenType::L_PAR, true))return false;
  if (!ID(1))return false;
  return tryMatch(tokenType::R_PAR, true);
}

//写语句 finish
bool Parser::WriteExp() {
  if (!tryMatch(tokenType::WRITE, false))return false;
  if (!tryMatch(tokenType::L_PAR, true))return false;
  if (!ID(1))return false;
  return tryMatch(tokenType::R_PAR, true);
}

//赋值语句 finish
bool Parser::AssignExp() {
  status = ID_USE;
  if (!ID(0))return false;
  status = NONE;
  if (!tryMatch(tokenType::ASSIGN, false))return false;
  return ArithExp();
}
//条件语句
bool Parser::ConditionExp() {
  if (!tryMatch(tokenType::IF, false))return false;
  if (!ConditionStat())return false;
  if (!tryMatch(tokenType::THEN, true))return false;
  if (!Execution())return false;
  if (!tryMatch(tokenType::ELSE, true))return false;
  return Execution();
}

//关系运算符
bool Parser::RelationOP() {
  if (tryMatch(tokenType::LT, false))return true;
  if (tryMatch(tokenType::GT, false))return true;
  if (tryMatch(tokenType::LE, false))return true;
  if (tryMatch(tokenType::GE, false))return true;
  if (tryMatch(tokenType::NE, false))return true;
  return tryMatch(tokenType::EQ, true);
}

//条件表达式
bool Parser::ConditionStat() {
  if (!ArithExp())return false;
  if (!RelationOP())return false;
  return ArithExp();
}
//执行语句列表
bool Parser::ExecutionList() {
  if (!Execution())return false;
  return ExecutionList2();
}

/********************Description Expression***********************/
//描述语句
bool Parser::Description() {
  //识别integer关键字
  if (!tryMatch(tokenType::INT, false))return false;
  //后面识别id(或者)不完整的函数声明
  status = VAR_NAME;
  if (ID(0)) {
    return true;
  }
  return PartFuncDescription();
}

//函数描述
bool Parser::PartFuncDescription() {
  if (!tryMatch(tokenType::FUNC, false))return false;
  status = FUNC_NAME;
  if (!ID(1))return false;
  if (!tryMatch(tokenType::L_PAR, false))return false;
  status = PARA_NAME;
  if (!ID(1))return false;
  if (!tryMatch(tokenType::R_PAR, false))return false;
  if (!tryMatch(tokenType::SEM, false))return false;
  return FunctionBody();
}

//finish
bool Parser::StatementBlock() {
  if (!tryMatch(tokenType::BEGIN, true))return false;
  if (!DescriptionList())return false;
  if (!tryMatch(tokenType::SEM, false))return false;
  if (!ExecutionList())return false;
  return End();
}

bool Parser::DescriptionList() {
  if (!Description())return false;
  return DescriptionList2();
}

//finish
bool Parser::DescriptionList2() {
  int idx = this->index;
  if (!tryMatch(tokenType::SEM, true))return true;
  //识别完成分号后识别下面的一个声明语句
  if (!Description()) {
    //识别失败要回退啊
    this->index = idx;
    return true;
  }
  return DescriptionList2();
}

bool Parser::FunctionBody() {
  return StatementBlock();
}

bool Parser::ID(int forceErrorOutput) {
  auto result = tryMatch(tokenType::ID, forceErrorOutput);
  if (result) {
    switch (status) {
      case VAR_NAME:
        //检查变量定义是否重复定义
        checkDupVarDef(lastName, functionStack.top());
        this->varTable.emplace_back(lastName,
                                    functionStack.top().pname,
                                    0,
                                    tokenType::INT,
                                    functionStack.size() - 1,
                                    currentOffset++);
        break;
      case PARA_NAME:
        //检查形参和变量重复定义
        if (checkDupVarDef(lastName, functionStack.top())) {
          this->varTable.emplace_back(lastName, functionStack.top().pname, 1, tokenType::INT,
                                      functionStack.size() - 1,
                                      currentOffset++);
        }
        break;
      case FUNC_NAME:
        //检查函数重复定义
        checkDupFunDef(lastName);
        currentFunction.pname = lastName;
        currentFunction.fadr = currentOffset;
        functionStack.emplace(currentFunction);
        break;
      case ID_USE:checkIDUse(lastName);
        break;
      default:break;
    }
  }
  status = NONE;
  return
      result;
}

bool Parser::End() {
  auto result = tryMatch(tokenType::END, false);
  if (result && !functionStack.empty()) {
    auto func = functionStack.top();
    functionStack.pop();
    func.ladr = currentOffset - 1;
    func.plev = functionStack.size();
    funcTable.emplace_back(func);
  }
  return result;
}

bool Parser::tryMatch(const tokenType type, bool err) {
  if (index >= tokenList.size())
    return false;
  //当前index没有越界 而且index
  if (currentType() == type) {
    lastName = tokenList[index].getName();
    std::cout << "match:  " << lastName << "\n";
    ++index;
    return true;
  } else {
    if (err) {
      errFile << "***LINE " << currentToken().getLine()
              << "  expected a symbol: '" << TYPE_TO_STR.at(type)
              << "' near '"
              << tokenList[index].getName()
              << "'\n";
    }
    return false;
  }
}

/***************error help map************************/
const std::map<int, std::string> Parser::TYPE_TO_STR = { //NOLINT
    {1, "keyword begin"}, {2, "keyword end"}, {3, "keyword integer"},
    {4, "keyword if"}, {5, "keyword then"}, {6, "keyword else"},
    {7, "keyword function"}, {8, "keyword read"}, {9, "keyword write"},
    {10, "identifier"}, {11, "number"},
    {12, "relation op(>/</=/<>/<=/>=)"},
    {13, " relation op(>/</=/<>/<=/>=)"},
    {14, "relation op(>/</=/<>/<=/>=)"},
    {15, "relation op(>/</=/<>/<=/>=)"},
    {16, "relation op(>/</=/<>/<=/>=)"},
    {17, "relation op(>/</=/<>/<=/>=)"},
    {18, "-"},
    {19, "*"}, {20, ":="},
    {21, "("}, {22, ")"}, {23, ";"}
};

/**************dup define or undefined check**************/
bool Parser::checkDupVarDef(const std::string &name, const FuncItem &fun) {
  for (const auto &item:varTable) {
    if (item.vproc == fun.pname && name == item.vname && item.vkind == 0) {
      errFile << "***LINE " << currentToken().getLine()
              << "  duplicate definition identifier '" << name << "'\n";
      return false;
    }
  }
  return true;
}

//同时检查函数栈和函数表
void Parser::checkDupFunDef(const std::string &funName) {
  for (const auto &f: funcTable) {
    std::cout << f.pname << "\n";
    if (funName == f.pname) {
      errFile << "***LINE " << currentToken().getLine()
              << "  duplicate definition function '" << funName << "'\n";
      return;
    }
  }
  auto stack = this->functionStack;
  while (!stack.empty()) {
    if (stack.top().pname == funName) {
      errFile << "***LINE " << currentToken().getLine()
              << "  duplicate definition function '" << funName << "'\n";;
      return;
    }
    stack.pop();
  }
}

void Parser::checkIDUse(const std::string &id) {
  for (const auto &func:funcTable) {
    if (func.pname == id)
      return;
  }
  for (const auto &var:varTable) {
    if (var.vname == id)
      return;
  }
  auto stack = this->functionStack;
  while (!stack.empty()) {
    if (stack.top().pname == id) {
      return;
    }
    stack.pop();
  }
  errFile << "***LINE " << currentToken().getLine() <<
          "  undefined function or identifier '" << id << "'\n";
}

/************************IO help function********************/
void Parser::initFileStream() {
  valFile.open("./output/test.var");
  funFile.open("./output/test.pro");
  errFile.open("./output/test.err2");
}

void Parser::print() {
  valFile << "vname    vtype    vadr  vproc     vlev     vkind\n";
  char buffer[256];
  for (const auto &item:varTable) {
    sprintf(buffer, "%-8s integer  %d     %-8s    %d     %d\n",
            item.vname.c_str(),
            item.vaddr,
            item.vproc.c_str(),
            item.vlev,
            item.vkind);
    valFile << buffer;
  }

  funFile << "pname      ptype    plev   fadr   ladr\n";
  for (const auto &i:funcTable) {
    sprintf(buffer, "%-8s  integer   %d      %d      %d\n", i.pname.c_str(), i.plev, i.fadr, i.ladr);
    funFile << buffer;
  }
}

Parser::~Parser() {
  valFile.close();
  funFile.close();
  errFile.close();
}

}
