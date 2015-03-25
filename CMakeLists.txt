cmake_minimum_required (VERSION 2.6)

project(msgpack-filter)
set (MsgpackFilter_VERSION_MAJOR 0)
set (MsgpackFilter_VERSION_MINOR 1)

configure_file (
  "${PROJECT_SOURCE_DIR}/MsgpackFilter.h.in"
  "${PROJECT_BINARY_DIR/}MsgpackFilter.h"
  )

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.45.0 COMPONENTS timer system) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
endif()

include_directories("${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/msgpack-c/include")

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ./msgpack-c)

add_executable(main main.cpp)
target_link_libraries(main ${Boost_LIBRARIES})
