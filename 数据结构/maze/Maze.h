//
// Created by agno3 on 9/19/19.
// Last modified : 10/2/19
//

#ifndef MAZE_MAZE_H
#define MAZE_MAZE_H

#include <string>
#include <cassert>
#include <fstream>
#include <iostream>
#include <stack>
#include <set>
#include <utility>
#include <vector>
#include <random>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include "svpng.inc"

#define WALL '1'
#define AIR '0'
#define  PATH '*'

//迷宫数据结构
class Maze {
private:
    //地图数据,用一个char数组存的没用vector
    std::vector<char> maze;
    //行
    int row;
    //列
    int col;
    //入口
    int in;
    //出口
    int out;

    static bool valid(int x, int y, int r, int c) {
        return (x >= 0 && x <= r - 1 && y >= 0 && y <= c - 1);
    }

    char index(int i, int j) const { return maze[i * col + j]; }

    //两个辅助函数
    static std::vector<int> get_opt_next(int &top, const std::vector<char> &vector, int row, int col);

    int get_next(int target, const bool *visited) const;

public:

    Maze() : maze(), row(-1), col(-1), in(-1), out(-1) {}

    Maze(std::vector<char> m, int r, int c, int i) : maze(std::move(m)), row(r), col(c), in(i), out(-1) {}

    void set_out(int o) { this->out = o; }

    friend std::istream &operator>>(std::istream &is, Maze &m);

    friend std::ostream &operator<<(std::ostream &os, const Maze &m);

    std::vector<int> find_path();

    static std::vector<char> generate(int row, int col, int start);

    static int get_random_int(int begin, int end) { return rand() % (end - begin + 1) + begin; }

    void show_maze() {
        for (int i = 0; i < row * col; ++i) {
            if (maze[i] == AIR) {
                std::cout << "  ";
            } else if (maze[i] == WALL) {
                std::cout << "##";
            } else if (maze[i] == PATH) {
                std::cout << "**";
            }
            if (i % col == col - 1)printf("\n");
        }
    }

    void save_png(const std::string &path);
};


#endif //MAZE_MAZE_H
