# SSD1306 TDD

A minimal implementation of SSD1306 driver using Test Driven Development (TDD).

Designed for 128x64 models.

Font is not included, since it is not the responsiblitiy of the driver.

See demo link below for a minimal presenter layer implementation.


## Using The Driver

- include the project in the root CMake list file like so:

```
include(FetchContent)
FetchContent_Declare(
  ssd1306_pico
  GIT_REPOSITORY https://github.com/vladstechblog/ssd1306_pico.git
  GIT_TAG develop
)

FetchContent_MakeAvailable(ssd1306_pico)
```

See the [demo project](https://github.com/vladstechblog/ssd1306_pico_demo.git)


## Building and Running Tests

- building display test runner

```shell
git clone https://github.com/vladstechblog/ssd1306_tdd
cd ssd1306_tdd
cmake -B build_host -DPICO_PLATFORM=host
cmake --build build_host/ -t displayTestRunner
./build_host/test/displayTestRunner
```