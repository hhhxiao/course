//
// Created by agno3 on 9/28/19.
//

#ifndef AI_LAB_1_STATUS_H
#define AI_LAB_1_STATUS_H

#include <vector>
#include<algorithm>
#include <ostream>
#include <stack>
#include <set>

//船位置的枚举变量
enum class pos {
    left, right
};


//船实体
struct boat {
    int size; //船的容量
    int mission; //船上的传教士数量
    int common;  //船上的野人数量
    pos position;

    explicit boat(int s) : size(s), mission(0), common(0), position(pos::left) {}

    boat() = default;

    //获取船上人数
    int sum() { return mission + common; }

    bool operator==(const boat &rhs) const {
        return size == rhs.size &&
               mission == rhs.mission &&
               common == rhs.common &&
               position == rhs.position;
    }

    //下面是一些必要的运算符重载
    bool operator!=(const boat &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const boat &rhs) const {
        if (size < rhs.size)
            return true;
        if (rhs.size < size)
            return false;
        if (mission < rhs.mission)
            return true;
        if (rhs.mission < mission)
            return false;
        if (common < rhs.common)
            return true;
        if (rhs.common < common)
            return false;
        return position < rhs.position;
    }
};

//获取上船的可选项
/**
 * @param max_num_boat 船的容纳数量 
 * @param mission 该岸的传教士数量
 * @param common 该岸的野人数量
 * @return 返回船上人人员分布情况
 */
std::vector<std::pair<int, int>> get_all_opt(int max_num_boat, int mission, int common) {

    std::vector<std::pair<int, int>> opts;
    int min_n;
    for (int i = max_num_boat; i >= 1; --i) {
        if (i <= common)
            opts.emplace_back(0, i);//补上全部是野人的情况
        min_n = (i + 1) / 2;//最小可载的传教士数量
        //船上可载ｉ人的时候
        for (int j = i; j >= min_n; --j)
            if (j <= mission && i - j <= common //人数足够上船
                && mission - j >= common - (i - j))//上船后岸上的人不会被吃

                opts.emplace_back(j, i - j);
    }
    return opts;
}


//状态类
struct status {
    int mission_left; //左岸的传教士数目
    int mission_right; //右岸传教士数目
    int common_left; //左岸的野人数目
    int common_right; //右岸的野人数目
    boat b; //船

    status(int m_left, int m_right, int c_left, int c_right, int size) :
            mission_left(m_left),
            mission_right(m_right),
            common_left(c_left),
            common_right(c_right),
            b(size) {}

    status() = default;

    //必要的运算符重载
    bool operator==(const status &rhs) const {
        return mission_left == rhs.mission_left &&
               mission_right == rhs.mission_right &&
               common_left == rhs.common_left &&
               common_right == rhs.common_right &&
               b == rhs.b;
    }

    bool operator!=(const status &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const status &rhs) const {
        if (mission_left < rhs.mission_left)
            return true;
        if (rhs.mission_left < mission_left)
            return false;
        if (mission_right < rhs.mission_right)
            return true;
        if (rhs.mission_right < mission_right)
            return false;
        if (common_left < rhs.common_left)
            return true;
        if (rhs.common_left < common_left)
            return false;
        if (common_right < rhs.common_right)
            return true;
        if (rhs.common_right < common_right)
            return false;
        return b < rhs.b;
    }


    //获取某一侧河岸上的总人数(包括船上)
    int sum(pos p) {
        int sum = 0;
        if (p == pos::left) {
            sum += common_left + mission_left;
            if (b.position == pos::left)
                sum += b.sum();
        } else if (p == pos::right) {
            sum += common_right + mission_right;
            if (b.position == pos::right)
                sum += b.sum();
        }
        return sum;
    }

    //获取下一个状态(人员交互方案)
    std::vector<status> get_next_set() {
        std::vector<status> status_list;
        std::vector<std::pair<int, int>> opts;
        if (b.position == pos::left) {//若船在左边
            opts = get_all_opt(b.size,
                               mission_left + b.mission,
                               common_left + b.common);
            //更新岸上人员状态和船位置
            for (auto opt : opts) {
                //更新下一个状态
                status s(
                        mission_left + b.mission - opt.first,
                        mission_right,
                        common_left + b.common - opt.second,
                        common_right,
                        b.size
                );
                s.b.mission = opt.first;
                s.b.common = opt.second;
                s.b.position = pos::right;
                status_list.push_back(s);
            }
        } else if (b.position == pos::right) {//若船在右边
            opts = get_all_opt(b.size,
                               mission_right + b.mission,
                               common_right + b.common);
            for (auto opt : opts) {
                //更新下一个状态
                status s(
                        mission_left,
                        mission_right + b.mission - opt.first,
                        common_left,
                        common_right + b.common - opt.second,
                        b.size
                );
                s.b.mission = opt.first;
                s.b.common = opt.second;
                s.b.position = pos::left;
                status_list.push_back(s);
            }
        }
        return status_list;
    }


    //判断当前状态是不是最终状态
    bool is_end() {
        return mission_left == 0 && common_left == 0 && b.position == pos::right;
    }


    friend std::ostream &operator<<(std::ostream &os, const status &status) {
        std::string pos = status.b.position == pos::left ? "left side" : "right side";
        std::cout << "left side:(" << status.mission_left <<
                  "," << status.common_left <<
                  "),right side(" << status.mission_right <<
                  "," << status.common_right << ") boat status: " << pos <<
                  " (" << status.b.mission <<
                  "," << status.b.common << ")";
        return os;
    }


    //dfs从所有状态中进行搜索
    std::vector<status> find_path() {
        std::vector<status> s;
        std::set<status> visited;
        std::stack<status> stack;
        status next;
        visited.insert(*this);
        stack.push(*this);
        while (!(stack.empty() || stack.top().is_end())) {
            //从可选集合中选择一个状态
            //去右岸的船选择船上人尽量多的
            //去左岸选择人尽量少的
            //这个选择策略体现在@see get_next()函数里面
            //但是因为不是最短路径算法依旧不能保证是最快方案
            next = stack.top().get_next(visited);
            if (next == status(-1, -1, -1, -1, -1)) {
                stack.pop();
            } else {
                stack.push(next);
                visited.insert(next);
            }
        }
        //栈转vector
        while (!stack.empty()) {
            s.push_back(stack.top());
            stack.pop();
        }
        std::reverse(s.begin(), s.end());
        return s;
    }

    //选择策略
    //去右岸尽量选人船上,左岸尽量选船上人少的
    status get_next(std::set<status> visited) {

        std::vector<status> next_sets = get_next_set();

        //筛选出已没有走过的状态
        std::vector<status> next_set;
        for (auto i :next_sets)
            if (visited.find(i) == visited.end())
                next_set.push_back(i);

        if (next_set.empty())
            return {-1, -1, -1, -1, -1};

        //不为空就开始寻找最为合适的(贪心)
        status max = next_set[0];
        status min = next_set[1];

        for (auto i : next_set) {
            if (visited.find(i) == visited.end()) {
                //选取左岸人尽量少的转移方法(去左边的人尽量多)
                if (i.common_left + i.mission_left < min.common_left + min.mission_left)
                    min = i;
                //选取右岸人尽量多的转移方法(去右边人尽量少)
                if (i.common_right + i.mission_right > max.mission_right + max.common_right)
                    max = i;
            }
        }
        return this->b.position == pos::left ? min : max;
    }
};

#endif //AI_LAB_1_STATUS_H
