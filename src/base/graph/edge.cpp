#include "edge.hpp"

namespace graph {

Edge::Edge(Vertex from, Vertex to, double weight, EdgeType type)
    : m_from(from.id()), m_to(to.id()), m_weight(weight), m_type(type){}

unsigned Edge::from() const { return m_from; }
unsigned Edge::to() const { return m_to; }
double Edge::weight() const { return m_weight; }

std::string Edge::to_string(){
    switch(m_type){
    case EdgeType::Land:
        return "Land";
        break;
    case EdgeType::River:
        return "River";
        break;
    default:
        return "";
    }
}

} // namespace graph
