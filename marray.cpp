#include <iostream>
#include "marray.h"
using namespace std;
using UltimateHandsome::Array;
int main(){

    Array<int> a;
    a.add(6);
    a.add(5);
    a.add(2);
    a.add(1);
    a.sort();
    a.each([](int & ele){
        cout<<ele<<"\n";
    });
}