//
// Created by xhy on 2020/4/25.
//

#include "LexicalAnalyzer.h"
namespace xhy {

//关键字列表
const std::map<std::string, Token::Type> LexicalAnalyzer::KEY_WORD_LIST = { //NOLINT
    {"begin", tokenType::BEGIN},
    {"end", tokenType::END},
    {"integer", tokenType::INT},
    {"if", tokenType::IF},
    {"then", tokenType::THEN},
    {"else", tokenType::ELSE},
    {"function", tokenType::FUNC},
    {"read", tokenType::READ},
    {"write", tokenType::WRITE}
};
//终结状态和识别到的token的对应关系
const std::map<int, Token::Type> LexicalAnalyzer::STATUS_TOKEN_TYPE = { //NOLINT
    {1, tokenType::ID},
    {3, tokenType::CONS},
    {5, tokenType::EQ},
    {6, tokenType::MINUS},
    {7, tokenType::MUL},
    {8, tokenType::L_PAR},
    {9, tokenType::R_PAR},
    {10, tokenType::LT},
    {11, tokenType::LE},
    {12, tokenType::NE},
    {14, tokenType::GT},
    {15, tokenType::GE},
    {18, tokenType::ASSIGN},
    {20, tokenType::SEM},
    {21, tokenType::EOL}
};
/**
 * 这里可以自定义状态机的转移规则
 */
const LexicalAnalyzer::fsmMap LexicalAnalyzer::FSM_TABLE = { //NOLINT
    {0,
     {
         {'c', 1},
         {'n', 3},
         {' ', 0},
         {'=', 5},
         {'-', 6},
         {'*', 7},
         {'(', 8},
         {')', 9},
         {'<', 10},
         {'>', 14},
         {':', 17},
         {';', 20},
         {'\n', 21}
     }
    },
    {1,
     {
         {'c', 1},
         {'n', 1}
     }
    },
    {3,
     {
         {'n', 3},
     }
    },
    {10, {
        {'=', 11},
        {'>', 12}
    }
    },
    {14, {
        {'=', 15}
    }
    },
    {17,
     {
         {'=', 18}
     }
    },
};

void LexicalAnalyzer::tryCreateToken() {
  //寻找当前状态是否有下个状态
  auto result = STATUS_TOKEN_TYPE.find(status);
  //如果走到了死胡同
  if (result == STATUS_TOKEN_TYPE.end()) {
    //如果没找到直接报错类型待定
    if (status == 17) {
      errFile << "Missing = after ':' at line " << line << "\n";
    } else{
      std::cout<<"status:  "<<status<<"\n";
      errFile << "unresolved symbol '" << text[index] << "' at line " << line << "\n";
    }
    //忽略栈中的内容继续进行后续操作
    tryAdvance();
  } else {
    //这里可能有关键字
    auto tokenType = result->second;
    if (status == 1) {
      auto r = KEY_WORD_LIST.find(tokenBuffer);
      if (r != KEY_WORD_LIST.end())
        tokenType = r->second;
    }
    if (tokenBuffer == "\n") {
      tokenBuffer = "EOL";
      ++this->line;
    }
    if (tokenBuffer.size() < MAX_NAME_LENGTH) {
      tokens.emplace_back(tokenBuffer, tokenType, this->line);
      tokenFile << Token(tokenBuffer, tokenType, this->line) << "\n";
    } else {
      errFile << "token '" << tokenBuffer << "' is to long at line " << line << " (max: " << MAX_NAME_LENGTH << ")\n";
    }
  }
  resetStatus();//重置状态，进行下一个token的识别
  tokenBuffer.clear(); //清除字符栈的内容
}

void LexicalAnalyzer::transfer() {
  auto result = FSM_TABLE.find(status);
  if (result == FSM_TABLE.end()) {
    //如果找不到尝试生成token
    tryCreateToken();
    return;
  }
  //下面是找到的情况
  char c = text[index];
  //这里方便状态转移用n表示数字,c表示字母，硬编码问题不大
  if ('0' <= c && '9' >= c) {
    c = 'n';
  } else if ('a' <= c && 'z' >= c) {
    c = 'c';
  }
  //现在找当前状态在当前字符下有无下一个状态
  auto iter = FSM_TABLE.at(status).find(c);
  if (iter == FSM_TABLE.at(status).end()) {
    //也尝试结算
    tryCreateToken();
  } else {
    if (text[index] != ' ')
      tokenBuffer += text[index];//加入缓冲
    status = FSM_TABLE.at(status).at(c);//更新字符
    tryAdvance();//尝试移除指针
  }
}

void LexicalAnalyzer::lex() {
  while (!end) {
    transfer();
  }//没有结束的时候
  tryCreateToken();//规约剩余字符
}
std::vector<Token> LexicalAnalyzer::getTokens() const {
  return this->tokens;
}

}
