#include <iostream>
#include <stdio.h>
#include <exception>
using namespace std;
namespace UltimateHandsome{
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
                pivot = (node **) malloc(sizeof(Array::_node));
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

        inline bool is_empty() { return this->length == 0; }
        
        
        void remove(unsigned int index) {
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
            if (pre_node &&
                next_node)/*the element which have just been removed is neither the first nor the last element*/{
                pre_node->next = next_node;
                next_node->previous = pre_node;
            } else if (pre_node)/*The last element have just been removed*/{
                this->last_node = pre_node;
                this->last_node->next = nullptr;
            } else if (next_node)/*The first element have just been removed*/{
                next_node->previous = nullptr;
            }
            if (index % 10 != 0 && pivot_number < this->pivot_length - 1)
                pivot_number++;
            if (index < (this->pivot_length - 1) * 10 || this->length <= 10)
                do {
                    this->pivot[pivot_number] = this->pivot[pivot_number]->next;
                } while (++pivot_number != this->pivot_length);
            delete _nodex;
            this->length--;
            if (!this->pivot[pivot_length - 1]) /*If the last tens are completely removed*/
                this->pivot = (node **) realloc(this->pivot, sizeof(Array::node) * --this->pivot_length);

        }


        void insert_after(unsigned int index, T value) {
            node *inserted_node = new node{value};
            if (this->length == 0 or this->length <= index)
                throw OutOfRange("Index is out of range", index);
            int offset;
            int pivot_number = (index - (offset = index % 10)) / 10;
            node *before_node = this->pivot[pivot_number];
            for (auto i = 0; i < offset; i++) {
                before_node = before_node->next;
            }
            node *after_node;
            if (index == this->length - 1)/*New item is inserted after the last item*/{
                before_node->next = inserted_node;
                inserted_node->previous = before_node;
                this->last_node = inserted_node;
            } else {
                after_node = before_node->next;
                before_node->next = inserted_node;
                after_node->previous = inserted_node;
                inserted_node->previous = before_node;
                inserted_node->next = after_node;
            }
            if (index % 10 != 0 && pivot_number < this->pivot_length - 1)
                pivot_number++;
            if (index < (this->pivot_length - 1) * 10 || this->length <= 10)
                do {
                    this->pivot[pivot_number] = this->pivot[pivot_number]->previous;
                } while (++pivot_number < this->pivot_length);
            if (this->length++ % 10 == 0) {
                this->pivot = (node **) realloc(this->pivot, sizeof(Array::node) * ++pivot_length);
                this->pivot[pivot_length - 1] = this->last_node;
            }
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
        void insert_before(unsigned int index, T value) {
            node *inserted_node = new node{value};
            if (this->length == 0 or this->length <= index)
                throw OutOfRange("Index is out of range", index);
            int offset;
            int pivot_number = (index - (offset = index % 10)) / 10;
            node *after_node = this->pivot[pivot_number];
            for (auto i = 0; i < offset; i++) {
                after_node = after_node->next;
            }
            node *before_node;
            if (index == 0)/*New item is inserted before the first item*/{
                after_node->previous = inserted_node;
                inserted_node->next = after_node;
            } else {
                before_node = after_node->previous;
                before_node->next = inserted_node;
                after_node->previous = inserted_node;
                inserted_node->previous = before_node;
                inserted_node->next = after_node;
            }
            if (index % 10 != 0 && pivot_number < this->pivot_length - 1)
                pivot_number++;
            if (index < (this->pivot_length - 1) * 10 || this->length <= 10)
                do {
                    this->pivot[pivot_number] = this->pivot[pivot_number]->previous;
                } while (++pivot_number < this->pivot_length);
            if (this->length++ % 10 == 0) {
                this->pivot = (node **) realloc(this->pivot, sizeof(Array::node) * ++pivot_length);
                this->pivot[pivot_length - 1] = this->last_node;
            }
        }

        void clear() {
            if (length == 0)return;
            node *inode = this->pivot[0];
            while (inode) {
                node *iinode = inode->next;
                delete (inode);
                inode = iinode;
            }
            free(this->pivot);
            this->length = 0;
            this->pivot_length = 0;

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

        void each(void (*func)(T &)) {
            if (this->length == 0) return;
            node *_nodex = this->pivot[0];
            do {
                func(_nodex->value);
            } while (_nodex = _nodex->next);
        }

        void each(void (*func)(T &, unsigned int)) {
            unsigned int index = 0;
            if (this->length == 0) return;
            node *_nodex = this->pivot[0];
            do {
                func(_nodex->value, index++);
            } while (_nodex = _nodex->next);
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

        void sort()/*A implementation of the quick sort algorithm*/{
           if(this->length <= 1) return;
           node * begin_node = this->pivot[0], * end_node = this->last_node;
           node * current_node;
           T buffer;
           while(true){
           	pharse1:
           		current_node = end_node;
           		while(current_node != begin_node){
           			current_node = current_node->previous;
           			if(current_node && current_node->value > end_node->value){
           				buffer = current_node->value;
						current_node->value = end_node->value;
						end_node->value = buffer;	
						goto pharse2;
					   }
				   }
				   if((end_node = end_node->previous) == begin_node) return;
           	pharse2:
           		current_node = begin_node;
           		while(current_node != end_node){
           			current_node = current_node->next;
           			if(current_node && current_node->value < begin_node->value){
           				buffer = current_node->value;
           				current_node->value = begin_node->value;
           				begin_node->value = buffer;
           				goto pharse1;
					   }
				   }
				   if((begin_node = begin_node->next) == end_node) return;
		   }
        }
    private:
        typedef struct _node {
            T value;
            _node *next;
            _node *previous;
        } node;
        node *last_node;
        unsigned int length = 0, pivot_length = 0;
        node **pivot;
    };
}
