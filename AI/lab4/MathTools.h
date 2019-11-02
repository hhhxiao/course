//
// Created by agno3 on 10/30/19.
//

#ifndef AI_LAB_4__MATHTOOLS_H_
#define AI_LAB_4__MATHTOOLS_H_
#include "TrainItem.h"
#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using namespace std;
/**
* 根据结果对训练集做划分,返回结果到样本集合的映射
* @param train_set 训练集合
* @return 结果到样本集的映射
*/
static map<int, vector<TrainItem>> DivideByResult(vector<TrainItem> &train_set) {
  map<int, vector<TrainItem>> divide_set;
  for (auto &item:train_set)
    divide_set[item.get_category()].push_back(item);
  return divide_set;
}

/**
 * 根据样本分布计算信息熵
 * @param sample 
 * @return 
 */
double InfoEnt(vector<TrainItem> &sample) {
  auto divide_set = DivideByResult(sample);
  double x = 0;
  double pos;
  for (auto &i:divide_set) {
    pos = static_cast<double >(i.second.size()) / sample.size();
    x += pos * log2(pos);
  }
  return -x;
}

/**
 * 计算特定连续的属性在特定分割点上的信息增益
 * @param train_set 样本集合
 * @param attr_index 属性位置
 * @param middle 分割点
 * @param ent 样本信息熵
 * @return 信息增益
 */
double InfoGain(vector<TrainItem> &train_set, int attr_index, double middle, double ent) {
  vector<TrainItem> less;
  vector<TrainItem> much;
  //按分割点分裂成俩
  for (auto &item:train_set) {
    if (item.get_attr(attr_index) > middle)
      much.push_back(item);
    else
      less.push_back(item);
  }
  double less_ent = InfoEnt(less);
  double less_p = static_cast<double>(less.size()) / train_set.size();
  double much_ent = InfoEnt(much);
  double much_p = static_cast<double>(much.size()) / train_set.size();
  return ent - less_ent * less_p - much_ent * much_p;
}

/**
 * 计算某个属性(非离散)的最大信息增益
 * @param train_set 训练集 
 * @param attr_index 属性标号
 * @return [最佳分割点,最大增益]
 */
pair<double, double> AttrInfoBestGain(vector<TrainItem> &train_set, int attr_index) {
  if (train_set.size() < 2)
    throw runtime_error("single train set");
  double ent = InfoEnt(train_set);
  sort(train_set.begin(), train_set.end(), [attr_index](const TrainItem &t1, const TrainItem &t2) {
    return t1.get_attr(attr_index) < t2.get_attr(attr_index);
  });

  double middle_init = (train_set[0].get_attr(attr_index) + train_set[1].get_attr(attr_index)) / 2;
  double ent_init = InfoGain(train_set, attr_index, middle_init, ent);
  for (size_t i = 0; i < train_set.size() - 1; ++i) {
    double middle = (train_set[i].get_attr(attr_index) + train_set[i + 1].get_attr(attr_index)) / 2;
    double_t ent_current = InfoGain(train_set, attr_index, middle, ent);
    if (ent_current > ent_init) {
      ent_init = ent_current;
      middle_init = middle;
    }
  }
  return {middle_init, ent_init};
}

/**
 * 返回最佳信息增益的属性以及分割点
 * @param train_set 训练集
 * @param attrs 可能的节点
 * @return {分割点,节点id}
 */
pair<double, int> GetBestAttr(vector<TrainItem> &train_set, set<int> &attrs) {
  int best = *attrs.begin();
  auto result_init = AttrInfoBestGain(train_set, best);
  for (auto &i:attrs) {
    auto result_current = AttrInfoBestGain(train_set, i);
    if (result_init.second < result_current.second) {
      result_init = result_current;
      best = i;
    }
  }
  return {result_init.first, best};
}

//from stack overflow
template<typename TreeNode>
void printBT(const std::string &prefix, const TreeNode *node, bool isLeft) {
  if (node != nullptr) {
    std::cout << prefix;
    std::cout << (isLeft ? "├─(<)──" : "└─(>)──");
    // print the value of the node
    std::cout << *node << std::endl;
    // enter the next tree level - left and right branch
    printBT(prefix + (isLeft ? "│           " : "            "), node->left, true);
    printBT(prefix + (isLeft ? "│           " : "            "), node->right, false);
  }
}
#endif //AI_LAB_4__MATHTOOLS_H_
