#include <CppUTest/CommandLineTestRunner.h>
#include "I2CWriteSpyTest.cpp"
#include "ClearDisplayTest.cpp"
#include "SSD1306WriteDataBufferTest.cpp"

int main(int argc, char* argv[]){
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
