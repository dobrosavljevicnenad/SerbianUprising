#pragma once

#include <string>
#include<QPointF>
#include "../gui/maplayer.h"
#include "../gui/maplayer.h"
#include "../base/Army.h"
#include "../base/Battle.h"
#include "../base/Strength.h"
#include "../base/mergeArmies.h"
#include "../base/player.h"
#include "../base/Terrain.h"

namespace graph {

//using position_t = std::pair<unsigned, unsigned>;
using nodeID_t = unsigned;

class Vertex {
public:
  Vertex(QPointF position, const std::string &label,
           MapLayer *map_layer, Terrain territory, Army army,
           Player player);

  ~Vertex();

  unsigned id() const;
  QPointF position() const;
  std::string label() const;

  bool operator==(const Vertex &other) const { return id() == other.id(); }

public:
  Terrain terrain;
  Army army;
  Player player;

private:
  static unsigned m_index;
  unsigned m_id;

  QPointF m_position;
  std::string m_label;
  MapLayer* map_layer;
};

} // namespace graph

namespace std {
template <> struct hash<graph::Vertex> {
  std::size_t operator()(const graph::Vertex &vertex) const {
    return std::hash<unsigned>{}(vertex.id());
  }
};
} // namespace std
