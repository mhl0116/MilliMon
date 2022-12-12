add_library(milliqan_library lib.cpp DigitizerOutput.cpp mappings.cpp Pulse.cpp)

# We need this directory, and users of our library will need it too
target_include_directories(milliqan_library PUBLIC ../include)

target_link_libraries(milliqan_library PUBLIC ROOT::RIO ROOT::Tree ROOT::Gpad)  
target_link_libraries(milliqan_library PUBLIC spdlog::spdlog)

# All users of this library will need at least C++11
# target_compile_features(milliqan_library PUBLIC cxx_std_11)

