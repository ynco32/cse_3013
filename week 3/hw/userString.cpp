

#include "userString.h"


Str::Str(int leng){
    //length must be positive int or 0
    if (leng >= 0){
        len = leng;
        //set private parameter len
    }
    
    else {
        //return error
        cout << "ERROR: Invalid length" << endl;
    }
}

Str::Str(const char *neyong){
    //get size from input
    int size = (int)strlen(neyong);
    //length should be size+1 as null at the end
    str = new char[size+1];
    //set private parameter len
    len = size;
    //set private parameter str
    strcpy(str, neyong);
}

Str::~Str(){
    //delete str in the class
    delete[] str;
}

int Str::length(void){
    //return len in the class
    return len;
}

char* Str::contents(void){
    //return str in the class
    return str;
}

//when input is Str class
int Str::compare(class Str& a){
    //return value of strcmp of str and a.contents()(str class)
    return strcmp(str, a.contents());
}

//when input is string
int Str::compare(const char *a){
    //return value of strcmp of str and a(string)
    return strcmp(str, a);
}

//when input is string
void Str::operator=(const char *a){
    //calculate lenght from input
    int size = (int)strlen(a);
    //delete str in the class
    delete[] str;
    //set private parameter len
    len = size;
    //length should be size+1 as null at the end
    str = new char[size+1];
    //set private parameter str
    strcpy(str, a);
}

//when input is Str class:
void Str::operator=(class Str& a){
    //calculate lenght from input
    int size = a.length();
    //delete str in the class
    delete[] str;
    //length should be size+1 as null at the end
    str = new char[size+1];
    //set private parameter len
    len = size;
    //set private parameter str
    strcpy(str, a.contents());
}
