#include "graph.hpp"

namespace graph {

Graph::Graph() = default;
Graph::~Graph(){
    for(auto& pair : vertices)
        delete pair.second;
}

Vertex* Graph::insert_vertex(QPointF position, const std::string &label,
                             MapLayer *map_layer, Terrain territory, Army army,
                             Player player) {
    Vertex* vertex = new Vertex(m_next_id++,position, label, map_layer, territory, army, player);

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
        << "\n----------------------" << std::endl;
    }
}

QJsonObject Graph::serialize() const {
    QJsonObject graphJson;

    // Serialize vertices
    QJsonArray verticesArray;
    for (const auto& [id, vertex] : vertices) {
        QJsonObject vertexJson;
        vertexJson["id"] = static_cast<int>(vertex->id()); // Store the vertex ID
        vertexJson["label"] = QString::fromStdString(vertex->label()); // Store the label
        vertexJson["num_of_soldiers"] = vertex->army.getSoldiers(); // Store the number of soldiers
        vertexJson["army_type"] = QString::fromStdString(vertex->army.to_string(vertex->army.armyType())); // Store the army type
        vertexJson["terrain_type"] = QString::fromStdString(vertex->terrain.to_string(vertex->terrain.getTerrain())); // Store the terrain type
        // Add the vertex to the vertices array
        verticesArray.push_front(vertexJson);
    }
    graphJson["vertices"] = verticesArray;

    // Serialize edges
    QJsonArray edgesArray;
    for (const auto& [vertex, edges] : m_adj_list) {
        for (const Edge& edge : edges) {
            QJsonObject edgeJson;
            edgeJson["from"] = static_cast<int>(edge.from()); // Store the source vertex ID
            edgeJson["to"] = static_cast<int>(edge.to()); // Store the destination vertex ID
            edgeJson["weight"] = edge.weight(); // Store the weight of the edge
            edgeJson["type"] = QString::fromStdString(edge.to_string()); // Store the type of the edge

            // Add the edge to the edges array
            edgesArray.append(edgeJson);
        }
    }
    graphJson["edges"] = edgesArray;

    return graphJson;
}

void Graph::deserialize(const QJsonObject &json) {
    QJsonArray verticesArray = json["vertices"].toArray();
    if(!initialized){// Deserialize vertices
        for (const QJsonValue& value : verticesArray) {
            QJsonObject vertexJson = value.toObject();
            std::string label = vertexJson["label"].toString().toStdString(); // Read the label
            int armyCount = vertexJson["num_of_soldiers"].toInt(); // Read the number of soldiers
            std::string armyType = vertexJson["army_type"].toString().toStdString(); // Read the army type
            int playerId=0;
            std::string terrainType = vertexJson["terrain_type"].toString().toStdString();
            if (armyType == "HAJDUK") {
                playerId = 1;
            } else if (armyType == "JANISSARY") {
                playerId = 2;
            }
            insert_vertex(QPointF(0, 0),
                          label,
                          nullptr,
                          Terrain::fromString(terrainType),
                          Army::fromString(armyType, armyCount),
                          Player::fromJson(playerId, armyType));
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
            unsigned id = vertexJson["id"].toInt(); // Read the vertex ID
            int armyCount = vertexJson["num_of_soldiers"].toInt();
            std::string armyType = vertexJson["army_type"].toString().toStdString();
            int playerId = (armyType == "HAJDUK") ? 1 : (armyType == "JANISSARY" ? 2 : 0);

            vertices[id]->player = Player::fromJson(playerId, armyType);
            vertices[id]->army = Army::fromString(armyType, armyCount);
        }
    }
}

}


