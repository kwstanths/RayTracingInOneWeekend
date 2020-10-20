cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0015 NEW)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set (PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/)

set(appName raymarch)
project(${appName})


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

