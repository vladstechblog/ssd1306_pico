#include <stdbool.h>
#include "I2CWriteSpy.h"

static command_t expected_buffer[I2C_CHUNK_SIZE]; 
static int invocation_counter = 0xff;

int i2c_write_blocking(i2c_inst_t* i2c, uint8_t addr, const uint8_t* src, size_t len, bool nostop){
  if (len != I2C_CHUNK_SIZE)
    return BUFFER_LENGTH_MISMATCH;
  
  if(src[0] == expected_buffer[0] && src[1] == expected_buffer[1]){
    invocation_counter++;
    return len;
  }

  return BUFFER_VALUE_MISMATCH;
}

static void reset_buffer(){
  expected_buffer[0] = 0xff;
  expected_buffer[1] = 0xff;
}

static void reset_invodation_counter(void){ invocation_counter = 0; }

int I2CWriteSpy_GetInvocationCounter(void){ return invocation_counter; }

void I2CWriteSpy_Create(void){
  reset_buffer();
  reset_invodation_counter();
}

void I2CWriteSpy_Destroy(void){}

void I2CWriteSpy_SetExpectedBytes(command_t control_byte, command_t command_byte){
  expected_buffer[0] = control_byte;
  expected_buffer[1] = command_byte;
}

command_t* I2CWriteSpy_GetBufferPtr(void){
  return expected_buffer;
}
