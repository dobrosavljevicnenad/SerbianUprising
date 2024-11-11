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

bool Graph::insert_edge(const Vertex &from, const Vertex &to, double weight, EdgeType type) {
  if (vertices.find(from.id()) == vertices.end() ||
      vertices.find(to.id()) == vertices.end()) {
    std::cerr << "Node doesn't exist";
    return false;
  }

  weight = (type == EdgeType::River ? (weight - 10) : weight);
  Edge edge(from, to, weight, type);
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

bool Graph::is_neighbor(const Vertex &vertex1, const Vertex &vertex2) const {
    auto it = m_adj_list.find(vertex1);
    for (const Edge &edge : it->second){
        if (edge.to() == vertex2.id() || edge.from() == vertex2.id())
            return true;
    }
    return false;
}

std::unordered_map<Vertex, std::vector<Edge>> Graph::adj_list() const{
    return m_adj_list;
}

void Graph::print_graph() const{
    for(const auto &[id, vertex] : vertices){
        std::cout << "[Vertex ID]: " << vertex.id() << "\n[Label]: " << vertex.label()
                  << "\n[Army type]: " << vertex.army.to_string(vertex.army.armyType()) << "\n[Soldiers]: " << vertex.army.getSoldiers()
                  << "\n[Terrain]: " << vertex.territory.to_string(vertex.territory.getTerrain())
                  << "\n----------------------" << std::endl;;
    }
}
}
