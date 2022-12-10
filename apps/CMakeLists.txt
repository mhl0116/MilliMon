add_executable(apps apps.cpp)
#target_compile_features(apps PRIVATE cxx_std_17)

target_link_libraries(apps PRIVATE milliqan_library)

