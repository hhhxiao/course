#include <iostream>
#include "Maze.h"
//输入和输出测试
void test_1(const std::string & file_name){
    std::cout<<"Test 1: IO for maze:\n";
    Maze maze;
    std::fstream  stream(file_name);
    stream >> maze;
    std::cout<<maze;
    std::cout<<"\nplease the check the output\n\n";
    //是否正确由人工观察结果
}


void test_2(const std::string & file_name){
    std::cout<<"Test 2: maze show:\n";
    Maze maze;
    std::fstream  stream( file_name);
    stream >> maze;
    maze.show_maze();
    std::cout<<"please check the output \n\n";
}
//寻路测试
void test_3(std::string  & file_name) {
    std::cout<<"Test 3: path find for maze:\nthe path is:";
    Maze maze;
    std::fstream  stream (file_name);
    stream >> maze;
    std::vector<int> path =  maze.find_path();
    for (auto i : path) 
        std::cout<<i<<" --> ";
    std::cout<<"out\nplease the check the output\n\n"; 
}




void test_4(){

    int r, c, in;
    std::cout<<"Test 4: maze generate:\n input row,col and in:\n";
    std::cin >>r >>c >>in;
    std::vector<char> maze = Maze::generate(r,c,in); 
    Maze m(maze,r,c,in);
    m.set_out(r * c -1);
    m.save_png("maze.png");
    m.find_path();
    m.save_png("solve.png");
    std::cout<<"please check the output[maze.png],[solve.png]\n";
}

int main(int argc,char *argv[]) {
    std::cout<<"please the check the output\n";
    if(argc == 1){
        std::cerr<<"Usage: [maze date file]\n";
        return -1;
    }
    std::string file_name(argv[1]);
    test_1(file_name);
    test_2(file_name);
    test_3(file_name);
    test_4();
    
    return 0;
}