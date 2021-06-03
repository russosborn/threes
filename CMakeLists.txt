cmake_minimum_required(VERSION 3.14)

# project name
project(threes)

#c++ requirements
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#gtest dependency
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#########################
## threes libs

add_library(GameSrc)
target_sources(GameSrc PUBLIC ${CMAKE_SOURCE_DIR}/game/src/Board.cc)


##########################

## test setup
enable_testing()

add_executable(
  example_test
  ${CMAKE_SOURCE_DIR}/test/example.cc
)
target_link_libraries(
  example_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(example_test)
