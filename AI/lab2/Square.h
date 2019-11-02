//
// Created by agno3 on 10/13/19.
//

#ifndef AI_LAB_2_SQUARE_H
#define AI_LAB_2_SQUARE_H

#include <initializer_list>
#include <ostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

class Square {
private:
    int arr[3][3]{};

    std::pair<int, int> get_empty() const {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (arr[i][j] == 0)return std::make_pair(i, j);
        return std::make_pair(-1, -1);
    }

public:
    Square(const Square &rhs) {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                this->arr[i][j] = rhs.arr[i][j];
    }

    explicit Square(const int *array) {
        for (int i = 0; i < 9; ++i)
            arr[i / 3][i % 3] = array[i];
    }

    //h*(x) = W(x)启发函数
    //计算该状态的放错的数字的个数
    int get_num_wrong() const {
        int num = 0;
        for (int i = 0; i < 3; ++i)
            if (arr[0][i] != i + 1)++num;
        if (arr[1][0] != 8)++num;
        if (arr[1][2] != 4)++num;
        for (int j = 0; j < 3; ++j)
            if (arr[2][j] != 7 - j)++num;
        return num;
    }

    bool operator==(const Square &rhs) const {
        int num = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (arr[i][j] == rhs.arr[i][j])++num;
            }
        }
        return num == 9;
    }

    std::string to_string() const {
        std::string s;
        for (auto &i : arr) {
            for (int j : i) {
                s += (std::to_string(static_cast<char>(j) + '0'));
            }
        }
        return s;
    }

    bool operator<(const Square &rhs) const {
        return this->to_string() < rhs.to_string();
    }


    friend std::ostream &operator<<(std::ostream &os, const Square &square) {
        for (auto i : square.arr) {
            for (int j = 0; j < 3; ++j)
                os << i[j] << " ";
            os << "\n";
        }
        return os;
    }

    std::vector<Square> get_next() const {
        std::vector<Square> vec;
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};
        std::pair<int, int> pos = get_empty();

        int new_x, new_y;
        for (int i = 0; i < 4; ++i) {
            new_x = pos.first + dx[i];
            new_y = pos.second + dy[i];
            if (new_x >= 0 && new_x <= 2 && new_y >= 0 && new_y <= 2) {
                Square square(*this);
                square.arr[new_x][new_y] = this->arr[pos.first][pos.second];
                square.arr[pos.first][pos.second] = this->arr[new_x][new_y];
                vec.push_back(square);
            }
        }
        return vec;
    }


    static std::pair<Square, int> get_min(std::map<Square, int> &open) {
        auto iter = min_element(open.begin(), open.end(), [](pair<Square, int> p1, pair<Square, int> p2) {
            return p1.second < p2.second;
        });
        return *iter;
    }

    int h_star() const {
        return get_num_wrong();
    }


    vector<Square> a_star() {
        vector<Square> sq_set;
        map<Square, int> open_set;
        map<Square, int> closed_set;
        map<Square, Square> pointer_set;
        open_set.emplace(*this, this->h_star());
        while (!open_set.empty()) {
            pair<Square, int> best_node = get_min(open_set);
            open_set.erase(best_node.first);
            if (best_node.first.h_star() == 0)break;
            //删除选出的最小节点
            closed_set.insert(best_node);
            //获取后记节点
            vector<Square> succsors = best_node.first.get_next();
            for (auto &item:succsors) {
                auto iter_closed = closed_set.find(item);
                //不在closed中的点
                if (iter_closed == closed_set.end()) {
                    //看看有没有在开集合里面
                    auto iter_open = open_set.find(item);
                    if (iter_open == open_set.end()) {
                        //不在就是新的节点就加到开集合里面,增加父亲指针
                        int g_best = best_node.second - best_node.first.h_star();
                        open_set.emplace(item, g_best + 1 + item.h_star());
                        //第一个元素是子节点,第二个父亲节点
                        pointer_set.emplace(item, best_node.first);
                    } else {
                        //如果开集合里面已经有这个元素了,比较g值(当前成本)看是否需要调整父节点的指向
                        int new_g = best_node.second - best_node.first.h_star() + 1;
                        //这是走之前路线的成本
                        int old_g = iter_open->second - iter_open->first.h_star();
                        if (new_g < old_g) {
                            //更新节点
                            iter_open->second = iter_open->first.h_star() + new_g;
                            pointer_set.erase(iter_open->first);
                            pointer_set.emplace(iter_open->first, item);
                        }
                    }
                }
            }
        }
        if (open_set.empty()) {
            std::cout << "no path!!!!\n";
            return sq_set;
        }
        int array[9] = {1, 2, 3,
                        8, 0, 4,
                        7, 6, 5
        };
        Square s(array);
        sq_set.push_back(s);
        auto iter = pointer_set.find(s);
        while (iter != pointer_set.end()) {
            sq_set.push_back(iter->second);
            iter = pointer_set.find(iter->second);
        }
        std::reverse(sq_set.begin(), sq_set.end());
        return sq_set;
    }
};


#endif //AI_LAB_2_SQUARE_H