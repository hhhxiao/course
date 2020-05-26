//
// Created by xhy on 2020/4/25.
//
#include <iostream>
#include "../def/Token.h"
#include "../lex/LexicalAnalyzer.h"

static std::string readFileIntoString(const char *filename) {
  std::ifstream file(filename);
  std::string s;
  char ch;
  while (file.get(ch))
    s.push_back(ch);
  return s;
}

int main(int argc, char *argv[]) {
  auto s = readFileIntoString("../test.pas");
  std::cout<<s<<"\n";
  xhy::LexicalAnalyzer analyzer(s);
  analyzer.lex();
  return 0;
}
