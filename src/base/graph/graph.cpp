#include "graph.hpp"

namespace graph {

Graph::Graph() = default;
Graph::~Graph(){
    for(auto& pair : vertices)
        delete pair.second;
}

Vertex* Graph::insert_vertex(QPointF position, const std::string &label,
                             MapLayer *map_layer, Terrain territory, Army army,
                             Player player, City* city, Region* region) {
    Vertex* vertex = new Vertex(position, label, map_layer, territory, army, player, city, region);

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
        << "\n----------------------" << std::endl;
    }
}

}
