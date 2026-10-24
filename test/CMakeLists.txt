include(include_local_cpputest_git_repo.cmake)

get_target_property(DRIVER_SOURCES displayDriver SOURCES)
get_target_property(DRIVER_SOURCE_DIR displayDriver SOURCE_DIR)
get_target_property(DRIVER_INCLUDE_DIR displayDriver INCLUDE_DIRECTORIES)

add_executable(
  displayTestRunner
  displayTestRunner.cpp
  "${DRIVER_SOURCE_DIR}/${DRIVER_SOURCES}"
  util_i2c_write_blocking.c
)
target_include_directories(displayTestRunner PUBLIC include ${DRIVER_INCLUDE_DIR})
target_link_libraries(
  displayTestRunner
  PUBLIC
  CppUTest
  CppUTestExt
)
