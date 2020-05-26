//
// Created by xhy on 2020/4/25.
//

#ifndef COMPILER_DEF_TOKEN_H_
#define COMPILER_DEF_TOKEN_H_
#include <string>
#include <iostream>
#include <utility>
#include <cassert>
#define MAX_NAME_LENGTH 16
namespace xhy {
class Token {
 public:
  enum Type {
    BEGIN = 1, END = 2, INT = 3, IF = 4, THEN = 5, ELSE = 6, FUNC = 7, READ = 8,
    WRITE = 9, ID = 10, CONS = 11, EQ = 12, NE = 13, LE = 14, LT = 15, GE = 16, GT = 17,
    MINUS = 18, MUL = 19, ASSIGN = 20, L_PAR = 21, R_PAR = 22, SEM = 23, EOL = 24
  };
  const Type type;
 private:
  const std::string name;
  const int lineNumber;
 public:
  Token(std::string name, Type type, int line) : name(std::move(name)), type(type), lineNumber(line) {}
  friend std::ostream &operator<<(std::ostream &os, const Token &token) {
    assert(token.name.length() <= MAX_NAME_LENGTH);
    os <<
       std::string(MAX_NAME_LENGTH - token.name.length(), ' ') <<
       token.name << " ";
    if (token.type < 10)
      os << "0";
    os << token.type;
    return os;
  }
  std::string getName() const { return this->name; }
  int getLine() { return lineNumber; }
};
typedef Token::Type tokenType;
}

#endif //COMPILER_DEF_TOKEN_H_
