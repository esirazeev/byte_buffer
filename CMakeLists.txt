cmake_minimum_required(VERSION 3.27.5)

project(byte-buffer-lib VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)

# create byte buffer lib
add_library(byte_buffer SHARED src/byte_buffer.cpp)
target_include_directories(byte_buffer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# download google test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# for Windows: prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# create byte buffer lib unit tests
enable_testing()
add_executable(byte_buffer_unit_test unit_test/byte_buffer_unit_test.cpp)
target_link_libraries(byte_buffer_unit_test PRIVATE GTest::gtest_main byte_buffer)

include(GoogleTest)
gtest_discover_tests(byte_buffer_unit_test)
