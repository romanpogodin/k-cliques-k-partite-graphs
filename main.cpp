//
// Created by Roman Pogodin, 2018.
//

#include "io_utils.h"
#include "partite_binary_graph.h"
#include <iostream>
#include <fstream>

int main(int32_t argc, char **argv) {
  CommandLineArguments command_line_arguments = ParseCommandLine(argc, argv);

  std::ifstream input_file(command_line_arguments.graph_filename, std::ios_base::in);
  if (!input_file.is_open()) {
    throw std::runtime_error("Failed to open " + command_line_arguments.graph_filename);
  }

  PartiteBinaryGraph graph = ReadGraph(input_file);
  input_file.close();

  PartiteGraphVisitor visitor(graph, command_line_arguments.save_solutions,
      command_line_arguments.n_solution_to_stop);
  FindMaximumCliques(graph, &visitor);
  PrintMaxCliqueSolution(visitor);

  return 0;
}
