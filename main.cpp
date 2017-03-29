#include <iostream>
#include "marray.h"
using namespace std;
using namespace UltimateHandsome;
int main(){
    Array<int>x ;
    x.add(1,2);
    x.sort(SORT_OPTION::BIGGEST2SMALLEST);
    x.each([](int & e){
        cout<<e<<'\n';
    });
    cout<<"look for: "<<4<< " : "<< x.look_for(2,SEARCH_TYPE::BINARY);
}