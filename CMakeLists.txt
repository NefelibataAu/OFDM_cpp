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

# add file
file(GLOB SOURCES "src/*.cpp")
list(APPEND SOURCES "src/ErrorCorrection/hammingCode.cpp")

# include_directories(/Applications/MATLAB_R2022b.app/extern/include)
# include_directories(${CMAKE_SOURCE_DIR}/include)
find_library(FFTW3_LIB fftw3 REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# exe
add_executable(ofdm ${SOURCES})
target_include_directories(ofdm PRIVATE /usr/include)
# lib
target_link_libraries(ofdm PRIVATE ${FFTW3_LIB})
# target_link_libraries(ofdm PRIVATE  ${CMAKE_SOURCE_DIR}/lib/libfftw3.dylib)
# target_link_libraries(ofdm PRIVATE  /Applications/MATLAB_R2022b.app/bin/maci64/libmat.dylib)
# target_link_libraries(ofdm PRIVATE  /Applications/MATLAB_R2022b.app/bin/maci64/libmx.dylib)

