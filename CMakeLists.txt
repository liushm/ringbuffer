project(test)

#
add_library(ringbuffer STATIC ringbuffer.cc)
target_include_directories(ringbuffer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(ringbuffer PUBLIC cxx_std_17)

#
add_executable(test main.cc)
target_link_libraries(test PRIVATE ringbuffer)
add_dependencies(test PRIVATE consume)
add_dependencies(test PRIVATE produce)

#
add_executable(produce produce.cc)
target_link_libraries(produce PRIVATE ringbuffer)

#
add_executable(consume consume.cc)
target_link_libraries(consume PRIVATE ringbuffer)