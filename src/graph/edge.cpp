#include "edge.hpp"

namespace graph {

Edge::Edge(unsigned from, unsigned to, double weight)
    : m_from(from), m_to(to), m_weight(weight) {}

unsigned Edge::from() const { return m_from; }
unsigned Edge::to() const { return m_to; }
unsigned Edge::weight() const { return m_weight; }

} // namespace graph
