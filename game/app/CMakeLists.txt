project(threesMain)

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

add_executable(
  cli_main
  ${CMAKE_SOURCE_DIR}/game/app/main.cc
)
target_link_libraries( cli_main game_src)

