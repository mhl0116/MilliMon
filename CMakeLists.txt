cmake_minimum_required(VERSION 3.4...3.24)

project(MilliANA LANGUAGES CXX)

set(CMAKE_CXX_STANDARD
    17
    CACHE STRING "C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

# Locate the ROOT package and define a number of useful targets and variables.
find_package(ROOT 6.26 CONFIG REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#git clone https://github.com/gabime/spdlog.git
#cd spdlog && mkdir build && cd build
#cmake -DCMAKE_INSTALL_PREFIX="/home/hmei/.local" ..
#make -j && make install
set(spdlog_DIR /home/hmei/.local/lib64/cmake/spdlog/) 
find_package(spdlog REQUIRED)

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(apps)
