cmake_minimum_required(VERSION 3.20.0)
project(thread)
set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/include)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/thread thre)
add_library(trt STATIC ${thre})


aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
link_directories(${PROJECT_SOURCE_DIR}/lib)
link_libraries(trt)
add_executable(
  execute
  ${SRC_LIST}
)

