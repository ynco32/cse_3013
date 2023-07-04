#include "Header.h"

int main(int argc, const char * argv[]) {
    int t; //testcase
    scanf("%d", &t); //input testcase
    //repeat t-times
    for (int k = 0; k < t; k++){
        int page; //number of pages
        scanf("%d", &page); // get the page
        int num_list[10] = { 0, }; // array for number of 0-9 (eg.num[1] means number of 1)
        
        count_num(page, num_list); // do conting --> function is in count.c
        print_num(num_list); //do printing --> function is in print.c
    }
    
    return 0;
}
