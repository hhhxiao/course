//
// Created by agno3 on 10/30/19.
//

#ifndef AI_LAB_4__TRAINITEM_H_
#define AI_LAB_4__TRAINITEM_H_
#include <vector>
#include <set>
#include <ostream>
using namespace std;
class TrainItem {
 private:
  double attrs[4]{};
  int category;
 public:

  explicit TrainItem(const vector<double> &d) {
    for (int i = 0; i < 4; i++)
      this->attrs[i] = d[i];
    category = static_cast<int>(d[4]);
  }

  TrainItem(const double *attrs, int category) : category(category) {
    for (int i = 0; i < 4; i++)
      this->attrs[i] = attrs[i];
  }

  double *get_attrs() { return attrs; }
  double get_attr(int i) const { return attrs[i]; }
  int get_category() { return category; }
  bool operator==(const TrainItem &rhs) const {
    return attrs == rhs.attrs &&
        category == rhs.category;
  }

  friend std::ostream &operator<<(std::ostream &os, const TrainItem &item) {
    os << "attrs: ";
    for (auto i :item.attrs) {
      os << i << "  ";
    }
    os << " category: " << item.category;
    return os;
  }

};
#endif //AI_LAB_4__TRAINITEM_H_
