//
// Created by agno3 on 11/17/19.
//

#ifndef OS_LAB1__RCB_H_
#define OS_LAB1__RCB_H_
#include <vector>
typedef unsigned int rid_type;
//资源控制块
struct Rcb {
  rid_type id; //资源id
  int init_num; //总共的数量
  int available; //可请求分配的数量
  // std::vector<Process *> waiting_list;
  Rcb(int id_, int num_) : id(id_), init_num(num_), available(num_)
  //, waiting_list(std::vector<Process *>()) 
  {}
};
#endif //OS_LAB1__RCB_H_
