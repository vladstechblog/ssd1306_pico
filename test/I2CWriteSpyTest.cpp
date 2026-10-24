extern "C"
{
#include "I2CWriteSpy.h"
}

#include "CppUTest/TestHarness.h"

TEST_GROUP(I2CWriteSpy){
    void setup(){
      //I2CWriteSpy_Create();
    }

    void teardown(){
       //I2CWriteSpy_Destroy();
    }
};

TEST(I2CWriteSpy, GetBufferPtr){
  POINTERS_EQUAL(-1, I2CWriteSpy_GetBufferPtr());
}

