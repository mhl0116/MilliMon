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

# Create a shared library.
# Passing cmake targets such as `ROOT::RIO` as dependencies (rather than plain
# library names for example via ${ROOT_LIBRARIES}) ensures that properties such as required
# include directories and C++ standard are propagated to our libraries or executables.
# Note: To ensure compatibility with Cling, targets *must* be compiled using the
# same C++ standard as ROOT was compiled with.

#add_library(Event SHARED Event.cxx)
#target_link_libraries(Event PUBLIC ROOT::RIO ROOT::Net)

# Create the main program using the library.
add_executable(Main postprocess_digitizer.cpp)
#target_link_libraries(Main PUBLIC ROOT::Physics ROOT::RIO ROOT::Tree ROOT::Core ROOT::Gpad ROOT::Hist ROOT::Graf) 
target_link_libraries(Main PUBLIC ROOT::RIO ROOT::Tree ROOT::Gpad)  
target_link_libraries(Main PRIVATE spdlog::spdlog)
