#include <stdbool.h>
#include "I2CWriteSpy.h"

static command_t expected_i2c_payload[I2C_PAYLOAD_SIZE]; 
static int valid_invocation_counter = 0xff;
static data_t* expected_buffer_ptr = (data_t*) 0xee;
static size_t expected_buffer_size = 0;

void advance_i2c_expected_payload(void){
  if (valid_invocation_counter < expected_buffer_size){
    expected_i2c_payload[0] = DISPLAY_CONTROL_DATA_BYTE;
    expected_i2c_payload[1] = expected_buffer_ptr[valid_invocation_counter];
  }
}

int i2c_write_blocking(i2c_inst_t* i2c, uint8_t addr, const uint8_t* src, size_t len, bool nostop){
  if (len != I2C_PAYLOAD_SIZE)
    return BUFFER_LENGTH_MISMATCH;
   
  if(src[0] == expected_i2c_payload[0] && src[1] == expected_i2c_payload[1]){
    valid_invocation_counter++;
    if(expected_buffer_ptr != NULL)
      advance_i2c_expected_payload(); 
    return len;
  }

  return BUFFER_VALUE_MISMATCH;
}

static void reset_buffer(){
  expected_i2c_payload[0] = 0xff;
  expected_i2c_payload[1] = 0xff;
}

static void reset_invodation_counter(void){ valid_invocation_counter = 0; }

int I2CWriteSpy_GetInvocationCounter(void){ return valid_invocation_counter; }

void I2CWriteSpy_Create(void){
  expected_buffer_ptr = NULL;
  expected_buffer_size = 0;
  reset_buffer();
  reset_invodation_counter();
}

void I2CWriteSpy_Destroy(void){
  expected_buffer_ptr = NULL;
  expected_buffer_size = 0;
}

void I2CWriteSpy_SetExpectedI2CPayloadBytes(command_t control_byte, command_t command_byte){
  expected_i2c_payload[0] = control_byte;
  expected_i2c_payload[1] = command_byte;
}

command_t* I2CWriteSpy_GetExpectedI2CPayloadPointer(void){
  return expected_i2c_payload;
}

data_t* I2CWriteSpy_GetExpectedBuffer(void){
  return expected_buffer_ptr;
}

void I2CWriteSpy_SetExpectedBuffer(data_t* new_buffer_pointer){
  expected_buffer_ptr = new_buffer_pointer;  
}

void I2CWriteSpy_SetExpectedBufferSize(size_t new_size){
  expected_buffer_size = new_size;
}

size_t I2CWriteSpy_GetExpectedBufferSize(void){ return expected_buffer_size; }