add_library(sc_minilib
  sm_defs.cpp
  timeout.cpp
  ptime.cpp
  state_chart.cpp
  transition.cpp
  display.cpp)

target_include_directories (sc_minilib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sc_minilib PUBLIC pthread)
