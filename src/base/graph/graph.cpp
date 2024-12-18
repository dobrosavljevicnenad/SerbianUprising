#include "graph.hpp"

namespace graph {

Graph::Graph() = default;
Graph::~Graph(){
    for(auto& pair : vertices)
        delete pair.second;
}

Vertex* Graph::insert_vertex(QPointF position, const std::string &label,
                             MapLayer *map_layer, Terrain territory, Army army,
                             Player player, CultureType culture, City* city, Region* region) {
    Vertex* vertex = new Vertex(m_next_id++,position, label, map_layer, territory, army, player, culture, city, region);


    vertices.emplace(vertex->id(), vertex);
    m_adj_list[vertex] = std::vector<Edge>();
    return vertex;
}
Vertex* Graph::get_vertex_by_id(nodeID_t id) const {
    auto it = vertices.find(id);

    if (it != vertices.end()) {
        return it->second;
    }
    return nullptr;
}

bool Graph::insert_edge(Vertex* from,Vertex* to, double weight, EdgeType type) {
    if (vertices.find(from->id()) == vertices.end() ||
        vertices.find(to->id()) == vertices.end()) {
        std::cerr << "Node doesn't exist";
        return false;
    }

    weight = (type == EdgeType::River ? (weight - 10) : weight);
    Edge edge(*from, *to, weight, type);
    m_adj_list[from].push_back(edge);
    m_adj_list[to].push_back(edge);

    return true;
}

bool Graph::remove_vertex(Vertex* vertex) {
    auto it = vertices.find(vertex->id());
    if (it == vertices.end()) {
        std::cerr << "Vertex doesn't exist\n";
        return false;
    }

    m_adj_list.erase(vertex);
    for (auto& [node, edges] : m_adj_list) {
        edges.erase(std::remove_if(edges.begin(), edges.end(),
                                   [vertex](const Edge &edge) {
                                       return edge.from() == vertex->id() ||
                                              edge.to() == vertex->id();
                                   }),
                    edges.end());
    }

    delete vertex;
    vertices.erase(it);

    return true;
}

bool Graph::remove_edge(Vertex* from, Vertex* to) {
    auto remove_edge_from_list = [&from, &to](std::vector<Edge>& edges) {
        edges.erase(std::remove_if(edges.begin(), edges.end(),
                                   [from, to](const Edge &edge) {
                                       return (edge.from() == from->id() &&
                                               edge.to() == to->id()) ||
                                              (edge.from() == to->id() &&
                                               edge.to() == from->id());
                                   }),
                    edges.end());
    };
    remove_edge_from_list(m_adj_list[from]);
    remove_edge_from_list(m_adj_list[to]);

    return true;
}

std::vector<Vertex*> Graph::neighbors(const Vertex* vertex) const {
    std::vector<Vertex*> neighbors;

    auto it = m_adj_list.find(const_cast<Vertex*>(vertex));
    if (it != m_adj_list.end()) {
        for (const Edge &edge : it->second) {
            int neighbor_id = (edge.from() == vertex->id()) ? edge.to() : edge.from();
            neighbors.push_back(vertices.at(neighbor_id));
        }
    }

    return neighbors;
}

bool Graph::is_neighbor(const Vertex* vertex1, const Vertex* vertex2) const {
    auto it = m_adj_list.find(const_cast<Vertex*>(vertex1));
    for (const Edge &edge : it->second) {
        if (edge.to() == vertex2->id() || edge.from() == vertex2->id())
            return true;
    }
    return false;
}

std::unordered_map<Vertex*, std::vector<Edge>> Graph::adj_list() const {
    return m_adj_list;
}

void Graph::print_graph() const {
    for (const auto& [id, vertex] : vertices) {
        std::cout << "[Vertex ID]: " << vertex->id() << "\n[Label]: " << vertex->label()
        << "\n[Army type]: " << vertex->army.to_string(vertex->army.armyType())
        << "\n[Soldiers]: " << vertex->army.getSoldiers()
        << "\n[Terrain]: " << vertex->terrain.to_string(vertex->terrain.getTerrain())
        << "\n[Player]: " << vertex->player.getPlayerId()
        << "\n[city]: " << vertex->city->getLevel()
        //<< "\n[region]: " << vertex->region->getRegionName()
        << "\n[culture]: " << vertex->culture
        << "\n----------------------" << std::endl;
    }
}

graph::Vertex* Graph::get_vertex_by_label(const QString& label) const {
    for (const auto& vertexPair : vertices) {
        if (vertexPair.second->label() == label.toStdString()){
            return vertexPair.second;
        }
    }
    return nullptr; // Vraća nullptr ako ne pronađe
}

QJsonObject Graph::serialize() const {
    QJsonObject graphJson;

    // Serialize vertices
    QJsonArray verticesArray;
    for (const auto& [id, vertex] : vertices) {
        QJsonObject vertexJson;
        vertexJson["id"] = static_cast<int>(vertex->id());
        vertexJson["label"] = QString::fromStdString(vertex->label());
        vertexJson["num_of_soldiers"] = vertex->army.getSoldiers();
        vertexJson["army_type"] = QString::fromStdString(vertex->army.to_string(vertex->army.armyType()));
        vertexJson["terrain_type"] = QString::fromStdString(vertex->terrain.to_string(vertex->terrain.getTerrain()));

        // Include cityLevel
        vertexJson["cityLevel"] = vertex->city ? vertex->city->getLevel() : 0;

        // Include region info
        if (vertex->region) {
            vertexJson["regionName"] = QString::fromStdString(vertex->region->getRegionName());
            vertexJson["regionId"] = QString::fromStdString(vertex->region->getRegionId());
        } else {
            vertexJson["regionName"] = "";
            vertexJson["regionId"] = "";
        }

        verticesArray.push_front(vertexJson);
    }
    graphJson["vertices"] = verticesArray;

    // Serialize edges
    QJsonArray edgesArray;
    for (const auto& [vertex, edges] : m_adj_list) {
        for (const Edge& edge : edges) {
            QJsonObject edgeJson;
            edgeJson["from"] = static_cast<int>(edge.from());
            edgeJson["to"] = static_cast<int>(edge.to());
            edgeJson["weight"] = edge.weight();
            edgeJson["type"] = QString::fromStdString(edge.to_string());
            edgesArray.push_back(edgeJson);
        }
    }
    graphJson["edges"] = edgesArray;

    return graphJson;
}


void Graph::deserialize(const QJsonObject &json) {
    QJsonArray verticesArray = json["vertices"].toArray();
    if(!initialized){// Deserialize vertices
        for (const QJsonValue &value : verticesArray) {
            QJsonObject vertexJson = value.toObject();

            // Basic Data
            std::string label = vertexJson["label"].toString().toStdString();
            int armyCount = vertexJson["num_of_soldiers"].toInt();
            std::string armyType = vertexJson["army_type"].toString().toStdString();
            std::string terrainType = vertexJson["terrain_type"].toString().toStdString();

            int playerId = (armyType == "HAJDUK") ? 1 : 2;
            CultureType culture = (armyType == "HAJDUK") ? CultureType::SERBIAN : CultureType::TURKISH;

            // City Level and City Initialization
            int cityLevel = vertexJson["cityLevel"].toInt();
            City *city = CityFactory::getCityByLevel(cityLevel);


            // Insert Vertex
            insert_vertex(QPointF(0,0),
                          label,
                          nullptr,
                          Terrain::fromString(terrainType),
                          Army::fromString(armyType, armyCount),
                          Player::fromJson(playerId, armyType),
                          culture,
                          city,
                          nullptr); // Region initialized later
        }

        // Deserialize edges
        QJsonArray edgesArray = json["edges"].toArray();
        for (const QJsonValue& value : edgesArray) {
            QJsonObject edgeJson = value.toObject();

            unsigned fromId = edgeJson["from"].toInt();
            unsigned toId = edgeJson["to"].toInt();
            double weight = edgeJson["weight"].toDouble();
            std::string type = edgeJson["type"].toString().toStdString();

            Vertex* fromVertex = vertices[fromId];
            Vertex* toVertex = vertices[toId];

            if (fromVertex && toVertex) {
                EdgeType edgeType = Edge::fromString(type);
                insert_edge(fromVertex, toVertex, weight, edgeType);
            }
        }
        initialized=true;
    } else {
        for (const QJsonValue& value : verticesArray) {
            QJsonObject vertexJson = value.toObject();
            std::string label = vertexJson["label"].toString().toStdString(); // Extract label
            int armyCount = vertexJson["num_of_soldiers"].toInt();
            std::string armyType = vertexJson["army_type"].toString().toStdString();
            int playerId = (armyType == "HAJDUK") ? 1 : (armyType == "JANISSARY" ? 2 : 0);

            // Find the vertex ID using the label
            for (auto& [id, vertex] : vertices) {
                if (vertex->label() == label) {
                    vertex->player = Player::fromJson(playerId, armyType);
                    vertex->army = Army::fromString(armyType, armyCount);
                    break; // Stop searching once the vertex is found
                }
            }
        }
    }
}

}


