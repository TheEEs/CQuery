#include <iostream>
#include "marray.h"
using namespace std;
using namespace UltimateHandsome;
int main(){
    Array<int> a;
    a.add(1,2,3,4,5,6,7,8,9,0);
    Array<int> b = a.map<int>([](int ele,int * new_item)-> bool{
        return (*new_item = ele * 2) > 10;
    });
    a.clear();
    b.each([](int & element){
        cout<<element<<"\n";
    });
}