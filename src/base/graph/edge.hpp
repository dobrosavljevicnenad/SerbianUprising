#pragma once
#include <string>
#include "vertex.hpp"

namespace graph {

enum class EdgeType {
    River,
    Land
};

class Edge {
public:
  Edge(Vertex from, Vertex to, double weight = 0.0, EdgeType type = EdgeType::Land);

  unsigned from() const;
  unsigned to() const;
  double weight() const;

  std::string to_string() const;

  static EdgeType fromString(const std::string& edgeType) {
      if (edgeType == "Land") {
          return EdgeType::Land;
      }
      return EdgeType::River;
  }

private:
  unsigned m_from;
  unsigned m_to;
  double m_weight = 0.0;
  EdgeType m_type;
};
} // namespace graph
