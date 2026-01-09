cmake_minimum_required(VERSION 3.16)
project(search)

# shorter alias, default debug
if(NOT DEFINED Build)
    set(Build "d" CACHE STRING "Short build type: d=Debug, r=Release")
endif()

# map shorter onto standard
if(Build STREQUAL "r")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
elseif(Build STREQUAL "d")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
else()
    message(FATAL_ERROR "Unknown Build type: ${Build}. Use 'd' or 'r'.")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON) # for ninja

# outputs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# different levels
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  add_compile_options(
    -O3 -Wshadow -Wunused -Wextra
  )
else()
  set(SANITIZE_FLAGS "-fsanitize=undefined")
  add_compile_options(
    -g -O0 -Wshadow -Wunused -Wextra
    ${SANITIZE_FLAGS})
  add_link_options(${SANITIZE_FLAGS})
  add_compile_definitions(DEBUG)
endif()


file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(
  ${PROJECT_NAME}
  cli.cpp
  ${SOURCES}
)
