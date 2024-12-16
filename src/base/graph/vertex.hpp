#pragma once

#include <string>
#include<QPointF>
#include "../../Interface/maplayer.h"
#include "../Entities/Army.h"
#include "../Mechanics/Battle.h"
#include "../Mechanics/Strength.h"
#include "../Entities/mergeArmies.h"
#include "../Entities/player.h"
#include "../Entities/Terrain.h"
#include "../Entities/Culture.h"
namespace graph {

//using position_t = std::pair<unsigned, unsigned>;
using nodeID_t = unsigned;

class Vertex {
public:

    Vertex(unsigned id,QPointF position, const std::string &label,
           MapLayer *map_layer, Terrain terrain, Army army,
           Player player,CultureType culture);

    ~Vertex();
    void updateLayerColor();
    unsigned id() const;
    QPointF position() const;
    std::string label() const;

    bool operator==(const Vertex &other) const { return id() == other.id(); }

public:
    Terrain terrain;
    Army army;
    Player player;
    MapLayer* map_layer;
    CultureType culture;

private:
    unsigned m_id;

    QPointF m_position;
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
