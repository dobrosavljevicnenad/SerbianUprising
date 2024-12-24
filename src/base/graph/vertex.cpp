#include "vertex.hpp"

namespace graph {

Vertex::Vertex(unsigned id,QPointF position, const std::string &label,
               MapLayer *map_layer, Terrain terrain, Army army,
               Player player, CultureType culture, City* city, Region* region)
    : m_id(id),m_position(position), m_label(label),map_layer(map_layer),
    terrain(terrain), army(army), player(player), culture(culture),city(city), region(region), m_bio(""), newRecruits(0) {}

Vertex::~Vertex() = default;

QPointF Vertex::position() const { return m_position; }
std::string Vertex::label() const { return m_label; }
std::string Vertex::bio() const { return m_bio; }
void Vertex::setBio(std::string bio) { m_bio = bio; }
unsigned Vertex::id() const { return m_id; }

} // namespace graph
