#pragma once

#include "edge.hpp"
#include "vertex.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <QJsonArray>
#include <QJsonObject>
#include "../../Interface/filemanager.h"

namespace graph {

class Graph {
public:
    explicit Graph();
    ~Graph();

    Vertex* insert_vertex(QPointF position, const std::string& label,
                          MapLayer* map_layer, Terrain territory, Army army,
                          Player player, CultureType culture, City* city, Region* region);

    bool insert_edge(Vertex* from, Vertex* to, double weight, EdgeType type = EdgeType::Land);
    bool remove_vertex(Vertex* vertex);
    bool remove_edge(Vertex* from, Vertex* to);
    std::vector<Vertex*> neighbors(const Vertex* vertex) const;
    bool is_neighbor(const Vertex* vertex1, const Vertex* vertex2) const;
    std::unordered_map<Vertex*, std::vector<Edge>> adj_list() const;
    MapLayer* get_layer_for_vertex(const Vertex* vertex) const;
    void print_graph() const;
    Vertex* get_vertex_by_id(nodeID_t id) const;
    QJsonObject serialize(QJsonObject graphJson);
    void deserialize(const QJsonObject &json);
    void save_to_json(const std::string &file_path) const;
    void save_to_json(const std::string &file_path, const QString &current_year) const;
    void clear();
    graph::Vertex* get_vertex_by_label(const QString& label) const;

public:
    std::unordered_map<nodeID_t, Vertex*> vertices;
    std::vector<Edge *> getEdges(const Vertex *vertex) const;
private:
    std::unordered_map<Vertex*, std::vector<Edge>> m_adj_list;
    unsigned m_next_id = 1;
    bool initialized = false;
    bool initializedSer = false;
};

} // namespace graph
