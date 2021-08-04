cmake_minimum_required(VERSION 3.14)

# project name
project(threes)

#c++ requirements
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

IF(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
  add_compile_options(-Wall -Wextra -pedantic -Werror)
    
ENDIF(CMAKE_COMPILER_IS_GNUCC)


#########################
## threes libs
add_subdirectory( game )
add_subdirectory( test )

##########################

