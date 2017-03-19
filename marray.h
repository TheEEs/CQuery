#include <stdlib.h>
#include <initializer_list>
#include <stdio.h>
#include <exception>
using namespace std;
namespace UltimateHandsome{
    enum class SORT_OPTION{
        SMALLEST2BIGGEST = 0,
        BIGGEST2SMALLEST = 1
    };
    enum class SEARCH_TYPE{
        LINEAR = 0,
        BINARY = 1
    };
    class OutOfRange : public exception {
    private:
        const char *_message;
        unsigned int _index;
    public:
        unsigned int index() { return this->_index; }

        const char *message() { return this->_message; }

        OutOfRange(const char *mess, unsigned int index) : exception() {
            this->_message = mess;
            this->_index = index;
        }
    };

    class EmptyException : public exception {
    private:
        const char *_message;
    public:
        const char *message() { return this->_message; }

        EmptyException(const char *message):exception() { this->_message = message; }
    };

    template<class T>
    class Array {
    public:
    	
    	/*Constructors are here*/
    	
    	template<class ...type>
    	void add(T item,type... params){
    		if(sizeof...(params)){
    			this->add(item);
    			this->add(params...);
			}
		}
    	
        void add(T item) {
            if (length == 0) {
                pivot = (node **)malloc(sizeof(Array::_node));
                last_node = new node{.value=item};
                length++;
                pivot[0] = last_node;
                pivot_length++;
            } else {
                node *new_node = new node{.value=item};
                new_node->previous = this->last_node;
                this->last_node->next = new_node;
                this->last_node = new_node;
                if (length % 10 == 0) {
                    this->pivot = (node **) realloc(this->pivot, sizeof(Array::node) * (length / 10));
                    this->pivot[length / 10] = new_node;
                    pivot_length++;
                }
                length++;
            }
        }



        Array<T> &operator <<(T item){
            this->add(item);
            return *this;
        }
        inline bool is_empty() { return this->length == 0; }
        

        UltimateHandsome::Array<T> & remove(unsigned int index) {
            if (this->length == 0 or this->length <= index)
                throw OutOfRange("Index is out of range", index);
            int offset;
            int pivot_number = (index - (offset = index % 10)) / 10;
            node *_nodex = this->pivot[pivot_number];
            short i;
            for (i = 0; i < offset; i++) {
                _nodex = _nodex->next;
            }
            node *pre_node, *next_node;
            pre_node = _nodex->previous;
            next_node = _nodex->next;
            if (pre_node) pre_node->next=next_node;
            if(next_node) next_node->previous=pre_node;
            if((index %10 !=0) and pivot_number < this->pivot_length-1)/*If index is in range 10..(this->pivot_length-1)*10*/
                pivot_number++;
            if(index <= (this->pivot_length-1) * 10)
            for(i = pivot_number;i<this->pivot_length;i++)
                this->pivot[i] = this->pivot[i]->next;
            this->length--;
            delete _nodex;
            if (!this->pivot[pivot_length - 1]) /*If the last tens are completely removed*/
                this->pivot = (node **) realloc(this->pivot, sizeof(Array::node) * --this->pivot_length);

            return * this;
        }


        UltimateHandsome::Array<T> & insert_after(unsigned int index, T value) {
            if (this->length == 0 or this->length <= index)
                throw OutOfRange("Index is out of range", index);
            int offset;
            int pivot_number = (index - (offset = index % 10)) / 10;
            node *before_node = this->pivot[pivot_number];
            for (auto i = 0; i < offset; i++) {
                before_node = before_node->next;
            }
            node * new_node = new node();new_node->value = value;
            if(index == this->length-1) this->last_node = new_node;
            node * after_node = before_node->next;
            before_node->next = new_node;
            new_node->previous=before_node;
            if(after_node){
                after_node->previous=new_node;
                new_node->next=after_node;
            }
            if(index < (this->pivot_length - 1)* 10)
                pivot_number++;
            if(index < (this->pivot_length-1) * 10)
                for(auto i = pivot_number;i<this->pivot_length;i++)
                    this->pivot[i] = this->pivot[i]->previous;
            if (this->length++ % 10 == 0) {
                this->pivot = (node **) realloc(this->pivot, sizeof(Array::node) * ++this->pivot_length);
                this->pivot[pivot_length - 1] = this->last_node;
            }
            return *this;
        }


        UltimateHandsome::Array<T> & insert_before(unsigned int index, T value) {
            if (this->length == 0 or this->length <= index)
                throw OutOfRange("Index is out of range", index);
            int offset;
            int pivot_number = (index - (offset = index % 10)) / 10;
            node *after_node = this->pivot[pivot_number];
            for (auto i = 0; i < offset; i++) {
                after_node = after_node->next;
            }
            node * new_node = new node{value};
            node * before_node = after_node->previous;
            new_node->next = after_node;
            after_node->previous = new_node;
            if(before_node){
                before_node->next=new_node;
                new_node->previous=before_node;
            }
            if(index %10 != 0 and index <= (this->pivot_length - 1)* 10)
                pivot_number++;
            if(index <= (this->pivot_length-1) * 10)
                for(auto i = pivot_number;i<this->pivot_length;i++)
                    this->pivot[i] = this->pivot[i]->previous;
            if (this->length++ % 10 == 0) {
                this->pivot = (node **) realloc(this->pivot, sizeof(Array::node) * ++pivot_length);
                this->pivot[pivot_length - 1] = this->last_node;
            }
            return * this;
        }

        template<typename x>
        Array<x> map(x *(&func)(T ele_ptr)) {
            Array<x> rel;
            if (this->length == 0) return rel;
            node *_nodex = this->pivot[0];
            x *callback_return;
            do {
                callback_return = func(_nodex->value);
                if (callback_return)
                    rel.add(*callback_return);
            } while (_nodex = _nodex->next);
            return rel;
        }


        UltimateHandsome::Array<T> & clear() {
            if (length == 0)return *this;
            node *inode = this->pivot[0];
            while (inode) {
                node *iinode = inode->next;
                delete (inode);
                inode = iinode;
            }
            free(this->pivot);
            this->length = 0;
            this->pivot_length = 0;
            return * this;
        }


        UltimateHandsome::Array<T> &operator << (UltimateHandsome::Array<T> a){
            if(!a.count()) return *this;
            node * begin = a.pivot[0];
            do {
                this->add(begin->value);
            }while(begin = begin->next);
            return *this;
        }


        T &operator[](unsigned int index) {
            if (this->length == 0)
                throw OutOfRange("Index is out of range", index);
            int offset;
            int pivot_number = (index - (offset = index % 10)) / 10;
            node *_nodex = this->pivot[pivot_number];
            for (auto i = 0; i < offset; i++) {
                _nodex = _nodex->next;
            }
            return _nodex->value;
        }

        unsigned int count() {
            return this->length;
        }

        T &last() {
            if (this->length == 0)
                throw EmptyException("Array is empty");
            return this->last_node->value;
        }

        T &first() {
            if (this->length == 0)
                throw EmptyException("Array is empty");
            return this->pivot[0]->value;
        }

        void swap(unsigned int first_index, unsigned int second_index) {
            if (first_index < 0 or first_index >= this->length) throw OutOfRange("Index is out of range", first_index);
            if (second_index < 0 or second_index >= this->length)
                throw OutOfRange("Index is out of range", second_index);
            T buffer;
            buffer = this->operator[](first_index); 
            this->operator[](first_index) = this->operator[](second_index);
            this->operator[](second_index) = buffer;
        }

        UltimateHandsome::Array<T> & each(void (*func)(T &)) {
            if (this->length == 0) return * this;
            node *_nodex = this->pivot[0];
            do {
                func(_nodex->value);
            } while (_nodex = _nodex->next);
            return  * this;
        }

        UltimateHandsome::Array<T> & each(void (*func)(T &, unsigned int)) {
            unsigned int index = 0;
            if (this->length == 0) return * this;
            node *_nodex = this->pivot[0];
            do {
                func(_nodex->value, index++);
            } while (_nodex = _nodex->next);
            return * this;
        }

        UltimateHandsome::Array<T> clone(){
            Array<T> rel = Array<T>();
            if (this->length == 0) rel;
            node *_nodex = this->pivot[0];
            do {
               rel.add(_nodex->value);
            } while (_nodex = _nodex->next);
            return rel;
        }

        T max(){
            if (this->length == 0) throw EmptyException("Array is empty");
            node *_nodex = this->pivot[0];
            T value = _nodex->value;
            while (_nodex = _nodex->next){
                if (_nodex->value > value)
                    value = _nodex->value;
            }
            return value;
        }

        T min(){
            if (this->length == 0) throw EmptyException("Array is empty");
            node *_nodex = this->pivot[0];
            T value = _nodex->value;
            while (_nodex = _nodex->next){
                if (_nodex->value < value)
                    value = _nodex->value;
            }
            return value;
        }

        UltimateHandsome::Array<T> & sort(UltimateHandsome::SORT_OPTION option = SORT_OPTION::SMALLEST2BIGGEST)/*An implementation of the quick sort algorithm*/{
            if(this->length <= 1) return *this;
            node * begin_node = this->pivot[0], * end_node = this->last_node;
            node * current_node;
            T buffer;
            while(true){
            	pharse1:
           		current_node = end_node;
           		while(current_node != begin_node){
           			current_node = current_node->previous;
           			if(option == SORT_OPTION::SMALLEST2BIGGEST ? current_node && current_node->value > end_node->value : current_node && current_node->value < end_node->value){
           				buffer = current_node->value;
						current_node->value = end_node->value;
						end_node->value = buffer;	
						goto pharse2;
					   }
				   }
				   if((end_node = end_node->previous) == begin_node) {this->sort_type = option; return *this;}
           	    pharse2:
           		current_node = begin_node;
           		while(current_node != end_node){
           			current_node = current_node->next;
           			if(option == SORT_OPTION::SMALLEST2BIGGEST ? current_node && current_node->value < begin_node->value : current_node && current_node->value > begin_node->value){
           				buffer = current_node->value;
           				current_node->value = begin_node->value;
           				begin_node->value = buffer;
           				goto pharse1;
					   }
				   }
				   if((begin_node = begin_node->next) == end_node) {this->sort_type = option; return *this;}
		   }
        }
    private:
        typedef struct _node {
            T value;
            _node *next;
            _node *previous;
        } node;
        node *last_node;
        node **pivot;
        unsigned int length = 0, pivot_length = 0;

        SORT_OPTION sort_type;
    public:
        //LINQ-like functions bellow here
        UltimateHandsome::Array<T> & look_for(T value,bool * return_value,enum SEARCH_TYPE type = SEARCH_TYPE::LINEAR){
            if (!this->count()) throw EmptyException("Array is empty.");
            node * begin_node ;//= this->pivot[0];
            if(type == SEARCH_TYPE::LINEAR){
                begin_node = this->pivot[0];
                do
                    if (begin_node->value == value) {*return_value=true; return *this;}
                while(begin_node = begin_node->next);
                *return_value = false;
                return * this;
            }
            else/*Binary search*/{
                /*Note that if the array have not been sorted yet this function will not work well!*/
                unsigned int pivot , begin = 0, end = this->length -1 ;
                while(end - begin != 0) {
                    pivot = begin + (end - begin)/2;
                    T value_x = this->operator[](pivot);
                    if( (bool)this->sort_type ? value < value_x : value > value_x) begin   = pivot +1;
                    else if ( (bool)this->sort_type ? value > value_x : value < value_x) end = pivot -1;
                    else return true;
                }
                return this->operator[](begin)== value;
            }
        }
        UltimateHandsome::Array<T> & filter(bool (*func)(T element)) {
            unsigned int index = 0;
            if(!this->length) return *this;
            node * _node = this->pivot[0];
            node * _next_node;
            do{
                _next_node = _node->next;
                if(!func(_node->value)){this->remove(index);}
                else{index++;}
            } while(_node = _next_node);
            return * this;
        }
    };
}
