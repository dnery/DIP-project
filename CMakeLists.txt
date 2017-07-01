project(image_segmentation)
cmake_minimum_required(VERSION 2.6)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DEPS REQUIRED opencv igraph)

add_executable(image_segmentation image_segmentation.cc)
include_directories(${DEPS_INCLUDE_DIRS})
target_link_libraries(image_to_graph ${DEPS_LIBRARIES})
