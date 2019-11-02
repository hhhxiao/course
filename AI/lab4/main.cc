#include <iostream>
#include "MathTools.h"
#include "TrainItem.h"
#include "DecisionTree.h"
#include <fstream>
#include <vector>
using namespace std;
vector<vector<double>> read_data(const string &path) {
  ifstream stream(path);
  double ele = 0;
  vector<vector<double>> result(75);
  int i = 0;
  int j = -1;
  while (stream >> ele) {
    if (i % 5 == 0)
      j++;
    result[j].push_back(ele);
    i++;
  }
  return result;
}

vector<TrainItem> vec2TItem(vector<vector<double >> &data) {
  vector<TrainItem> vector;
  for (auto i :data)
    vector.push_back(TrainItem(i));
  return vector;
}

int main() {

  auto result = read_data("../traindata.txt");
  vector<TrainItem> v = vec2TItem(result);
  TreeNode node(v);
  GenerateTree(&node);
  printBT<TreeNode>("", &node, false);
  do_cut(&node);
  std::cout << "------------------------------------------------\n";
  printBT<TreeNode>("", &node, false);
  auto test_data = read_data("../testdata.txt");
  vector<TrainItem> test_items = vec2TItem(test_data);
  int right = 0;
  for (auto &i:test_items) {
    if (i.get_category() == find(i, &node))
      right++;
  }
  std::cout << right * 100.0 / 75 << "%";
  return 0;
}