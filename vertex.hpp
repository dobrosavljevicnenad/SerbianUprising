#pragma once

#include <string>
namespace graph {

using position_t = std::pair<unsigned, unsigned>;
using nodeID_t = unsigned;

class Vertex {
public:
  Vertex(position_t position, std::string label);

  ~Vertex();

  unsigned id() const;
  position_t position() const;
  std::string label() const;

  bool operator==(const Vertex &other) const { return id() == other.id(); }

private:
  static unsigned m_index;
  unsigned m_id;

  position_t m_position;
  std::string m_label;
};

} // namespace graph

namespace std {
template <> struct hash<graph::Vertex> {
  std::size_t operator()(const graph::Vertex &vertex) const {
    return std::hash<unsigned>{}(vertex.id());
  }
};
} // namespace std
