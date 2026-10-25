#include <CppUTest/CommandLineTestRunner.h>
#include "I2CWriteSpyTest.cpp"
#include "DisplayResetTest.cpp"

int main(int argc, char* argv[]){
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
