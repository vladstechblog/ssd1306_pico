extern "C"
{
#include "I2CWriteSpy.h"
#include "SSD1306.h"
}

#define TOTAL_BYTES_FITS_ON_SCREEN ((DISPLAY_HEIGHT * DISPLAY_WIDTH)/8)

#include "CppUTest/TestHarness.h"

TEST_GROUP(DisplayReset){
    i2c_inst_t* mock_i2c_address = (i2c_inst_t*)0xffffffff;
    data_t control_byte = DISPLAY_CONTROL_DATA_BYTE;
    data_t value_byte = 0;
    
    void setup(){
      I2CWriteSpy_Create();
      SSD1306_Create(mock_i2c_address);
    }

    void teardown(){
      SSD1306_Destroy();
    }
};

TEST(DisplayReset, ClearDisplayInvokesI2CWriteBlockingCorrectNumberOfTimes){
  I2CWriteSpy_SetExpectedBytes(control_byte, value_byte);
  SSD1306_clearDisplayMemory();

  LONGS_EQUAL(TOTAL_BYTES_FITS_ON_SCREEN, I2CWriteSpy_GetInvocationCounter());
}

TEST(DisplayReset, ReturnsCorrectBytesSent){
  I2CWriteSpy_SetExpectedBytes(control_byte, value_byte);
  LONGS_EQUAL(TOTAL_BYTES_FITS_ON_SCREEN * I2C_CHUNK_SIZE, SSD1306_clearDisplayMemory());
}
