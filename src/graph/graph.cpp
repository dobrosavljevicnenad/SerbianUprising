#include "graph.hpp"
#include "edge.hpp"
#include "vertex.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace graph {

Graph::Graph() = default;
Graph::~Graph() = default;

Vertex Graph::insert_vertex(QPointF position, const std::string &label,
                            MapLayer *map_layer, Territory territory, Army army,
                            Player player) {
  Vertex vertex(position, label, map_layer, territory, army, player);

  vertices.emplace(vertex.id(), vertex);
  m_adj_list[vertex] = std::vector<Edge>();

  return vertex;
}

bool Graph::insert_edge(const Vertex &from, const Vertex &to, double weight) {
  if (vertices.find(from.id()) == vertices.end() ||
      vertices.find(to.id()) == vertices.end()) {
    std::cerr << "Node doesn't exist";
    return false;
  }

  Edge edge(from.id(), to.id(), weight);
  m_adj_list[from].push_back(edge);
  m_adj_list[to].push_back(edge);

  return true;
}

bool Graph::remove_vertex(const Vertex &vertex) {

  auto it = vertices.find(vertex.id());
  if (it == vertices.end()) {
    std::cerr << "Vertex doesn't exist\n";
    return false;
  }

  vertices.erase(it);
  m_adj_list.erase(vertex);
  for (auto &[node, edges] : m_adj_list) {
    edges.erase(std::remove_if(edges.begin(), edges.end(),
                               [&vertex](const Edge &edge) {
                                 return edge.from() == vertex.id() ||
                                        edge.to() == vertex.id();
                               }),
                edges.end());
  }

  return true;
}

bool Graph::remove_edge(const Vertex &from, const Vertex &to) {

  auto remove_edge_from_list = [&from, &to](std::vector<Edge> &edges) {
    edges.erase(std::remove_if(edges.begin(), edges.end(),
                               [&from, &to](const Edge &edge) {
                                 return (edge.from() == from.id() &&
                                         edge.to() == to.id()) ||
                                        (edge.from() == to.id() &&
                                         edge.to() == from.id());
                               }),
                edges.end());
  };

  remove_edge_from_list(m_adj_list[from]);
  remove_edge_from_list(m_adj_list[to]);

  return true;
}

std::vector<Vertex> Graph::neighbors(const Vertex &vertex) const {
  std::vector<Vertex> neighbors;

  auto it = m_adj_list.find(vertex);
  if (it != m_adj_list.end()) {
    for (const Edge &edge : it->second) {
      int neighbor_id = (edge.from() == vertex.id()) ? edge.to() : edge.from();
      neighbors.push_back(vertices.at(neighbor_id));
    }
  }

  return neighbors;
}



// void Graph::print_graph() const {
//   std::cout << "Nodes:\n";
//   for (const auto &[id, vertex] : vertices) {
//     auto position = vertex.position();
//     std::cout << "ID: " << vertex.id() << ", Label: " << vertex.label()
//               << ", Position: (" << position.x() << ", " << position.y()
//               << ")\n";
//   }

//   std::cout << "\nGrane u grafu:\n";
//   for (const auto &[id, edges] : m_adj_list) {
//     for (const auto &edge : edges) {
//       std::cout << "From: " << edge.from() << " ->  To: " << edge.to() << '\n';
//     }
//   }
// }

void Graph::print_graph() const{
    for(const auto &[id, vertex] : vertices){
        std::cout << "[Vertex ID]: " << vertex.id() << "\n[Label]: " << vertex.label()
                  << "\n[Army type]: " << vertex.army.to_string(vertex.army.armyType()) << "\n[Soldiers]: " << vertex.army.getSoldiers()
                  << "\n[Terrain]: " << vertex.territory.to_string(vertex.territory.getTerrain())
                  << "\n----------------------" << std::endl;;
    }
}
}
