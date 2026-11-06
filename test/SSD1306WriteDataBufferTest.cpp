extern "C"
{
#include "I2CWriteSpy.h"
#include "SSD1306.h"
#include "pico/error.h"

} //end of extern "C"

#include "CppUTest/TestHarness.h"


TEST_GROUP(WriteDataBuffer){

  data_t test_buffer[8] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
  size_t test_buffer_size = sizeof(test_buffer);

  void basicI2CSpyBufferSetup(){
    I2CWriteSpy_SetExpectedI2CPayloadBytes(DISPLAY_CONTROL_DATA_BYTE, test_buffer[0]);
    I2CWriteSpy_SetExpectedBuffer(test_buffer);
    I2CWriteSpy_SetExpectedBufferSize(test_buffer_size);
  }
  
  void setup(){
    I2CWriteSpy_Create();
    SSD1306_Create(mock_i2c_address);
  }
  
  void teardown(){
    I2CWriteSpy_Destroy();
    SSD1306_Destroy();
  }
};

TEST(WriteDataBuffer, SendDataBufferReturnsCorrectBytesSent){
  basicI2CSpyBufferSetup();
  int actual_return = SSD1306_sendDataBuffer(test_buffer, test_buffer_size);
  LONGS_EQUAL(test_buffer_size * I2C_PAYLOAD_SIZE, actual_return);
}

TEST(WriteDataBuffer, NullBufferPtrReturnsPicoErrorInvliadArgument){
  int actual_return = SSD1306_sendDataBuffer(NULL, 10);
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, actual_return);
}

TEST(WriteDataBuffer, ZeroBufferSizePicoErrorInvliadArgument){
  int actual_return = SSD1306_sendDataBuffer(test_buffer, 0);
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, actual_return);
}

TEST(WriteDataBuffer, BufferSizeGreaterThanMaxPicoErrorInvliadArgument){
  int actual_return = SSD1306_sendDataBuffer(test_buffer, DISPLAY_MAX_BYTES + 1);
  LONGS_EQUAL(PICO_ERROR_INVALID_ARG, actual_return);
}