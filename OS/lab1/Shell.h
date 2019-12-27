//
// Created by agno3 on 11/17/19.
//

#ifndef OS_LAB1__SHELL_H_
#define OS_LAB1__SHELL_H_
#include <string>
#include <iostream>
#include <map>
#include <ios>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>
#include "ProcessManager.h"
std::vector<std::string> split(const std::string &command_line) {
  std::vector<std::string> tokens;
  std::stringstream s(command_line);
  std::string command;
  while (s >> command)
    tokens.push_back(command);
  return tokens;
}

class Shell {
 private:
  ProcessManager manager;
  std::string script;
  bool use_script;
 public:
  explicit Shell(std::string  name):script(std::move(name)),use_script(true){}
  Shell():use_script(false){}
  static std::map<std::string, int> command_map;
  void run() {
    std::ifstream file(script);
    std::ofstream stream("./a.txt");
    
    std::string command;
    std::cout << "CopyRight @agno3";
    do {
      if(use_script){
        getline(file,command);
      } else{
        printf("\n>>> ");
        getline(std::cin, command);
      }
      std::vector<std::string> tokens = split(command);
      if (!tokens.empty()) {
        if (command_map.find(tokens[0]) != command_map.end()) {
          switch (command_map[tokens[0]]) {
            case 0:manager.init();
              break;
            case 1:
              if (tokens.size() != 3) {
                std::cout << "Usage cr <pid> <priority>";
                break;
              }
              manager.Create(std::stoi(tokens[1]), std::stoi(tokens[2]));
              break;
            case 2:
              if (tokens.size() != 2) {
                std::cout << "Usage de <pid>";
                break;
              }
              manager.Destroy(std::stoi(tokens[1]));
              break;
            case 3:
              if (tokens.size() != 3) {
                std::cout << "Usage req <rid> <num>";
                break;
              }
              manager.Request(std::stoi(tokens[1]), std::stoi(tokens[2]));
              break;
            case 4:
              if (tokens.size() != 3) {
                std::cout << "Usage rel <rid> <num>";
                break;
              }
              manager.Release(std::stoi(tokens[1]), std::stoi(tokens[2]));
              break;
            case 5:manager.list();
              break;
            case 6:manager.TimeOut();
              break;
            default:break;
          }
          stream<<manager.Pid();
        } else {
          if (tokens[0] != "quit")
            std::cout << "no command named " << tokens[0];
        }
      }
    } while (command != "quit");
  }
};

std::map<std::string, int> Shell::command_map = {
    {"init", 0},
    {"cr", 1},
    {"de", 2},
    {"req", 3},
    {"rel", 4},
    {"list", 5},
    {"to", 6}
};
#endif //OS_LAB1__SHELL_H_
