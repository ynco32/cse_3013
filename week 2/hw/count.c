#include "Header.h"



//get n and address of array
void count_num(int page, int* num_list){
    //every page starts from 1. Count from 1 to n
    for (int i = 1; i < page+1; i++){
        int tmp = i; //save page number
        int digit = get_digit(i); //how many digits does i-th page have?
        
        //printf("digit is: %d\n\n", digit); //test
        
        //repeat for each digit (ex. if i=123, repeat 3 times)
        for (int j = 0; j < digit; j++){
            int index;
            index = tmp % 10; //get the last digit
            tmp = tmp / 10; // divide tmp to get the next digit number
            num_list[index]++; //count
        }
    }
}
