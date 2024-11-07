#pragma once

namespace graph {

class Edge {
public:
  Edge(unsigned from, unsigned to, double weight = 1);

  unsigned from() const;
  unsigned to() const;
  unsigned weight() const;

private:
  unsigned m_from;
  unsigned m_to;
  double m_weight;
};
} // namespace graph
