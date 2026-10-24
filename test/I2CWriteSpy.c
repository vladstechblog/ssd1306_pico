#include <stdbool.h>
#include "I2CWriteSpy.h"

static command_t* buffer_ptr = (command_t*)-1;

void I2CWriteSpy_Create(void){

}

void I2CWriteSpy_Destroy(void){

}

command_t* I2CWriteSpy_GetBufferPtr(void){
  return buffer_ptr;
}
