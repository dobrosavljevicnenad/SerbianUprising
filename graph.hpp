#pragma once

#include "edge.hpp"
#include "vertex.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace graph {

using adj_list_t = std::unordered_map<Vertex, std::vector<Edge>>;

class Graph {
public:
  Graph();

  ~Graph();

  bool insert_vertex(position_t position, const std::string &label);
  bool insert_edge(const Vertex &from, const Vertex &to, double weight = 1.0);
  bool remove_vertex(const Vertex &vertex);
  bool remove_edge(const Vertex &from, const Vertex &to);

  void print_graph() const;

public:
  std::unordered_map<nodeID_t, Vertex> m_nodes;

private:
  adj_list_t m_adj_list;
  unsigned m_next_id;
};

} // namespace graph
