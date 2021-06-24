project(threesGame)

# Where Google Test's .h files can be found.
set(threesGame_build_include_dirs
  "${threesGame_SOURCE_DIR}/src"
  "${threesGame_SOURCE_DIR}"
  )
include_directories(${threesGame_build_include_dirs})

add_subdirectory( app )

add_library(game_src)
target_sources(game_src PUBLIC ${CMAKE_SOURCE_DIR}/game/src/Board.cc ${CMAKE_SOURCE_DIR}/game/src/CardSequence.cc)
