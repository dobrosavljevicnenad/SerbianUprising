#include "vertex.hpp"

namespace graph {

unsigned graph::Vertex::m_index = 1;

Vertex::Vertex(QPointF position, const std::string &label,
               MapLayer *map_layer, Terrain territory, Army army,
               Player player)
    : m_position(position), m_label(label), m_id(m_index++), territory(territory), army(army), player(player) {}

Vertex::~Vertex() = default;

QPointF Vertex::position() const { return m_position; }
std::string Vertex::label() const { return m_label; }
unsigned Vertex::id() const { return m_id; }

} // namespace graph
