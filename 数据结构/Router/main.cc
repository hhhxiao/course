#include <iostream>
#include "NetData.h"
#include "PathAlgo.h"
#include <thread>
#include "Router.h"
using  namespace std;

void RouterThread(ip_type ip, NetData &data) {
  Router router(ip, data);
  router.RouterRun();
}

void GatewayThread(ip_type ip, NetData &data) {
  Router router(ip, data);
  router.GatewayRun();

}

int main() {
  NetData n(9);
  vector<thread> threads;
  threads.reserve(8);
  for (int i = 1; i <= 8; ++i)
    threads.emplace_back(RouterThread,i,std::ref(n));
  thread gateway_thread(GatewayThread,0,std::ref(n));

  for (int i = 0; i < 8; ++i) 
    threads[i].join();
  gateway_thread.join();
  return 0;
}