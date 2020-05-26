//
// Created by xhy on 2020/4/25.
//

#ifndef COMPILER_LEX_LEXICALANALYZER_H_
#define COMPILER_LEX_LEXICALANALYZER_H_
#include <map>
#include "../def/Token.h"
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <ostream>
namespace xhy {
class LexicalAnalyzer {
 private:
  int status = 0;
  const std::string text; //text
  std::string tokenBuffer; //buffer
  std::ofstream tokenFile, errFile; //output stream
  int index = 0;//当前字符指针
  std::vector<Token> tokens; //tokenList
  bool end = false; //read end
  int line = 1; //line number


 public:
  typedef std::map<int, std::map<char, int>> fsmMap;
  static const fsmMap FSM_TABLE;
  static const std::map<int, Token::Type> STATUS_TOKEN_TYPE;
  static const std::map<std::string, Token::Type> KEY_WORD_LIST;
 public:
  explicit LexicalAnalyzer(std::string t) : text(std::move(t)) {
    end = text.empty();
    initOutputStream();
  }

  void lex();
  std::vector<Token> getTokens() const;
  std::vector<Token> getTokenWithoutEOL() {
    std::vector<Token> tokenList;
    for (const auto &token:tokens) {
      if (token.type != tokenType::EOL)
        tokenList.push_back(token);
    }
    return tokenList;
  }

  ~LexicalAnalyzer() {
    tokenFile.close();
    errFile.close();
  }

 private:
  void initOutputStream() {
    tokenFile.open("./output/test.dyd");
    errFile.open("./output/test.err");
  }

  // 根据当前字符栈的状态来构造token
  void tryCreateToken();
  // 根据当前指针进行状态转移
  void transfer();
  //清除状态
  inline void resetStatus() { status = 0; }
  // 尝试移动字符指针，不能移动就说明字符串读完了
  void tryAdvance() { index == text.size() - 1 ? end = true : ++index; }
};
}
#endif //COMPILER_LEX_LEXICALANALYZER_H_
