#include "marray.h"
#include <iostream>
using namespace std;
using UltimateHandsome::SORT_OPTION ;
int main(){
    UltimateHandsome::Array<int> a;
    a.add(1,2,3,4,6,4,4,5,65,7,4,3,4,5,7,4);
    a.insert_before(9,56);
    a.filter([](int ele)->bool{
        return ele>4;
    }).each([](int & ele){cout<<ele<<"\n";});
    return 0;
}