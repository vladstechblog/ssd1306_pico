extern "C"
{
#include "I2CWriteSpy.h"
#include "SSD1306.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(I2CWriteSpy){
    i2c_inst_t* mock_i2c_address = (i2c_inst_t*)0xffffffff;
    command_t control_byte = DISPLAY_CONTROL_CMD_BYTE;
    command_t command_byte = DISPLAY_CMD_POWER_ON;
    
    void setup(){
      I2CWriteSpy_Create();
      SSD1306_Create(mock_i2c_address);
    }

    void teardown(){
      SSD1306_Destroy();
    }
};


TEST(I2CWriteSpy, CreateResetsExpectedBufferValues){
  command_t* expected_buffer_ptr = I2CWriteSpy_GetBufferPtr();
  BYTES_EQUAL(0xff, *expected_buffer_ptr);
  BYTES_EQUAL(0xff, *(expected_buffer_ptr + 1));
}

TEST(I2CWriteSpy, SetExpectedBytes){
  command_t expected_control_byte = 0xaa;
  command_t expected_command_byte = 0xbb;
  command_t* expected_buffer_ptr = I2CWriteSpy_GetBufferPtr();
  I2CWriteSpy_SetExpectedBytes(expected_control_byte, expected_command_byte);
  BYTES_EQUAL(expected_control_byte, *expected_buffer_ptr);
  BYTES_EQUAL(expected_command_byte, *(expected_buffer_ptr + 1));
}

TEST(I2CWriteSpy, I2CWriteBlockingTestWithCorrectBufferValues){
  command_t test_buffer[] = {control_byte, command_byte};
  I2CWriteSpy_SetExpectedBytes(control_byte, command_byte);
  LONGS_EQUAL(sizeof(test_buffer), SSD1306_displayPowerOn());
}

TEST(I2CWriteSpy, I2CWriteBlockingTestWithValueMistmatch){
  I2CWriteSpy_SetExpectedBytes(control_byte, 0xff);
  LONGS_EQUAL(BUFFER_VALUE_MISMATCH, SSD1306_displayPowerOn());
}

TEST(I2CWriteSpy, I2CWriteBlockingTestLengthMismatch){
  command_t test_buffer[] = {control_byte, command_byte};
  I2CWriteSpy_SetExpectedBytes(control_byte, command_byte);
  int return_value = i2c_write_blocking(mock_i2c_address, DISPLAY_I2C_ADDRESS, test_buffer, 5, false);
  LONGS_EQUAL(BUFFER_LENGTH_MISMATCH, return_value);
}

TEST(I2CWriteSpy, InvocationCounterClearedOnCreate){
  LONGS_EQUAL(0, I2CWriteSpy_GetInvocationCounter());
}

TEST(I2CWriteSpy, InvocationCounterIcrementedOnInvocations){
  int expected_invocations = 10;

  I2CWriteSpy_SetExpectedBytes(control_byte, command_byte);

  for (int current = 0; current < expected_invocations; current++){
    SSD1306_displayPowerOn();    
  }

  LONGS_EQUAL(expected_invocations, I2CWriteSpy_GetInvocationCounter());
}


