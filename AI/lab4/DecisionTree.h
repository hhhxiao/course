//
// Created by agno3 on 10/30/19.
//

#ifndef AI_LAB_4__DECISIONTREE_H_
#define AI_LAB_4__DECISIONTREE_H_
#include <utility>
#include <vector>
#include <cassert>
#include <ostream>
#include "TrainItem.h"
#include "MathTools.h"
using namespace std;

struct TreeNode {
  vector<TrainItem> train_set;
  std::set<int> attr;
  TreeNode *left{};
  TreeNode *right{};
  double split{};
  int index{};
  bool is_leaf{};
  int category{};
  explicit TreeNode(vector<TrainItem> set)
      : split(0), category(-1), train_set(std::move(set)), left(nullptr), right(nullptr), is_leaf(false) {
    for (int i = 0; i < 4; i++)
      attr.insert(i);
  }
  TreeNode() = default;

  ~TreeNode() {
    delete left;
    delete right;
  }
  friend ostream &operator<<(ostream &os, const TreeNode &node) {
    if (node.is_leaf) {
      std::cout << "(" << node.category << ")";
    } else {
      os << "attr[" << node.index << "] _?_ " << node.split;
    }
    return os;
  }
};

static void GenerateTree(TreeNode *node) {
  assert(node != nullptr);
  //把当前的训练集按照结果分割
  //如果分出来只有一中元素,则说明是叶子节点,标记该节点所属的类型
  map<int, vector<TrainItem>> result_set = DivideByResult(node->train_set);
  if (result_set.size() == 1) {
    //标记类型和叶子节点
    node->is_leaf = true;
    node->category = result_set.begin()->first;
    return;
  }
  //如果是空集
  if (node->attr.empty()) {
    //找出元素最多的分类
    int cap = result_set.begin()->first;
    int size = result_set.begin()->second.size();
    for (auto &i:result_set) {
      if (size < i.second.size()) {
        size = i.second.size();
        cap = i.first;
      }
    }
    //设置类别
    node->is_leaf = true;
    node->category = cap;
    return;
  }

  //获取最优的属性
  pair<double, int> result = GetBestAttr(node->train_set, node->attr);
  node->split = result.first;
  node->index = result.second;

  vector<TrainItem> less;
  vector<TrainItem> much;
  for (auto &item:node->train_set) {
    if (item.get_attr(result.second) > result.first)
      much.push_back(item);
    else
      less.push_back(item);
  }
  //生成左右俩子树
  //对子树进行生成
  auto *less_node = new TreeNode(less);
  less_node->attr = node->attr;
  less_node->attr.erase(result.second);
  node->left = less_node;

  auto *much_node = new TreeNode(much);
  much_node->attr = node->attr;
  much_node->attr.erase(result.second);
  node->right = much_node;
  GenerateTree(less_node);
  GenerateTree(much_node);
}

int find(TrainItem &item, TreeNode *node) {
  //if (node == nullptr)return -1;
  if (node->is_leaf) {
    return node->category;
  }
  if (item.get_attr(node->index) > node->split)
    return find(item, node->right);
  else
    return find(item, node->left);
}

void do_cut(TreeNode *node) {
  if (!node->left->is_leaf)
    do_cut(node->left);
  if (!node->right->is_leaf)
    do_cut(node->right);
  if (!node->is_leaf && node->left->is_leaf && node->right->is_leaf) {
    if (node->left->category == node->right->category) {
      node->is_leaf = true;
      node->category = node->left->category;
      node->left = nullptr;
      node->right = nullptr;
    }
    return;
  }

}

#endif //AI_LAB_4__DECISIONTREE_H_
