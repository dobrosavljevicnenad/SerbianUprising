#include "vertex.hpp"

namespace graph {

unsigned graph::Vertex::m_index = 1;

Vertex::Vertex(position_t position, std::string label)
    : m_position(position), m_label(label), m_id(m_index++) {}

Vertex::~Vertex() = default;

position_t Vertex::position() const { return m_position; }
std::string Vertex::label() const { return m_label; }
unsigned Vertex::id() const { return m_id; }

} // namespace graph
