#include <stdbool.h>
#include "I2CWriteSpy.h"

static command_t expected_buffer[I2C_CHUNK_SIZE]; 

int i2c_write_blocking(i2c_inst_t* i2c, uint8_t addr, const uint8_t* src, size_t len, bool nostop){
  if (len != I2C_CHUNK_SIZE)
    return BUFFER_LENGTH_MISMATCH;
  
  if(src[0] == expected_buffer[0] && src[1] == expected_buffer[1]) 
    return len;

  return BUFFER_VALUE_MISMATCH;
}

void I2CWriteSpy_ResetBuffer(){
  expected_buffer[0] = 0xff;
  expected_buffer[1] = 0xff;
}

void I2CWriteSpy_Create(void){
  I2CWriteSpy_ResetBuffer();
}

void I2CWriteSpy_Destroy(void){}

void I2CWriteSpy_SetExpectedBytes(command_t control_byte, command_t command_byte){
  expected_buffer[0] = control_byte;
  expected_buffer[1] = command_byte;
}

command_t* I2CWriteSpy_GetBufferPtr(void){
  return expected_buffer;
}
