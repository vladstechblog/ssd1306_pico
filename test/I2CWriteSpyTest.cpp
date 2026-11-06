extern "C"
{
#include "I2CWriteSpy.h"
#include "SSD1306.h"

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
  command_t* expected_buffer_ptr = I2CWriteSpy_GetExpectedI2CPayloadPointer();
  BYTES_EQUAL(0xff, *expected_buffer_ptr);
  BYTES_EQUAL(0xff, *(expected_buffer_ptr + 1));
}

TEST(I2CWriteSpy, SetExpectedBytes){
  command_t expected_control_byte = 0xaa;
  command_t expected_command_byte = 0xbb;
  command_t* expected_buffer_ptr = I2CWriteSpy_GetExpectedI2CPayloadPointer();
  I2CWriteSpy_SetExpectedI2CPayloadBytes(expected_control_byte, expected_command_byte);
  BYTES_EQUAL(expected_control_byte, *expected_buffer_ptr);
  BYTES_EQUAL(expected_command_byte, *(expected_buffer_ptr + 1));
}

TEST(I2CWriteSpy, I2CWriteBlockingTestWithCorrectBufferValues){
  command_t test_buffer[] = {control_byte, command_byte};
  I2CWriteSpy_SetExpectedI2CPayloadBytes(control_byte, command_byte);
  LONGS_EQUAL(sizeof(test_buffer), SSD1306_displayPowerOn());
}

TEST(I2CWriteSpy, I2CWriteBlockingTestWithValueMistmatch){
  I2CWriteSpy_SetExpectedI2CPayloadBytes(control_byte, 0xff);
  LONGS_EQUAL(BUFFER_VALUE_MISMATCH, SSD1306_displayPowerOn());
}

TEST(I2CWriteSpy, I2CWriteBlockingTestLengthMismatch){
  command_t test_buffer[] = {control_byte, command_byte};
  I2CWriteSpy_SetExpectedI2CPayloadBytes(control_byte, command_byte);
  int return_value = i2c_write_blocking(mock_i2c_address, DISPLAY_I2C_ADDRESS, test_buffer, 5, false);
  LONGS_EQUAL(BUFFER_LENGTH_MISMATCH, return_value);
}

TEST(I2CWriteSpy, InvocationCounterClearedOnCreate){
  LONGS_EQUAL(0, I2CWriteSpy_GetInvocationCounter());
}

TEST(I2CWriteSpy, InvocationCounterIcrementedOnInvocations){
  int expected_invocations = 10;

  I2CWriteSpy_SetExpectedI2CPayloadBytes(control_byte, command_byte);

  for (int current = 0; current < expected_invocations; current++){
    SSD1306_displayPowerOn();    
  }

  LONGS_EQUAL(expected_invocations, I2CWriteSpy_GetInvocationCounter());
}


TEST_GROUP(MultibyteBuffer){
  data_t fake_buffer[4] = {1,2,3,4};
  size_t fake_buffer_size = sizeof(fake_buffer);
  data_t fake_payload[I2C_PAYLOAD_SIZE] = { DISPLAY_CONTROL_DATA_BYTE, fake_buffer[0]};
  
  void setFakeBuffer(data_t* new_buffer, size_t new_size){
    I2CWriteSpy_SetExpectedBuffer(new_buffer);
    I2CWriteSpy_SetExpectedBufferSize(new_size);
  }

  void setFakePayload(){
    setFakeBuffer(fake_buffer, fake_buffer_size);
    I2CWriteSpy_SetExpectedI2CPayloadBytes(fake_payload[0], fake_payload[1]);
  }

  void setup(){
    I2CWriteSpy_Create();
  }
  
  void teardown(){
  }
};

TEST(MultibyteBuffer, SetExpectedBufferSizeGetterAndSetter){
  size_t expected_size = 5;
  I2CWriteSpy_SetExpectedBufferSize(expected_size);
  LONGS_EQUAL(expected_size, I2CWriteSpy_GetExpectedBufferSize());
}

TEST(MultibyteBuffer, CreateSetExpectedBufferPointerToNullAndSizeToZero){
  POINTERS_EQUAL(NULL, I2CWriteSpy_GetExpectedBuffer());
  LONGS_EQUAL(0, I2CWriteSpy_GetExpectedBufferSize());
}

TEST(MultibyteBuffer, I2CWriteSpySetsCorrectExpectedBuffer){
  setFakeBuffer(fake_buffer, fake_buffer_size);
  POINTERS_EQUAL(fake_buffer, I2CWriteSpy_GetExpectedBuffer());
}

TEST(MultibyteBuffer, DestroySetsExpectedBufferToNullAndSizeToZero){
  setFakeBuffer(fake_buffer, fake_buffer_size);
  I2CWriteSpy_Destroy();
  POINTERS_EQUAL(NULL, I2CWriteSpy_GetExpectedBuffer());
  LONGS_EQUAL(0, I2CWriteSpy_GetExpectedBufferSize());
}

TEST(MultibyteBuffer, I2CWriteSpyUpdatesExpectedI2CPayloadWhenExpectedBufferIsSet){
  setFakePayload();
  i2c_write_blocking(mock_i2c_address, DISPLAY_I2C_ADDRESS, fake_payload, I2C_PAYLOAD_SIZE, false);
  data_t* expected_i2c_payload_ptr = I2CWriteSpy_GetExpectedI2CPayloadPointer();
  BYTES_EQUAL(fake_buffer[1], expected_i2c_payload_ptr[1]);
}

TEST(MultibyteBuffer, PointerIsNotAdvancedOnExpectedBufferSizeOne){
  setFakePayload();
  I2CWriteSpy_SetExpectedBufferSize(1);
  i2c_write_blocking(mock_i2c_address, DISPLAY_I2C_ADDRESS, fake_payload, I2C_PAYLOAD_SIZE, false);
  data_t* expected_i2c_payload_ptr = I2CWriteSpy_GetExpectedI2CPayloadPointer();
  BYTES_EQUAL(fake_buffer[0], expected_i2c_payload_ptr[1]);
}

TEST(MultibyteBuffer, ExpectedBytesAreCorrectAtTheEndOfTheBuffer){
  setFakePayload();
  
  for(int i = 0; i < fake_buffer_size; i++){
    fake_payload[1] = fake_buffer[i];
    i2c_write_blocking(mock_i2c_address, DISPLAY_I2C_ADDRESS, fake_payload, I2C_PAYLOAD_SIZE, false);
  }

  data_t* expected_i2c_payload_ptr = I2CWriteSpy_GetExpectedI2CPayloadPointer();
  BYTES_EQUAL(fake_buffer[fake_buffer_size-1], expected_i2c_payload_ptr[1]);
}