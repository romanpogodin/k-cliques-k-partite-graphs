//
// Created by Roman Pogodin, 2018.
//

#include "partite_binary_graph.h"

PartiteBinaryGraph::PartiteBinaryGraph(std::vector<std::vector<bool>> adjacency_matrix,
                                       std::vector<int32_t> vertex_partitions)
    : adjacency_matrix_(std::move(adjacency_matrix)),
      vertex_partitions_(std::move(vertex_partitions)) {
  n_vertices_ = static_cast<int32_t>(vertex_partitions_.size());
  n_partitions_ = 1 + *std::max_element(vertex_partitions_.begin(), vertex_partitions_.end());

  for (int32_t vertex_one = 0; vertex_one < n_vertices_; ++vertex_one) {
    for (int32_t vertex_two = vertex_one + 1; vertex_two < n_vertices_; ++vertex_two) {
      if (vertex_partitions_[vertex_one] == vertex_partitions_[vertex_two] &&
          AreConnected(vertex_one, vertex_two)) {
        throw std::invalid_argument(
            "PartiteBinaryGraph: edge between vertices in the same partition");
      }
    }
  }

  partition_sizes_.resize(static_cast<uint32_t >(n_partitions_));

  for (int32_t partition : vertex_partitions_) {
    ++partition_sizes_[partition];
  }
  max_partition_size_ = *std::max_element(partition_sizes_.begin(), partition_sizes_.end());
}

void PartiteGraphVisitor::ReceiveSolution(const std::vector<int32_t> &solution) {
  ++n_solutions_;
  if (save_solutions_) {
    solutions_.push_back(solution);
  }
}

bool PartiteGraphVisitor::StopSearching() const {
  return ((n_solution_to_stop_ >= 0) && (n_solutions_ >= n_solution_to_stop_));
}

void FindMaximumCliques(const PartiteBinaryGraph &graph, PartiteGraphVisitor *visitor) {
  if (visitor->StopSearching()) {
    return;
  }

  auto n_partitions = static_cast<uint32_t>(graph.GetNumberOfPartitions());
  auto n_vertices = static_cast<uint32_t>(graph.GetNumberOfVertices());
  std::vector<int32_t> partition_sizes = graph.GetPartitionSizes();

  std::vector<int32_t> solution(n_partitions);

  std::vector<std::vector<int32_t>> compatible_solutions(n_partitions);
  std::vector<std::vector<int32_t>> stable_solutions(n_partitions);
  std::vector<std::vector<int32_t>> erased_solutions(n_partitions);

  for (int32_t partition = 0; partition < n_partitions; ++partition) {
    compatible_solutions[partition].reserve(static_cast<uint32_t>(partition_sizes[partition]));
    stable_solutions[partition].reserve(static_cast<uint32_t>(partition_sizes[partition]));
    erased_solutions[partition].reserve(n_vertices);
  }

  for (int32_t vertex = 0; vertex < n_vertices; ++vertex) {
    compatible_solutions[graph.GetVertexPartition(vertex)].push_back(vertex);
  }

  int32_t min_size_partition = static_cast<int32_t>(
      std::min_element(partition_sizes.begin(), partition_sizes.end()) - partition_sizes.begin());

  std::vector<int32_t> saved_vertices;
  saved_vertices.reserve(static_cast<uint32_t>(graph.GetMaxPartitionSize()));
  FindSingleMaximumClique(0, min_size_partition, graph, visitor,
      &solution, &compatible_solutions, &erased_solutions, &saved_vertices, &stable_solutions);
}

void EraseIncompatibleVertices(const PartiteBinaryGraph &graph,
                               std::vector<std::vector<int32_t>> *erased_solutions,
                               std::vector<int32_t> *saved_vertices,
                               std::vector<int32_t> *possible_partition,
                               int32_t level, int32_t vertex, int32_t *new_empty_partitions) {
  saved_vertices->clear();
  int32_t n_erased_vertices = 0;  // check if the partition is empty

  for (const auto &new_vertex : *possible_partition) {
    if (!graph.AreConnected(vertex, new_vertex)) {
      (*erased_solutions)[level].push_back(new_vertex);
      ++n_erased_vertices;

      if (n_erased_vertices == possible_partition->size()) {
        ++(*new_empty_partitions);
      }
    } else {
      saved_vertices->push_back(new_vertex);
    }
  }

  // assign non-erased vertices
  *possible_partition = *saved_vertices;
}

int32_t FindNextPartition(const PartiteBinaryGraph &graph,
    std::vector<std::vector<int32_t>> *compatible_solutions) {
  int32_t next_partition_size = graph.GetNumberOfVertices() + 1;
  int32_t next_partition = -1;

  for (int32_t possible_partition = 0;
       possible_partition < compatible_solutions->size();
       ++possible_partition) {
    if (!(*compatible_solutions)[possible_partition].empty() &&
        next_partition_size > (*compatible_solutions)[possible_partition].size()) {
      next_partition_size = static_cast<int32_t>(
          (*compatible_solutions)[possible_partition].size());
      next_partition = possible_partition;
    }
  }

  if (next_partition == -1) {
    throw std::logic_error("FindSingleMaximumClique: branching: Next partition is undefined");
  }

  return next_partition;
}

void RestoreErasedVertices(const PartiteBinaryGraph &graph,
                           std::vector<std::vector<int32_t>> *compatible_solutions,
                           std::vector<std::vector<int32_t>> *erased_solutions,
                           int32_t level) {
  for (const auto &erased_vertex : (*erased_solutions)[level]) {
    (*compatible_solutions)[graph.GetVertexPartition(erased_vertex)].push_back(erased_vertex);
  }

  (*erased_solutions)[level].clear();
}

void FindSingleMaximumClique(int32_t level, int32_t partition,
                             const PartiteBinaryGraph &graph, PartiteGraphVisitor *visitor,
                             std::vector<int32_t> *solution,
                             std::vector<std::vector<int32_t>> *compatible_solutions,
                             std::vector<std::vector<int32_t>> *erased_solutions,
                             std::vector<int32_t> *saved_vertices,
                             std::vector<std::vector<int32_t>> *stable_solutions) {
//  const auto stable_compatible_partition = (*compatible_solutions)[partition];
  (*stable_solutions)[partition] = (*compatible_solutions)[partition];

  for (const auto &vertex : (*stable_solutions)[partition]) {
    int32_t new_empty_partitions = 0;

    // forward loop
    for (auto &&possible_partition : *compatible_solutions) {
      if (new_empty_partitions <= 1) {
        EraseIncompatibleVertices(graph, erased_solutions, saved_vertices, &possible_partition,
            level, vertex, &new_empty_partitions);
      }
    }

    // branching
    if (new_empty_partitions <= 1) {
      (*solution)[partition] = vertex;

      if (level + 1 == solution->size()) {
        visitor->ReceiveSolution(*solution);

        if (visitor->StopSearching()) {
          return;
        }

      } else {
        int32_t next_partition = FindNextPartition(graph, compatible_solutions);

        FindSingleMaximumClique(level + 1, next_partition, graph, visitor, solution,
            compatible_solutions, erased_solutions, saved_vertices, stable_solutions);

        if (visitor->StopSearching()) {
          return;  // break the loop
        }
      }
      // (*solution)[partition] = -1;  // only for debug
    }

    // backward loop
    RestoreErasedVertices(graph, compatible_solutions, erased_solutions, level);
  }
}
