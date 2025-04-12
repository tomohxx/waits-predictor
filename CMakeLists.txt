cmake_minimum_required(VERSION 3.5)

project(waits-predictor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -Wextra -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra")

add_library(waits_predictor STATIC
  src/counter.cpp
  src/states.cpp
  src/value.cpp
  src/waits_predictor.cpp)
set(PUBLIC_HEADERS
  src/states.hpp
  src/value.hpp
  src/waits_predictor.hpp)
set_target_properties(waits_predictor PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")
install(TARGETS waits_predictor EXPORT WaitsPredictor)
install(FILES
  ${PROJECT_BINARY_DIR}/lh_h_a.bin
  ${PROJECT_BINARY_DIR}/lh_h_b.bin
  ${PROJECT_BINARY_DIR}/lh_h_d.bin
  ${PROJECT_BINARY_DIR}/lh_s_a.bin
  ${PROJECT_BINARY_DIR}/lh_s_b.bin
  ${PROJECT_BINARY_DIR}/lh_s_c.bin
  ${PROJECT_BINARY_DIR}/lh_s_d.bin
  ${PROJECT_BINARY_DIR}/lh_sp_h_a.bin
  ${PROJECT_BINARY_DIR}/lh_sp_h_b.bin
  ${PROJECT_BINARY_DIR}/lh_sp_h_d.bin
  ${PROJECT_BINARY_DIR}/lh_sp_s_a.bin
  ${PROJECT_BINARY_DIR}/lh_sp_s_b.bin
  ${PROJECT_BINARY_DIR}/lh_sp_s_c.bin
  ${PROJECT_BINARY_DIR}/lh_sp_s_d.bin
  ${PROJECT_BINARY_DIR}/sp_h_a.bin
  ${PROJECT_BINARY_DIR}/sp_h_b.bin
  ${PROJECT_BINARY_DIR}/sp_s_a.bin
  ${PROJECT_BINARY_DIR}/sp_s_b.bin
TYPE DATA)
install(EXPORT WaitsPredictor FILE WaitsPredictorConfig.cmake DESTINATION share/cmake)

add_executable(mkind src/mkind.cpp src/utils.cpp)

add_executable(main src/main.cpp)
target_compile_definitions(main PUBLIC STATES_FILE_PATH="${PROJECT_BINARY_DIR}")
target_link_libraries(main waits_predictor)
