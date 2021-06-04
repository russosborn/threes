cmake_minimum_required(VERSION 3.14)

# project name
project(threes)

#c++ requirements
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)


#########################
## threes libs
add_subdirectory( game )
add_subdirectory( test )

##########################

