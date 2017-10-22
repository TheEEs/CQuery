#include <stdio.h>
#include <stdlib.h>
class Permutation{
public:
    typedef void(*callback)(int * _p_array,int len);
    Permutation(int number){
        this->_nop = number;
        this->_p_array = new int[this->_nop];
        for(int i =1;i<= this->_nop;i++)
            this->_p_array[i-1]=i;
    }
    int set_callback(Permutation::callback _callback_fn) {
        if (_callback_fn)
            this->_cb = _callback_fn;
        else
            return 0;
        return 1;
    }
    int next(){
        if(this->is_max) return 0;
        if(this->_cb)this->_cb(this->_p_array,this->_nop);
        this->is_max = this->_permutation(this->_p_array,this->_nop);
        return 1;
    }
private:
    callback _cb = nullptr;
    int * _p_array;
    bool is_max=false;
    int _nop;
    int _search_for_the_greater_neighbor(int *start_array,int len, int search_value) throw(int){
        for(int i = 0;i< len;i++)
            if(start_array[i]> search_value)
                return i;
        return -1;
    }
    int /* the return value indicates that whether if the array is maximum state
 * 1 - the array is maximum state
 * 0 - the array is not maximum state
 * -1 - error raised
 * */ _permutation(int *_start_array, int len){
        int temp;
        switch (len){
            case 0:
            case 1:
                return 0;
            case 2:
                if(_start_array[0] > _start_array[1])
                    return 1;
                temp= _start_array[0];
                _start_array[0] = _start_array[1];
                _start_array[1] = temp;
                return 0;
            default:
                int is_max = this->_permutation(_start_array + 1, len -1);
                if(is_max)
                    if (*_start_array > _start_array[1])
                        return 1;
                    else{
                        qsort(_start_array+1,len-1,sizeof(int),({[](const void * a, const void *b){
                                int _a= *(int *)a;
                                int _b = *(int *)b;
                                return _a - _b;
                            };}));
                        int neighbor_pos = this->_search_for_the_greater_neighbor(_start_array+1,len -1,*_start_array) + 1;
                        if(neighbor_pos){
                            temp = _start_array[0];
                            _start_array[0] = _start_array[neighbor_pos];
                            _start_array[neighbor_pos]= temp;
                            return 0;
                        }else{
                            return -1;
                        }
                    }
                else
                    return 0;
        }


    }
};
