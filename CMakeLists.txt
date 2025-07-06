# cmake__version
cmake_minimum_required(VERSION 3.22.1)

# C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# project name
project(OFDM)

# Append the -g flag to the compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

# exe
file(GLOB SOURCES "src/*.cpp")
list(APPEND SOURCES "src/ErrorCorrection/hammingCode.cpp")
add_executable(ofdm ${SOURCES})

# include
target_include_directories(ofdm PRIVATE /usr/include)
target_include_directories(ofdm PRIVATE /usr/ErrorCorrection)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_library(FFTW3_LIB fftw3 REQUIRED)

# lib
target_link_libraries(ofdm PRIVATE ${FFTW3_LIB})


