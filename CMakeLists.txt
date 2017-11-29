cmake_minimum_required(VERSION 2.8)
project(a5)

find_package (Threads REQUIRED)
if (EXISTS "${CMAKE_SOURCE_DIR}/minisat")
  add_subdirectory(minisat)
else()
  message (FATAL_ERROR "MiniSat not found. Use `git clone https://github.com/agurfinkel/minisat` to check it out.")
endif()


## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -lpthread -Wall ${CMAKE_CXX_FLAGS}")

# expose minisat header files
include_directories(${CMAKE_SOURCE_DIR}/minisat)
include_directories(${CMAKE_SOURCE_DIR}/include)

# create the main executable
## add additional .cpp files if needed
add_executable(a5 a5.cpp)
# link MiniSAT libraries
target_link_libraries(a5 minisat-lib-static)

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(a5 PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(a5 "${CMAKE_THREAD_LIBS_INIT}")
endif()


