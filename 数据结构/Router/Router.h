//
// Created by agno3 on 11/20/19.
//

#ifndef ROUTER_2__ROUTER_H_
#define ROUTER_2__ROUTER_H_
using namespace std;
#include "NetData.h"
#include "PathAlgo.h"
void Sleep(unsigned int seconds) {
  this_thread::sleep_for(chrono::duration<int>(seconds));
}

class Router {
 private:
  NetData &net_data;
  const ip_type ip;
  map<ip_type, int> connect;
  vector<RouterItem> routerTable;
 public:
  
  Router(ip_type ip,NetData &data):
  net_data(data),
  ip(ip),
  connect(map<ip_type ,int>()),
  routerTable(vector<RouterItem>()){}
  
  void Send() {
    for (auto i:connect)
      net_data.setConn(ip, i.first, i.second);
    printf("%d:send data\n", ip);
  }
  void Receive() {
    this->routerTable = net_data.getRouterTable(ip);
    printTable();    
  }
  
  void printTable(){
    net_data.getMutex().lock();
    printf("+-----------------+\n|ip: %d            |"
           "\n|desc   next  cost|\n+-----------------+\n",ip);
    for(auto i:routerTable){
      printf("|%d       %d       %d|\n",i.desc,i.next,i.cost);
    }
    printf("+-----------------+\n\n");
    net_data.getMutex().unlock();
  }
  
  void RouterRun(){
    for(int i = 0;i< 100000;i++){
      geneConn();
      Send();
      Sleep(2);
      Receive();
      Sleep(8);
    }
  }
  
  void GatewayRun(){
    for(int i = 0;i< 100000;i++){
      geneConn();
      Send();
      Sleep(1);
      net_data.print();
      auto table = generateTable(net_data.getConnections());
      net_data.setRouterTable(table);
      Receive();
      Sleep(9);
    }
  }
  
  void geneConn(){
    connect.clear();
    srand(time(NULL));
    for (int i = 0; i < 9; ++i) {
      if(i!=ip && rand() %10 > 2){
        connect[i] = rand()%10 + 1;
      }
    }
  }

  bool operator<(const Router &r) const {return this->ip < r.ip;}
  bool operator==(const Router &r)const {return this->ip == r.ip;}
  
  

};

#endif //ROUTER_2__ROUTER_H_
