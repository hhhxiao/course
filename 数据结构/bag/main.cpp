#include <iostream>
#include "bag.h"

int main() {


    int a = 0;
    std::cout << "input the size of the case:\n";
    int case_size;
    std::cin >> case_size;
    std::cout << "input all the item\n";
    bag bag;
    while (std::cin >> a) { bag.add_item(a); }

    if (case_size < bag.max()->size) {
        std::cout << "no case!!!\n";
        return 0;
    }
    
    
    int case_num = 0;//初始化箱子数量
    int current_case_left = case_size;
    while (bag.size() != 0) {
        std::cout << "case " << case_num << ":(";
        while (bag.size() != 0 && current_case_left >= bag.min()->size) {
            int size = bag.find_better(current_case_left);
            bag.remove_item(size);
            current_case_left -= size;
            std::cout << size << ",";
        }
        std::cout << ")\n";
        case_num++;
        current_case_left = case_size;
    }
    std::cout << "total num:" << case_num;
    return 0;
}