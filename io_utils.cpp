//
// Created by Roman Pogodin, 2018.
//

#include "io_utils.h"
#include <string>

CommandLineArguments ParseCommandLine(int32_t argc, char **argv) {
  CommandLineArguments parsed_arguments;

  for (int32_t i = 1; i < argc; ++i) {
    if (std::string("--max_cliques") == argv[i] && (i + 1 < argc)) {
      parsed_arguments.n_solution_to_stop = std::stoi(argv[i + 1]);
    }

    if (std::string("--graph_filename") == argv[i] && (i + 1 < argc)) {
      parsed_arguments.graph_filename = argv[i + 1];
    }
  }

  return parsed_arguments;
}

void PrintMaxCliqueSolution(const PartiteGraphVisitor &visitor, std::ostream &out_stream) {
  out_stream << visitor.GetNumberOfSolutions() << std::endl;

  auto solutions = visitor.GetSolutions();

  if (!solutions.empty()) {
    for (const auto &clique : solutions) {
      for (const auto &vertex : clique) {
        out_stream << vertex << ' ';
      }
      out_stream << std::endl;
    }
  }
}

PartiteBinaryGraph ReadGraph(std::istream &in_stream) {
  uint32_t n_vertices;
  in_stream >> n_vertices;

  std::vector<std::vector<bool>> adjacency_matrix(n_vertices);
  std::vector<int32_t> vertex_partitions(n_vertices);

  for (int32_t i = 0; i < n_vertices; ++i) {
    in_stream >> vertex_partitions[i];
  }

  int32_t edge;

  for (int32_t i = 0; i < n_vertices; ++i) {
    adjacency_matrix[i].resize(n_vertices);

    for (int32_t j = 0; j < n_vertices; ++j) {
      in_stream >> edge;
      adjacency_matrix[i][j] = edge > 0;
    }
  }

  return PartiteBinaryGraph(adjacency_matrix, vertex_partitions);
}
