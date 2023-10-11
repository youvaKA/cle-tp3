cmake_minimum_required (VERSION 3.10.2)
project (Air_Conditioner)
add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
add_link_options(-fsanitize=address)

add_subdirectory(sc_minilib/srcs)

add_executable(ac aircond.cpp)

target_include_directories (ac PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sc_minilib/srcs)
target_link_libraries(ac LINK_PUBLIC sc_minilib)
