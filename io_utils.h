//
// Created by Roman Pogodin, 2018.
//

#ifndef K_CLIQUES_K_PARTITE_GRAPHS_IO_UTILS_H
#define K_CLIQUES_K_PARTITE_GRAPHS_IO_UTILS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "partite_binary_graph.h"

struct CommandLineArguments {
  explicit CommandLineArguments(int32_t n_solutions_to_stop = -1,
      std::string graph_filename = "./input.txt")
      : n_solution_to_stop(n_solutions_to_stop), graph_filename(std::move(graph_filename)) {};

  int32_t n_solution_to_stop;
  std::string graph_filename;
};

CommandLineArguments ParseCommandLine(int32_t argc, char **argv);

PartiteBinaryGraph ReadGraph(std::istream &in_stream = std::cin);

void PrintMaxCliqueSolution(const PartiteGraphVisitor &visitor,
                            std::ostream &out_stream = std::cout);

#endif //K_CLIQUES_K_PARTITE_GRAPHS_IO_UTILS_H
