#include <iostream>
#include "Square.h"

int main() {

//    int arr1[9] = {2,1,3,8,0,4,7,6,5};
//    int arr2[9] = {1,2,3,4,6,5,7,8,0};
//    int arr3[9] = {1,3,2,4,5,6,7,8,0};
//    int arr4[9] = {4,2,3,1,5,6,7,8,0};
//    int arr5[9] = {1,2,3,4,5,6,7,8,0};
//    int arr6[9] = {1,2,3,6,5,4,7,8,0};
//    int arr7[9] = {1,8,3,4,5,6,7,2,0};
//    
//    Square sq1(arr1);
//    Square sq2(arr2);
//    Square sq3(arr3);
//    Square sq4(arr4); 
//    Square sq5(arr5);
//    Square sq6(arr6); 
//    Square sq7(arr7);
//    
//    set<Square> s;
//    s.insert(sq1);
//    s.insert(sq2);
//    s.insert(sq3);
//    s.insert(sq4);
//    s.insert(sq5);
//    s.insert(sq6);
//    s.insert(sq7);
//    
//    auto iter =  s.find(sq3);
//    
//    std::cout<<(iter == s.end());
    
    int arr[9] = {
            2,3,4,
            1,0,5,
            8,6,7
    };
    Square s(arr);
    std::vector<Square> steps =  s.a_star();
    int j = 1;
    for (const auto& i :steps) {
        std::cout<<"step"<<j<<":\n"<<i<<"\n";
        ++j;
    }
    
    return 0;
}