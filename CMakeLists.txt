cmake_minimum_required(VERSION 3.12)
project(k_cliques_k_partite_graphs)

set(CMAKE_CXX_STANDARD 17)

add_executable(k_cliques_k_partite_graphs main.cpp partite_binary_graph.h partite_binary_graph.cpp io_utils.h io_utils.cpp)