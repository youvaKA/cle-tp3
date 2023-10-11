cmake_minimum_required (VERSION 3.10.2)
project (SC_MINILIB)
add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
add_link_options(-fsanitize=address)

option(WITH_TESTS "Testing subsystem (based on catch2)" OFF)

add_subdirectory (srcs)
add_subdirectory (examples)

if (WITH_TESTS)
  message(STATUS "WITH_TESTS")
  add_subdirectory (test)
endif()
