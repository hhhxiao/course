//
// Created by xhy on 2020/5/24.
//

#ifndef COMPILER_PARSER_ITEM_H_
#define COMPILER_PARSER_ITEM_H_

namespace xhy {
struct VarItem {
  std::string vname; //变量名
  std::string vproc; //所属过程
  int vkind; //类型 0是变量1是形参
  tokenType vtype; //变量类型
  size_t vlev; //层次(函数嵌套的层次)
  size_t vaddr; //表中的偏移

  VarItem(std::string name, std::string proc, int kind, tokenType type, size_t lev, size_t addr)
      : vname(std::move(name)), vproc(std::move(proc)), vkind(kind), vtype(type), vlev(lev), vaddr(addr) {}
  friend std::ostream &operator<<(std::ostream &os, const VarItem &item) {
    os
        << item.vname << "     "
        << "integer" << "     "
        << item.vaddr << "     "
        << item.vproc << "     "
        << item.vlev << "     "
        << item.vkind;
    return os;
  }
};

//过程项
struct FuncItem {
  std::string pname;//过程名字
  tokenType ptype = tokenType::FUNC; //过程类型
  size_t plev = 0;
  int fadr = 0;
  int ladr = 0;

  FuncItem(std::string name, tokenType type, size_t lev, int faddr, int laddr)
      : pname(std::move(name)), ptype(type), plev(lev), fadr(faddr), ladr(laddr) {}
  FuncItem() = default;
  friend std::ostream &operator<<(std::ostream &os, const FuncItem &item) {
    os << item.pname << "  " << item.ptype << "  " << item.plev << "  "
       << item.fadr << "  " << item.ladr;
    return os;
  }
};

}
#endif //COMPILER_PARSER_ITEM_H_
