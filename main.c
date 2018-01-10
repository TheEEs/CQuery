#include <stdio.h>
#include<memory.h>
#include<stdlib.h>

void swap(int * a, int * b){
    int c = *a;
    *a = *b;
    *b = c;
}

void shift(int * segment, int len){
    int last_value = segment[--len];
    while(len)
        segment[len] = segment[--len];
    *segment = last_value;
}

int quick_sort(int * array,int len){
    if (len > 1){
        int i= -1 ,j=0;
        int pivot = array[len - 1];
        //i is used to mark the position that all elements before i is less than array[i]
        for(; j < len-1; j++){
            if(array[j] < pivot){
                i++;
                if(i != j)
                    swap(array + j, array + i);
            }
        }
        /*printf("------------\n");
        for(int l = 0;l<len;l++)
            printf("%d\n",array[l]);
         */
        shift(array + i + 1,len - i - 1);
        /*printf("------------\n");
        for(int l = 0;l<len;l++)
            printf("%d\n",array[l]);*/
        if(i > -1 && i< len - 2) {
            quick_sort(array, i + 1);
            quick_sort(array + i + 2, len - i - 2);
        }
        else if(i == -1) //if there are no elements less than the pivot
            quick_sort(array + 1,len - 1);
        else if(i == len - 2)// if there are no elements greater than the pivot
            quick_sort(array,len - 1);
        return 1;
    }
    return 0;
}

int main() {
    int a[] = {1,4,-2,10,9,3,2,6};
    int len = sizeof(a) / sizeof(int);
    quick_sort(a, len);
    for(int i = 0;i<len;i++)
        printf("%d\n",a[i]);
    printf("Hello, World!\n");
    return 0;
}