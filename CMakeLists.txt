cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(test VERSION 0.1.0 LANGUAGES CXX C)

# RingBuffer lib
add_library(ringbuffer STATIC ringbuffer.cc ringbuffer.h)
target_include_directories(ringbuffer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(ringbuffer PUBLIC cxx_std_17)

# test
add_executable(test main.cc)
target_link_libraries(test PRIVATE ringbuffer)
#add_dependencies(test PRIVATE consume)
#add_dependencies(test PRIVATE produce)

# produce
add_executable(produce produce.cc)
target_link_libraries(produce PRIVATE ringbuffer)

# consume
add_executable(consume consume.cc)
target_link_libraries(consume PRIVATE ringbuffer)