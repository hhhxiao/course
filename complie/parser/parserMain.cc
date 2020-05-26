//
// Created by xhy on 2020/4/25.
//
#include "Parser.h"
#define TRACE(fmt, ...)  printf(fmt, __VA_ARGS__)
static std::string readFileIntoString(const char *filename) {
  std::ifstream file(filename);
  std::string s;
  char ch;
  while (file.get(ch))
    s.push_back(ch);
  return s;
}

int main() {
  auto s = readFileIntoString("../test.pas");
  std::cout << s << "\n";
  xhy::LexicalAnalyzer analyzer(s);
  analyzer.lex();
  std::ofstream file("./output/test.dys");
  auto tokens = analyzer.getTokens();
  for (const auto &token:tokens) {
    file << token << "\n";
  }
  file.close();
  auto tokenList = analyzer.getTokenWithoutEOL();
  xhy::Parser parser(tokenList);
  std::cout << parser.parse();
  parser.print();
  return 0;
}


