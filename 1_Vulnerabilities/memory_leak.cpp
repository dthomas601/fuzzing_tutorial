#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <cstring>
int main(void)
{

    while(true){

        char* str1 = new char [30];

        char* str2 = new char [20000];

        strcpy(str1, "Memory leak");

        str2 = str1; // Bad! Now the bytes are impossible to free.

        delete [] str2; 
        //delete [] str1; // Possible access violation. What a disaster!

    }

}
