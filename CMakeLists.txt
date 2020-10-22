cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0015 NEW)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set (PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/)

set(appName raymarch)
project(${appName})

include_directories(external)

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

file(GLOB ${NAME}_HPP_HEADERS *.hpp)
file(GLOB ${NAME}_CPP_SOURCES *.cpp)
file(GLOB ${NAME}_H_HEADERS *.h)
file(GLOB ${NAME}_C_SOURCES *.c)

add_executable(${appName} 
    ${${NAME}_HPP_HEADERS}
    ${${NAME}_H_HEADERS}
    ${${NAME}_CPP_SOURCES}
    ${${NAME}_C_SOURCES}    
)

