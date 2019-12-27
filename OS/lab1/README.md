## 运行注意事项
- 运行`console.exe`是命令行交互模式，运行`runscript.exe`是运行脚本模式，脚本名称是`script.txt`,输出是`a.txt`

- 为方便起见**进程名字都只能用数字而不是字符**
- 所有命令如下：
  - `cr`
  - `de`
  - `to`
  - `rel`
  - `req`
  - `list`
  - `quit`

## 编译选项

- 手动编译直接`g++ main.cc `即可，默认输出是脚本模式，如果要编译成控制台模式需要编辑`main.cc`,注释掉`Shell s(script_name);`，加上`Shell s`即可。

