//
// Created by agno3 on 11/17/19.
//
/**
 * 调度策略:: 基于优先级的抢占式调度策略
 */
#ifndef OS_LAB1__PROCESSMANAGER_H_
#define OS_LAB1__PROCESSMANAGER_H_
#include "Pcb.h"
#include "Rcb.h"
#include <map>
#include <queue>
#include <cassert>
#include <set>

typedef std::queue<Process *> ReadyList;
typedef std::queue<std::pair<Process *, int>> BlockList;
class ProcessManager {
 private:
  std::vector<ReadyList> ready_lists; //等待队列(包括已经被killed的)
  std::vector<BlockList> block_list;
  Process *running; //当前在运行的进程
  std::set<Process *> alive; //当前所有进程
  std::vector<Rcb> resource_list; //当前资源列表
 public:
  ProcessManager() : ready_lists(std::vector<ReadyList>(3)),
                     block_list(std::vector<BlockList>(4)),
                     running(nullptr),
                     resource_list(std::vector<Rcb>()) {
    this->init();
  }
  //初始化函数
  void init() {
    //初始化init进程
    running = new Process(0, Status::Running, PRO_INIT, nullptr);
    //加入存活的进程
    alive.insert(running);
    //初始化资源
    for (int i = 1; i <= 4; i++)
      resource_list.emplace_back(i, i);
    std::cout << "init finished";
  }

  pid_type Pid() {
    return running->pid;
  }

  //创建进程
  void Create(pid_type pid, int priority) {
    for (auto i:alive) {
      if (i->pid == pid) {
        std::cout << "process [" << pid << "] already exist";
        return;
      }
    }
    if (priority < 0 || priority > 2) {
      std::cout << "Invalid priority";
      return;
    }
    //创建进程
    auto *process = new Process(pid, Status::Ready, priority, running);
    running->children.push_back(process);//连接父子关系
    alive.insert(process);
    std::cout << "add process ["
     << pid << "] to read queue " << priority << "\n";
    Schedule(process);
  }

  //销毁进程
  //todo BUG1: kill 根进程后的行为未知
  void kill_tree(Process *process) {
    //递归删除该进程以及所有的子进程
    if (process == nullptr) return;
    if (alive.find(process) == alive.end())return;
    for (auto pro_pointer:process->children)
      kill_tree(pro_pointer);
    std::cout << "killed process [" << process->pid << "]\n";
    //先删除子进程再删除当前进程
    //释放进程的资源 
    alive.erase(process);
    //释放process的资源
    for (auto i:process->resources) {
      this->resource_list[i.first-1].available += i.second;
      while(!block_list[i.first-1].empty() && 
      alive.find(block_list[i.first-1].front().first)==alive.end()){
        block_list[i.first-1].pop();  
      }
      if(!block_list[i.first-1].empty()){
        if(block_list[i.first-1].front().second <= this->resource_list[i.first-1].available){
          Schedule(block_list[i.first-1].front().first);
          block_list[i.first-1].pop();
        }
        
      }
    }
    
    
    
  }

  //kill进程
  void Destroy(pid_type pid) {
    if (running->pid == pid) {
      //如果是结束当前正在运行的进程
      //调用递归算法kill掉所有的子进程
      kill_tree(running);
      this->running = nullptr;
      Schedule(nullptr);
    } else {
      //直接kill该进程以及它的所有子进程
      for (auto list:ready_lists) {
        while (!list.empty()) {
          if (list.front()->pid == pid) {
            kill_tree(list.front());
            break;
          }
          list.pop();
        }
      }
      for (auto list:block_list) {
        while (!list.empty()){
          if(list.front().first->pid == pid){
            kill_tree(list.front().first);
            break;
          }
          list.pop();
        }
      }
    }
  }
  //请求资源
  void Request(rid_type rid, int n) {
    //获取当前进程已经占有该种资源的数量
    int num = running->resources[rid];

    //总共的资源数量小于要请求的数量
    if (resource_list[rid - 1].init_num < n + num) {
      //发出提示但是什么都不做
      std::cout << "num is too much,request failed!";
    } else {
      //总的资源数量是足够的
      //目前可获得的资源不够了,阻塞当前进程
      if (resource_list[rid - 1].available < n) {
        //阻塞当前进程(加入对应资源的阻塞队列)
        std::cout << "resource is not enough\n";
        running->status = Status::Block;
        block_list[rid - 1].push({running, n});
        this->running = nullptr;
        Schedule(nullptr);
      } else {//目前请求的资源数量足够
        std::cout << "res request success!\n";
        //请求资源
        running->resources.erase(rid);
        running->resources.emplace(rid, num + n);
        //更新剩余资源
        resource_list[rid - 1].available -= n;
      }
    }
  }

  //释放资源
  void Release( rid_type rid, int n) {
    int num = running->resources[rid];
    if (num < n) {
      //释放数量过多,视为错误
      std::cout << "num error!";
    } else {
      //更新资源数量
      running->resources.erase(rid);
      running->resources.emplace(rid, num - n);
      //增加可用资源
      resource_list[rid - 1].available += n;
      //如果当前资源的阻塞队列是空的就直接返回,不动调度,以免发生空指针异常
      if (block_list[rid - 1].empty())
        return;
      auto request = block_list[rid - 1].front();
      //如果阻塞列表的第一个进程请求的资源够了
      if (request.second <= resource_list[rid - 1].available) {
        std::cout << "process [" << request.first->pid << "] was wake up\n";
        //从阻塞列表中移除,该进程还是游离态的,传入schedule里面判断是否要调度运行
        block_list[rid - 1].pop();
        Schedule(request.first);
      }
    }
  }

  //时间片结束
  void TimeOut() {
    std::cout << "current thread CPU timeout\n";
    Schedule(running);
  }

  Process *getNext() {
    Process *process;
    //依次遍历三个等待队列的队头
    for (int priority = 2; priority > 0; --priority) {
      while (!ready_lists[priority].empty()) {
        if (alive.find(ready_lists[priority].front()) != alive.end()) {
          process = ready_lists[priority].front();
          ready_lists[priority].pop();
          return process;
        }
        ready_lists[priority].pop();
      }
    }
    return nullptr;
  }

  //调度新的进程上来运行
  void Schedule(Process *process) {
    //资源请求不够时的阻塞当前进程
    if (this->running == nullptr) {
      auto proc = getNext();
      assert(proc != nullptr);
      proc->status = Status::Running;
      running = proc;
    } else {
      if (running == process) {//time to的情况
        //从ready队列里面选出优先级最高的的调度运行
        auto proc = getNext();
        if (proc != nullptr) {
          std::cout << "Schedule process[" << proc->pid << "] in running\n";
          //说明整个环境不只当前一个进程
          //当前进程假如就绪队列
          ready_lists[running->priority].push(running);
          running->status = Status::Ready; //设置为ready
          proc->status = Status::Running;
          running = proc;
        } else {
          std::cout << "no  other process\n";
        }

      } else {
        //需要调度的进程优先级高于当前正在运行的进程的优先级
        if (running->priority < process->priority) {
          //更新进程状态
          running->status = Status::Ready;
          process->status = Status::Running;
          //被抢占的进程加入对应的队列
          ready_lists[running->priority].push(running);
          this->running = process;
        } else {
          //直接把需要调度的进程假如队列
          process->status = Status::Ready;
          ready_lists[process->priority].push(process);
        }
      }
    }
  }

  void list() const {
    std::cout << "Thread:\n";
    std::cout << "pid  status parent priority r1 r2 r3 r4\n";
    //打印正在运行的进程
    std::cout << *running << std::endl;
    //打印就绪态的进程
    for (auto list:ready_lists) {
      while (!list.empty()) {
        if (alive.find(list.front()) != alive.end())
          std::cout << *list.front() << "\n";
        list.pop();
      }
    }
    //打印阻塞的进程
    for (auto list:block_list) {
      while (!list.empty()) {
        if (alive.find(list.front().first) != alive.end())
          std::cout << *list.front().first << "\n";
        list.pop();
      }
    }
    //打印资源情况
    std::cout << ""
                 "\nResources:\nrid init_num available\n";
    for (auto i:resource_list)
      std::cout << i.id << "      " << i.init_num << "      " << i.available << "\n";
  }

};

#endif //OS_LAB1__PROCESSMANAGER_H_
