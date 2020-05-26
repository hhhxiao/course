文件夹说明:
def 
  - token.h 定义符号数据结构
lex -词法分析器
  - LexicalAnalyzer.cc /.h 词法分析器实现
  - lexMain.cc 生成词法分析器可执行文件
  - lexer.exe 词法分析器可执行文件
  - output 结果生成目录
      -  test.dyd 词法分析器生成的二元式文件
      -  test.err 词法分析器生成的错误文件
parser -语法分析器
   - Grammar.txt 文法
   - item.h 定义变量和函数表项
   - Parser.cc/.h 语法分析器实现
   - parerMain.cc 生成语法分析器可执行文件
   - parser.exe 语法分析器可执行文件
   - output 结果生成目录
      -  test.dyd 词法分析器生成的二元式文件
      -  test.err 词法分析器生成的错误文件
      -  test.err2  语法分析器生成的错误文件
      -  test.var  语法分析器生成的变量表
      -  test.pro  语法分析器生成的过程表

注意:这里没有自定义输入流，源文件只能是主目录下的test.pas文件，
生成的文件在各自目录的output目录中，output目录请勿删除不然无法写入文件(内部文件可删，目录本身别删)

编译(需要g++支持,msvc需要自己创建工程):
g++ ./lex/LexicalAnalyzer.cc ./lex/lexMain.cc -o ./lex/lex.exe
g++ ./lex/LexicalAnalyzer.cc ./parser/Parser.cc ./parser/parserMain.cc -o ./parser/parser.exe
或直接双击主目录下的build.bat

运行:
双击lex或者parser内部的*.exe文件即可






