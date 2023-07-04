#include "Header.h"

int get_digit(int n){
    char str[11]; //temporal string variable
    sprintf(str, "%d", n); //convert nubmer to string
    //printf("\n---str: %s\n", str);
    int len = (int) strlen(str); // get length of string
    return len;
}
