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

find_package(spdlog REQUIRED)

# library
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library(
  ${PROJECT_NAME}
  ${SOURCES}
)

#packages
include(FetchContent)

## spdlog
find_package(spdlog REQUIRED)

## gtest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

## pugixml
FetchContent_Declare(
  pugixml
  GIT_REPOSITORY https://github.com/zeux/pugixml.git
  GIT_TAG master
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(pugixml)


## cxxopts
FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG master
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(cxxopts)

## boost regex
find_package(Boost REQUIRED COMPONENTS regex)


# link step
target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  spdlog::spdlog
  gtest
  pugixml::pugixml
  Boost::regex
)

# executable
## cli
add_executable(cli cli.cpp)
target_link_libraries(cli PRIVATE ${PROJECT_NAME})

## generate
add_executable(generate generate.cpp)
target_link_libraries(generate PRIVATE ${PROJECT_NAME})


# tests
enable_testing()
include(GoogleTest)

file(GLOB tests ${PROJECT_SOURCE_DIR}/tests/*.cpp)
foreach(test IN LISTS tests)
  get_filename_component(name ${test} NAME_WE)
  add_executable(${name} ${test})
  target_link_libraries(${name} PRIVATE ${PROJECT_NAME} GTest::gtest_main)
  gtest_discover_tests(${name})
endforeach()
