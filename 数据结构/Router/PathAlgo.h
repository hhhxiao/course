//
// Created by agno3 on 11/20/19.
//

#ifndef ROUTER_2__PATHALGO_H_
#define ROUTER_2__PATHALGO_H_
#include "NetData.h"
#include <algorithm>
//路由器0为网关
vector<vector<ip_type>> dijkstra(
    const vector<vector<int>> &network) {
  int router_num = network.size();//=9
  vector<vector<ip_type>> paths(router_num,vector<ip_type >(1,0));//存储路径信息
  vector<int> distance(router_num,INF);//存储距离信息
  vector<bool> known(router_num, false);//存储发现信息
  //更新和网关直连的路由器的距离
  for (int i = 0; i < router_num; i++)
    distance[i] = network[0][i];
  //网关到自身的距离是0
  distance[0] = 0;
  known[0] = true;
  //没有全部发现的的时候while循环
  while (!all_of(known.begin(), known.end(),
                 [](bool p) { return p; }
  )) {
    //找到未发现的距离最短的路由器
	  ip_type target_router_ip = 10;
		int min_cost = INF;
		for (int i = 0; i < router_num; i++) {
			if (!known[i]) {
				target_router_ip = i;
				min_cost = distance[i];
			}
			}
    for (int i = 0; i < router_num; i++) {
      if (!known[i] && distance[i] < min_cost) {
        min_cost = distance[i];
        target_router_ip = i;
      }
    }
    known[target_router_ip] = true;
    paths[target_router_ip].push_back(target_router_ip);
    //更新距离
    for (int i = 0; i < router_num; i++) {
      if (i != target_router_ip && !known[i] &&
          distance[i] > network[target_router_ip][i] + distance[target_router_ip]) {
        //更新距离和已经发现的路径
        distance[i] = network[target_router_ip][i] + distance[target_router_ip];
        paths[i] = paths[target_router_ip];
      }
    }
  }
  return paths;
}

vector<vector<RouterItem>> generateTable(const vector<vector<int>>& network){
  vector<vector<RouterItem>> r (network.size());
  vector<vector<ip_type>> paths = dijkstra(network);
  for(size_t i = 1;i<paths.size();i++){
    //paths[i]就是去往i的路径
    for(size_t j = 0;j<paths[i].size()-1;j++)
      r[paths[i][j]].push_back(RouterItem(i,paths[i][j+1],network[paths[i][j]][paths[i][j+1]]));
  }
  return r;
}



#endif //ROUTER_2__PATHALGO_H_
