#include "edge.hpp"

namespace graph {

Edge::Edge(Vertex from, Vertex to, double weight, EdgeType type)
    : m_from(from.id()), m_to(to.id()), m_weight(weight), m_type(type){}

unsigned Edge::from() const { return m_from; }
unsigned Edge::to() const { return m_to; }
double Edge::weight() const { return m_weight; }

std::string Edge::to_string() const {
    switch (m_type) {
    case EdgeType::Land:
        return "Land";
    case EdgeType::River:
        return "River";
    default:
        return "";
    }
}

EdgeType Edge::type() const
{
    return m_type;
}


} // namespace graph
