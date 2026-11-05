extern "C"
{
#include "I2CWriteSpy.h"
#include "SSD1306.h"

static int callback_invocation_counter = 0;
static const data_t test_buffer[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 }; 
static int test_data_size = sizeof(test_buffer);

static void fakeCallback(){}

static void testCallback(){
  callback_invocation_counter++;
  data_t* next_bytes = I2CWriteSpy_GetBufferPtr();
  next_bytes[0] = DISPLAY_CONTROL_DATA_BYTE;
  next_bytes[1] = test_buffer[callback_invocation_counter];
}

i2c_inst_t* mock_i2c_address = (i2c_inst_t*)0xffffffff;

} //end of extern "C"

#include "CppUTest/TestHarness.h"

TEST_GROUP(I2CWriteSpy){
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


TEST_GROUP(UpdateValuesCallback){
  SetExpectedBytesCallbackPtr fake_callback = (SetExpectedBytesCallbackPtr) 0xcc;
  void setup(){
    callback_invocation_counter = 0;
    I2CWriteSpy_Create();
  }

  void teardown(){
    I2CWriteSpy_Destroy();
  }
};


TEST(UpdateValuesCallback, getCallBackRerturnsCorrectValue){
  I2CWriteSpy_setCallbackPointer(fake_callback);
  LONGS_EQUAL(fake_callback, I2CWriteSpy_getCallbackPointer());
}

TEST(UpdateValuesCallback, DestroySetsCallbackToNull){
  I2CWriteSpy_setCallbackPointer(fake_callback);
  I2CWriteSpy_Destroy();
  LONGS_EQUAL(0, I2CWriteSpy_getCallbackPointer());
}

TEST(UpdateValuesCallback, CreateSetsCallbackToNull){
  I2CWriteSpy_setCallbackPointer(fake_callback);
  I2CWriteSpy_Create();
  LONGS_EQUAL(0, I2CWriteSpy_getCallbackPointer());
}

TEST(UpdateValuesCallback, setCallbackPointerSetsCorrectPointer){
  I2CWriteSpy_setCallbackPointer(fakeCallback);
  POINTERS_EQUAL(fakeCallback, I2CWriteSpy_getCallbackPointer());
}

TEST(UpdateValuesCallback, CallbackIcrementsCallbackCounterAndSetsNextByte){
  I2CWriteSpy_setCallbackPointer(testCallback);
  data_t payload[] = {DISPLAY_CONTROL_DATA_BYTE, test_buffer[0]}; 
  I2CWriteSpy_SetExpectedBytes(DISPLAY_CONTROL_DATA_BYTE, test_buffer[0]);
  i2c_write_blocking(mock_i2c_address, DISPLAY_I2C_ADDRESS, payload, I2C_CHUNK_SIZE, false);
  data_t* next_bytes = I2CWriteSpy_GetBufferPtr();

  LONGS_EQUAL(1, callback_invocation_counter);
  BYTES_EQUAL(DISPLAY_CONTROL_DATA_BYTE, next_bytes[0]);
  BYTES_EQUAL(test_buffer[1], next_bytes[1]);
}
