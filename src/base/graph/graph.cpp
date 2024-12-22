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

std::vector<Edge*> Graph::getEdges(const Vertex* vertex) const {
    std::vector<Edge*> edges;

    auto it = m_adj_list.find(const_cast<Vertex*>(vertex));
    if (it != m_adj_list.end()) {
        for (const Edge &edge : it->second) {
            edges.push_back(const_cast<Edge*>(&edge));
        }
    }

    return edges;
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
        << "\n[region]: " << vertex->region->getRegionName()
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

QJsonObject Graph::serialize(QJsonObject graphJson){
    if(!initializedSer){
        initializedSer=true;
        return graphJson;

    } else{
        QJsonArray verticesArray;

        for (const auto& [id, vertex] : vertices) {
            QJsonObject vertexJson;

            vertexJson["id"] = static_cast<int>(id);
            vertexJson["label"] = QString::fromStdString(vertex->label());
            vertexJson["num_of_soldiers"] = vertex->army.getSoldiers();
            vertexJson["army_type"] = QString::fromStdString(vertex->army.to_string(vertex->army.armyType()));
            vertexJson["player_id"] = vertex->player.getPlayerId();

            verticesArray.append(vertexJson);
        }

        graphJson["vertices"] = verticesArray;

        return graphJson;
    }

}


void Graph::deserialize(const QJsonObject &json) {
    QJsonArray verticesArray = json["vertices"].toArray();
    if(!initialized){
        for (const QJsonValue &value : verticesArray) {
            QJsonObject vertexJson = value.toObject();

            std::string bio = vertexJson["bio"].toString().toStdString();
            std::string label = vertexJson["label"].toString().toStdString();
            int armyCount = vertexJson["num_of_soldiers"].toInt();
            std::string armyType = vertexJson["army_type"].toString().toStdString();
            std::string terrainType = vertexJson["terrain_type"].toString().toStdString();
            QString cultureStr = vertexJson["culture"].toString();

            int playerId = (armyType == "HAJDUK") ? 1 : 2;
            CultureType culture = Culture::fromString(cultureStr);

            int cityLevel = vertexJson["cityLevel"].toInt();
            City *city = CityFactory::getCityByLevel(cityLevel);


            auto* vertex = insert_vertex(QPointF(0,0),
                          label,
                          nullptr,
                          Terrain::fromString(terrainType),
                          Army::fromString(armyType, armyCount),
                          Player::fromJson(playerId, armyType),
                          culture,
                          city,
                          nullptr);

            vertex->setBio(bio);
        }

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
        initialized = true;
    } else {
        for (const QJsonValue& value : verticesArray) {
            QJsonObject vertexJson = value.toObject();
            unsigned id = vertexJson["id"].toInt();
            int armyCount = vertexJson["num_of_soldiers"].toInt();
            std::string armyType = vertexJson["army_type"].toString().toStdString();
            int playerId = (armyType == "HAJDUK") ? 1 : (armyType == "JANISSARY" ? 2 : 0);

            vertices[id]->player = Player::fromJson(playerId, armyType);
            vertices[id]->army = Army::fromString(armyType, armyCount);
        }
    }
}

void Graph::save_to_json(const std::string &file_path) const {
    QJsonObject graphJson;

    QJsonArray verticesArray;
    for (const auto& [id, vertex] : vertices) {
        QJsonObject vertexJson;
        vertexJson["id"] = static_cast<int>(vertex->id());
        vertexJson["label"] = QString::fromStdString(vertex->label());
        vertexJson["label_path"] = QString(":/resources/Images/%1.png").arg(QString::fromStdString(vertex->label()));
        vertexJson["army_type"] = QString::fromStdString(vertex->army.to_string(vertex->army.armyType()));
        vertexJson["num_of_soldiers"] = vertex->army.getSoldiers();
        vertexJson["position"] = QJsonObject{
            {"x", static_cast<int>(vertex->position().x())},
            {"y", static_cast<int>(vertex->position().y())}
        };

        verticesArray.append(vertexJson);
    }
    graphJson["vertices"] = verticesArray;

    QJsonArray edgesArray;
    for (const auto& [vertex, edges] : m_adj_list) {
        for (const Edge& edge : edges) {
            QJsonObject edgeJson;
            edgeJson["from"] = static_cast<int>(edge.from());
            edgeJson["to"] = static_cast<int>(edge.to());
            edgeJson["weight"] = edge.weight();
            edgeJson["type"] = QString::fromStdString(edge.to_string());

            edgesArray.append(edgeJson);
        }
    }
    graphJson["edges"] = edgesArray;

    FileManager::saveToFile(QString::fromStdString(file_path), graphJson);
}



}


