#include<stdio.h>
#include<assert.h>
#include<string.h>

int main(int argc, char** argv){

        char data[100];
        printf("Enter a string\n");
        scanf("%s",data);

        if (sizeof(data) >= 4){

                // Test to find the input to get to the assert statement
                if(data[0] == 'p') {
                        if(data[1] == 'o') {
                                if(data[2] =='p') {
                                        if(data[3] == '!') {
                                                assert(0);
                                        }
                                }
                        }
                }
        }
        return 0;
}