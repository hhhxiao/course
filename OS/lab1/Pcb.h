//
// Created by agno3 on 11/17/19.
//

#ifndef OS_LAB1__PCB_H_
#define OS_LAB1__PCB_H_
#include <vector>
#include <ostream>
#include <iostream>
#include "Rcb.h"
#define  PRO_INIT 0
#define PRO_USER 1
#define PRO_SYS 2
enum class Status { Ready, Block, Running };
typedef unsigned int pid_type;
struct Pcb {
  pid_type pid;
  Status status;
  Pcb *parent;
  std::vector<Pcb *> children;
  std::map<rid_type, int> resources;
  int priority;

  Pcb(pid_type pid_, Status status_, int priority_, Pcb *parent_) :
      pid(pid_),
      status(status_),
      parent(parent_),
      children(std::vector<Pcb *>()),
      priority(priority_) {
    for (int i = 1; i <= 4; ++i)
      resources[i] = 0;
  }

  friend std::ostream &operator<<(std::ostream &os, const Pcb &pcb) {
    std::cout << pcb.pid << "    ";
    if (pcb.status == Status::Ready) {
      std::cout << "ready    ";
    } else if (pcb.status == Status::Block) {
      std::cout << "block    ";
    } else if (pcb.status == Status::Running) {
      std::cout << "running  ";
    }
    if (pcb.parent == nullptr)
      std::cout << -1 << "      ";
    else
      std::cout << pcb.parent->pid << "       ";
    std::cout << pcb.priority<<"      ";
    for (auto i :pcb.resources)
      std::cout<<i.second<<"  ";
    return os;
  }

  bool operator<(const Pcb &rhs) const {
    return this->pid < rhs.pid;
  }

};
typedef Pcb Process;
#endif //OS_LAB1__PCB_H_
