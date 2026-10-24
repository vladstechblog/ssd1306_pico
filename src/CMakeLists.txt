add_library(displayDriver STATIC SSD1306.c)
target_include_directories(displayDriver PUBLIC include)
target_link_libraries(
  displayDriver
  PUBLIC
  pico_stdlib
)
