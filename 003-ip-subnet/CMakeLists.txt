cmake_minimum_required(VERSION 3.14)

project(NetworkingConcepts.utils)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${CMAKE_SOURCE_DIR}/include)

enable_testing()

add_executable(
  utils_test
  tests/utils_test.cc
)

target_link_libraries(
  utils_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(utils_test)
