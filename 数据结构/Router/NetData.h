//
// Created by agno3 on 11/20/19.
//

#ifndef ROUTER_2__NETDATA_H_
#define ROUTER_2__NETDATA_H_
#include <utility>
#include <vector>
#include <map>
#include <ostream>
#include <mutex>
const int INF = 1024;
typedef unsigned int ip_type;
using namespace std;
struct RouterItem {
  ip_type desc;
  ip_type next;
  int cost;
  RouterItem(ip_type d, ip_type n, int c) :
      desc(d), next(n), cost(c) {}
      RouterItem(ip_type d,ip_type n):desc(d),next(n),cost(0){}
  friend ostream &operator<<(ostream &os, const RouterItem &item);
  
};
ostream &operator<<(ostream &os, const RouterItem &item) {
  os << item.desc << "          " << item.next << "          " << item.cost;
  return os;
}

class NetData {
 private:
  mutex m;
  vector<vector<int>> connections;
  vector<vector<RouterItem>> routerTable;
 public:
  explicit NetData(int num)
      : connections(vector<vector<int>>(num)) {
    for (int i = 0; i < num; i++) {
      for (int j = 0; j < num; ++j)
        connections[i].push_back(INF);
      routerTable.emplace_back();
    }
    for (int i = 0; i < num; ++i)
      connections[i][i] = 0;
  }
  void setConn(ip_type r1, ip_type r2, int cost) {
    connections[r1][r2] = cost;
    connections[r2][r1] = cost;
  }
  int getCost(ip_type r1, ip_type r2) {
    return connections[r1][r2];
  }
  
  mutex& getMutex(){
    return m;
  }
  
  vector<vector<int>> getConnections()const {
    return connections;
  }
  
  
  void setRouterTable(std::vector<vector<RouterItem>> table){
    this->routerTable = std::move(table);
  }
  
  vector<RouterItem> getRouterTable(ip_type ip){
    return routerTable[ip];
  }
  
  
  
  
  
  void print() const {
    printf("      ");
    for (auto i = 0; i < connections.size(); ++i)
      printf("%7d",i);
    printf("\n");
    int index = 0;
    for (auto &i:connections) {
      printf("%6d ",index);
      index++;
      for (auto j:i)
        printf("%6d ", j);
      printf("\n");
    }
  }

};

#endif //ROUTER_2__NETDATA_H_
