//
// Created by agno3 on 9/19/19.
//

#include "Maze.h"

#define DEBUG


//IO
//从输入流中读取IO数据
std::istream &operator>>(std::istream &is, Maze &m) {
    is >> m.row >> m.col >> m.in >> m.out;
    m.maze.resize(m.row * m.col);
    char buffer[m.col + 1];
    is.getline(buffer, m.col + 1);
    for (int i = 0; i < m.row; ++i) {
        is.getline(buffer, m.col + 1);
        for (int j = 0; j < m.col; ++j) {
            m.maze[i * m.col + j] = buffer[j];
        }
    }
    return is;
}

//输出迷宫数据到输出流
std::ostream &operator<<(std::ostream &os, const Maze &m) {
    os << m.row << " " << m.col << " " << m.in << " " << m.out;
    for (int i = 0; i < m.col * m.row; ++i) {
        if (i % m.col == 0)os << "\n";
        os << m.maze[i];
    }
    os<<m.maze[m.row * m.col-1];
    return os;
}

/**
 * 路径查找，找到in－－> out的一条路线
 * @return 返回的是路径数组
 */
std::vector<int> Maze::find_path() {
    std::vector<int> path;
    assert(row > 0 && col > 0);
    bool visited[row * col];
    for (int i = 0; i < row * col; ++i)
        visited[i] = false;
    int next_point = -1;
    std::stack<int> stack;
    stack.push(this->in);
    visited[this->in] = true;
    while (!(stack.empty() || stack.top() == this->out)) {
        next_point = get_next(stack.top(), visited);
        if (next_point == -1)stack.pop();
        else {
            stack.push(next_point);
            visited[next_point] = true;
        }
    }
    while (!stack.empty()) {
        path.push_back(stack.top());
        maze[stack.top()] = PATH;
        stack.pop();
    }
     std::reverse(path.begin(),path.end()) ;
    return  path;
}

int Maze::get_next(int target, const bool *visited) const {
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};
    int new_x, new_y, pos;
    for (int i = 0; i < 4; ++i) {
        new_x = target / col + dx[i];
        new_y = target % col + dy[i];
        pos = new_x * col + new_y;
        //这里不会数组越界
        if (valid(new_x, new_y, row, col) && index(new_x, new_y) == AIR && !visited[pos])
            return pos;
    }
    return -1;
}


/**
 * 迷宫生成函数
 * @param row 迷宫的行数
 * @param col 迷宫的列数
 * @param start 迷宫入口位置
 * @return 迷宫数据的一维度数组
 */
std::vector<char> Maze::generate(int row, int col, int start) {

    std::vector<char> result(row * col, WALL);
    std::stack<int> stack;
    std::vector<int> opt;
    result[start] = AIR;
    stack.push(start);
    while (!stack.empty()) {
        opt = get_opt_next(stack.top(), result, row, col);
        if (opt.empty()) {
            stack.pop();
        } else {
            int index = get_random_int(0, (int) opt.size() - 1);
            stack.push(opt[index]);
            result[opt[index]] = AIR;
        }
        opt.clear();
    }
    return result;
}

std::vector<int> Maze::get_opt_next(int &top, const std::vector<char> &vector, int row, int col) {
    std::vector<int> opt;
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};
    int new_x, new_y, pos;
    int num = 0;
    int d_x, d_y, d_pos;
    for (int i = 0; i < 4; ++i) {
        new_x = top / col + dx[i];
        new_y = top % col + dy[i];
        pos = new_x * col + new_y;
        //这里不会数组越界
        if (valid(new_x, new_y, row, col) && vector[pos] == WALL) {
            //d对于每一个可能的下一个的点
            for (int j = 0; j < 4; ++j) {
                d_x = pos / col + dx[j];
                d_y = pos % col + dy[j];
                d_pos = d_x * col + d_y;
                if (valid(d_x, d_y, row, col) && d_pos != top && vector[d_pos] == AIR)num++;
            }
            if (num == 0)opt.push_back(pos);
            num = 0;
        }
    }
    return opt;
}

void Maze::save_png(const std::string & path) {
    if (row < 0 || col < 0) {
        std::cerr << "data error" << std::endl;
        return;
    }
    
    const  int line_width = 8;
    int width = col * line_width;
    int height = row * line_width;

    unsigned char rgb[width * height * 3], *p = rgb;
    unsigned x, y;
    FILE *fp = fopen(path.c_str(), "wb");
    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++) {
            switch (maze[y / line_width * col + x / line_width]) {
                case AIR:
                    *p++ = 255;    /* R */
                    *p++ = 255;    /* G */
                    *p++ = 255;
                    break;
                case PATH:
                    *p++ = 96;    /* R */
                    *p++ = 185;    /* G */
                    *p++ = 37;
                    break;
                case WALL:
                    *p++ = 0;    /* R */
                    *p++ = 0;    /* G */
                    *p++ = 0;
                    break;
                default:
                    break;
            }
            /* B */
        }
    svpng(fp, width, height, rgb, 0);
    fclose(fp);
    
}

