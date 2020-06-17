文件说明:
    CMakeLists.txt  cmake文件
    data_file.txt   迷宫数据文件(可按照格式自行更改)
    Maze.cpp        迷宫源代码文件
    Maze.h          迷宫头文件
    necessary.txt   需求
    svpng.inc       png图片编码(https://github.com/miloyip/svpng)
    test.cpp        测试代码源文件
    test            linux下的可执行文件
    test.exe        Windows 10下的可执行文件
    result.png      运行过程截图

运行可执行文件即可执行测试用例(用例的数据来源也就是可执行文件的参数,即data_file.txt),当然正确与否得自行判断.

如需自行编译:
    1. g++ test.cpp Maze.cpp -std=c++11 -o test
    2.  ./test data_file.txt

