cmake_minimum_required(VERSION 3.12)

project(waits-predictor)

add_library(waits_predictor STATIC
  src/counter.cpp
  src/states.cpp
  src/value.cpp
  src/waits_predictor.cpp)
target_compile_features(waits_predictor PUBLIC cxx_std_23)
target_compile_options(waits_predictor PUBLIC
  $<$<CONFIG:Debug>:-O0 -Wall -Wextra -g>
  $<$<CONFIG:Release>:-O3 -Wall -Wextra>)
target_include_directories(waits_predictor PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>)
target_compile_definitions(waits_predictor INTERFACE
  $<BUILD_INTERFACE:STATES_FILE_PATH="${PROJECT_BINARY_DIR}">
  $<INSTALL_INTERFACE:STATES_FILE_PATH="${CMAKE_INSTALL_PREFIX}/share/data">)
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
  DESTINATION share/data)
install(EXPORT WaitsPredictor FILE WaitsPredictorConfig.cmake DESTINATION share/cmake)

add_executable(mkind src/mkind.cpp src/utils.cpp)

add_executable(main src/main.cpp)
target_link_libraries(main waits_predictor)
