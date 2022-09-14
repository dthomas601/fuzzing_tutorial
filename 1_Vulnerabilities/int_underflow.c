#include<stdio.h>
#include<limits.h>


int main(){
	
	int a =5, b=6;
	
	size_t len = a-b;
	
	printf("The length is %lu\n",len);
    printf("ULONG_MAX   :   %lu\n", (unsigned long) ULONG_MAX);


    char buf[len]; //unexpected buffer length
	
	return 0;
	
}