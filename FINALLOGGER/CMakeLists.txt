#Minimum version required
cmake_minimum_required(VERSION 2.8.9)
project(Logger C CXX)
#include c++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#look for additional libraries
find_package(Threads REQUIRED)

#bring all the headers into our
include_directories(include)

#CppUnit adding stuff
file(GLOB TESTSOURCE "include/*Case.h")

#add_executable(Logger Main.cpp ${UnitTests_SRCS})

#GLOB is used for wildcard additions of all source files
file(GLOB SOURCES "src/*.cpp")

#adding executable
add_executable(Logger ${SOURCES} ${TESTSOURCE})
#add_executable(Logger ${TESTSOURCE})


#linking the library
target_link_libraries(Logger ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Logger cppunit)

