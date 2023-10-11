cmake_minimum_required(VERSION 3.11)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1)

FetchContent_MakeAvailable(Catch2)

add_executable(test_sc test_sc.cpp)
target_compile_features(test_sc PRIVATE cxx_std_17)  
if (WIN32)
  target_compile_options(test_sc PRIVATE /WX /W4)
else()
  target_compile_options(test_sc PRIVATE -Wall)
endif()
target_include_directories(test_sc PUBLIC ${PROJECT_SOURCE_DIR}/srcs)
target_link_libraries(test_sc PUBLIC Catch2::Catch2WithMain sc_minilib)
add_test (NAME test_sc COMMAND test_sc)
