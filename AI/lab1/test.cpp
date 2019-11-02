#include <iostream>
#include "status.h"
#include <vector>

int main() {
    
    
    int m,c,n;
    std::cout<<"please input the num of mission,common and max size of boat:\n";
    std::cin>>m>>c>>n;
    status sta(m,0,c,0,n);
    
    std::vector<status> s = sta.find_path();
    
    if(s.empty()){
        std::cout<<"no path!\n";
        return 0;
    }
    for (auto i:s) {
    std::cout<<i<<" --->\n "; 
    }
    std::cout<<"left side(0,0),right side("<<m<<","<<n <<") boat status: right side (0,0)";
    return 0;
}