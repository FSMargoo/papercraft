cmake_minimum_required(VERSION 3.10)

project(libostr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the header files to the project
file(GLOB_RECURSE HEADERS "include/*.h")

# Add the source files to the project
file(GLOB_RECURSE SOURCES "source/*.cpp")

include_directories("./include")

# Create the static library
add_library(libostr STATIC ${HEADERS} ${SOURCES})