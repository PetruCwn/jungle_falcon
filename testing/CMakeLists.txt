cmake_minimum_required(VERSION 3.2)
project("testing")

find_package(GTest REQUIRED)
find_package(glog 0.3.5 REQUIRED)
find_package(gflags REQUIRED)

set(GTEST_INCLUDE_DIRS "/usr/local/googletest/googletest/include")

set(RISCV_SIM_DIR ../sim)
set(RISCV_ASM_DIR ../asm)
set(RISCV_C_DIR ../c)

set(CMAKE_CXX_FLAGS "--std=c++14 -g ${CMAKE_CXX_FLAGS}")
set(TESTING_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TESTING_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SIM_INCLUDE_DIR ${RISCV_SIM_DIR}/include)
set(SIM_SOURCE_DIR ${RISCV_SIM_DIR}/src)

include_directories(
  ${GTEST_INCLUDE_DIRS}
  ${TESTING_INCLUDE_DIR}
  ${PROJ_INCLUDE_DIR}
  ${SIM_INCLUDE_DIR})

set(TESTING_SOURCES
  ${SIM_SOURCE_DIR}/b_type_instructions.cpp
  ${SIM_SOURCE_DIR}/commands.cpp
  ${SIM_SOURCE_DIR}/command_interpreter.cpp
  ${SIM_SOURCE_DIR}/cpu.cpp
  ${SIM_SOURCE_DIR}/hazard_detection.cpp
  ${SIM_SOURCE_DIR}/instructions.cpp
  ${SIM_SOURCE_DIR}/instruction_factory.cpp
  ${SIM_SOURCE_DIR}/i_type_instructions.cpp
  ${SIM_SOURCE_DIR}/j_type_instructions.cpp
  ${SIM_SOURCE_DIR}/memory.cpp
  ${SIM_SOURCE_DIR}/pipeline.cpp
  ${SIM_SOURCE_DIR}/register_file.cpp
  ${SIM_SOURCE_DIR}/r_type_instructions.cpp
  ${SIM_SOURCE_DIR}/s_type_instructions.cpp
  ${SIM_SOURCE_DIR}/u_type_instructions.cpp)

set(TESTING_HEADERS
  ${SIM_INCLUDE_DIR}/b_type_instructions.hpp
  ${SIM_INCLUDE_DIR}/commands.hpp
  ${SIM_INCLUDE_DIR}/command_interpreter.hpp
  ${SIM_INCLUDE_DIR}/cpu.hpp
  ${SIM_INCLUDE_DIR}/hazard_detection.hpp
  ${SIM_INCLUDE_DIR}/hardware_object.hpp
  ${SIM_INCLUDE_DIR}/instructions.hpp
  ${SIM_INCLUDE_DIR}/instruction_factory.hpp
  ${SIM_INCLUDE_DIR}/i_type_instructions.hpp
  ${SIM_INCLUDE_DIR}/j_type_instructions.hpp
  ${SIM_INCLUDE_DIR}/memory.hpp
  ${SIM_INCLUDE_DIR}/pipeline.hpp
  ${SIM_INCLUDE_DIR}/register_file.hpp
  ${SIM_INCLUDE_DIR}/riscv_defs.hpp
  ${SIM_INCLUDE_DIR}/r_type_instructions.hpp
  ${SIM_INCLUDE_DIR}/s_type_instructions.hpp
  ${SIM_INCLUDE_DIR}/u_type_instructions.hpp)

add_executable(riscv_tests
  ${TESTING_SOURCE_DIR}/riscv_tests.cpp
  ${TESTING_HEADERS}
  ${TESTING_SOURCES})

target_link_libraries(riscv_tests 
  ${GTEST_LIBRARIES}
  ${GTEST_MAIN_LIBRARIES}
  glog::glog
  gflags
  pthread)

