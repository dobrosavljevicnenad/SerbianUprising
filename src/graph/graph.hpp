#pragma once

#include "../gui/maplayer.h"
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

  bool insert_vertex(QPointF position, const std::string &label,
                     MapLayer *map_layer);
  bool insert_edge(const Vertex &from, const Vertex &to, double weight = 1.0);
  bool remove_vertex(const Vertex &vertex);
  bool remove_edge(const Vertex &from, const Vertex &to);
  std::vector<Vertex> neighbors(const Vertex &vertex) const;
  MapLayer *get_layer_for_vertex(const Vertex &vertex) const;
  void print_graph() const;

public:
  std::unordered_map<nodeID_t, Vertex> m_nodes;
  std::unordered_map<int, MapLayer *> vertex_to_layer;

private:
  adj_list_t m_adj_list;
  unsigned m_next_id;
};

} // namespace graph
