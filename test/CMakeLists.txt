project(threesTest)

# Where Google Test's .h files can be found.
set(threesGame_build_include_dirs
  "${threesGame_SOURCE_DIR}"
  "${threesGame_SOURCE_DIR}/src"
  )
include_directories(${threesGame_build_include_dirs})

message(STATUS "include='${threesGame_build_include_dirs}'")

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

#gtest dependency
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

## test setup
enable_testing()

add_executable(
  example_test
  ${CMAKE_SOURCE_DIR}/test/GameStateTests.cc
  ${CMAKE_SOURCE_DIR}/test/CardSequenceTests.cc
)
target_link_libraries( example_test gtest_main game_src)

include(GoogleTest)
gtest_discover_tests(example_test)
