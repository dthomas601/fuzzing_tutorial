#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stddef.h>

using namespace std;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {

  
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