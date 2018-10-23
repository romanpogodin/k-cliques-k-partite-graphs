//
// Created by Roman Pogodin, 2018.
//

#ifndef K_CLIQUES_K_PARTITE_GRAPHS_PARTITE_BINARY_GRAPH_H
#define K_CLIQUES_K_PARTITE_GRAPHS_PARTITE_BINARY_GRAPH_H

#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <stdint-gcc.h>
#include <stdexcept>

class PartiteBinaryGraph {
 public:
  explicit PartiteBinaryGraph(std::vector<std::vector<bool>> adjacency_matrix,
                              std::vector<int32_t> vertex_partitions);
  bool AreConnected(int32_t vertex_one, int32_t vertex_two) const {
    return adjacency_matrix_[vertex_one][vertex_two];
  }
  int32_t GetNumberOfVertices() const { return n_vertices_; }
  int32_t GetNumberOfPartitions() const { return n_partitions_; }
  int32_t GetVertexPartition(int32_t vertex) const { return vertex_partitions_[vertex]; }
  int32_t GetMaxPartitionSize() const { return max_partition_size_; }
  std::vector<int32_t> GetPartitionSizes() const { return partition_sizes_; }

 private:
  std::vector<std::vector<bool>> adjacency_matrix_;
  std::vector<int32_t> vertex_partitions_;
  std::vector<int32_t> partition_sizes_;
  int32_t n_vertices_;
  int32_t n_partitions_;
  int32_t max_partition_size_;
};

class PartiteGraphVisitor {
 public:
  explicit PartiteGraphVisitor(const PartiteBinaryGraph &graph, bool save_solutions = false,
                               int32_t n_solution_to_stop_ = -1)
      : graph_(graph), n_solutions_(0), save_solutions_(save_solutions),
        n_solution_to_stop_(n_solution_to_stop_) {}
  void ReceiveSolution(const std::vector<int32_t> &solution);
  bool StopSearching() const;
  int32_t GetNumberOfSolutions() const { return n_solutions_; }
  std::vector<std::vector<int32_t>> GetSolutions() const { return solutions_; }

 private:
  const PartiteBinaryGraph &graph_;
  int32_t n_solutions_;
  bool save_solutions_;
  std::vector<std::vector<int32_t>> solutions_;
  int32_t n_solution_to_stop_;
};

void EraseIncompatibleVertices(const PartiteBinaryGraph &graph,
                               std::vector<std::vector<int32_t>> *erased_solutions,
                               std::vector<int32_t> *saved_vertices,
                               std::vector<int32_t> *possible_partition,
                               int32_t level, int32_t vertex, int32_t *new_empty_partitions);

int32_t FindNextPartition(const PartiteBinaryGraph &graph,
                          std::vector<std::vector<int32_t>> *compatible_solutions);

void RestoreErasedVertices(const PartiteBinaryGraph &graph,
                           std::vector<std::vector<int32_t>> *compatible_solutions,
                           std::vector<std::vector<int32_t>> *erased_solutions,
                           int32_t level);

void FindSingleMaximumClique(int32_t level, int32_t partition,
                             const PartiteBinaryGraph &graph, PartiteGraphVisitor *visitor,
                             std::vector<int32_t> *solution,
                             std::vector<std::vector<int32_t>> *compatible_solutions,
                             std::vector<std::vector<int32_t>> *erased_solutions,
                             std::vector<int32_t> *saved_vertices);

void FindMaximumCliques(const PartiteBinaryGraph &graph, PartiteGraphVisitor *visitor);

#endif //K_CLIQUES_K_PARTITE_GRAPHS_PARTITE_BINARY_GRAPH_H
