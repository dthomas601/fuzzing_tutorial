#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
int main(void)
{ 
    char *line = NULL;
    size_t size = 0;
    /* The loop below leaks memory as fast as it can */
    while(true) { 
        getline(&line, &size, stdin); /* New memory implicitly allocated */
        /* <do whatever> */

        char *data1 = "opengenus";
	    char *data2 = "new_data";
	    data2 = data1; // both data1 and data2 point to the same memory
	    free(data1); 
	    free(data2); // does not free original data2

	    int *ptr = (int *) malloc(sizeof(int));

        line = NULL;
    }
    return 0;
 }