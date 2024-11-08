#include "vertex.hpp"

namespace graph {

unsigned graph::Vertex::m_index = 1;

Vertex::Vertex(QPointF position, std::string label)
    : m_position(position), m_label(label), m_id(m_index++) {}

Vertex::~Vertex() = default;

QPointF Vertex::position() const { return m_position; }
std::string Vertex::label() const { return m_label; }
unsigned Vertex::id() const { return m_id; }

} // namespace graph
