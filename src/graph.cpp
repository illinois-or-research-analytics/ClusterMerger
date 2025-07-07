#include "graph.h"


Graph::Graph(std::string edgelist, bool directed): directed(directed), edgelist(edgelist){
    char delimiter = Graph::get_delimiter(this->edgelist);
    std::ifstream input_edgelist(this->edgelist);
    std::string line;
    while(std::getline(input_edgelist, line)) {
        std::stringstream ss(line);
        std::string current_value;
        std::vector<std::string> current_line;
        while(std::getline(ss, current_value, delimiter)) {
            current_line.push_back(current_value);
        }
        std::string citing = current_line[0];
        if(citing == "#citing") {
            continue;
        }
        int integer_cited = std::atoi(current_line[1].c_str());
        int integer_citing = std::atoi(citing.c_str());
        this->AddEdge({integer_citing, integer_cited});
        this->SetWeight({integer_citing, integer_cited}, 0);
    }
}

Graph* Graph::Threshold(float t) {
    Graph* thresholded_graph = new Graph(this->directed);
    for(auto const& [u,u_neighbors] : this->GetAdjMap()) {
        for(const int& v : u_neighbors) {
            if (u < v) {
                if (this->GetWeight({u, v}) >= t) {
                    thresholded_graph->AddEdge({u, v});
                    thresholded_graph->SetWeight({u, v}, this->GetWeight({u, v}));
                }
            }
        }
    }
    return thresholded_graph;
}

void Graph::GetConnectedComponents(std::queue<std::vector<int>>& output_queue) const {
    std::set<int> visited;
    for(const int& node : this->GetNodeSet()) {
        if(!visited.contains(node)) {
            std::vector<int> current_cluster;
            std::queue<int> to_visit;
            to_visit.push(node);
            while(!to_visit.empty()) {
                int current_node = to_visit.front();
                to_visit.pop();
                visited.insert(current_node);
                current_cluster.push_back(current_node);
                if(this->GetAdjMap().contains(current_node)) {
                    for(const int& v : this->GetAdjMap().at(current_node)) {
                        if(!visited.contains(v)) {
                            to_visit.push(v);
                        }
                    }
                }
            }
            output_queue.push(current_cluster);
        }
    }
}

void Graph::AddEdge(std::pair<int, int> edge) {
    if (this->directed) {
        this->adj_map[edge.first].insert(edge.second);
    } else {
        this->adj_map[edge.first].insert(edge.second);
        this->adj_map[edge.second].insert(edge.first);
    }
    this->AddNode(edge.first);
    this->AddNode(edge.second);
}

const float& Graph::GetWeight(std::pair<int, int> edge) const {
    if(this->directed) {
        return this->edge_weight_map.at(edge);
    } else {
        if(edge.first < edge.second) {
            return this->edge_weight_map.at({edge.first, edge.second});
        } else {
            return this->edge_weight_map.at({edge.second, edge.first});
        }
    }
}

void Graph::SetWeight(std::pair<int, int> edge, float weight) {
    if(this->directed) {
        this->edge_weight_map[edge] = weight;
    } else {
        if(edge.first < edge.second) {
            this->edge_weight_map[{edge.first, edge.second}] = weight;
        } else {
            this->edge_weight_map[{edge.second, edge.first}] = weight;
        }
    }
}

void Graph::AddNode(int u) {
    this->node_set.insert(u);
}

const std::set<int>& Graph::GetNodeSet() const {
    return this->node_set;
}
const std::map<int, std::set<int>>& Graph::GetAdjMap() const {
    return this->adj_map;
}

void Graph::PrintGraph() const {
    if (this->directed) {
        std::cout << "Graph is directed" << std::endl;
        for(auto const& [u,u_neighbors] : this->GetAdjMap()) {
            for(const int& v : u_neighbors) {
                std::cout << u << "-" << v << "(" << this->GetWeight({u, v}) << ")"<< std::endl;
            }
        }
    } else {
        std::cout << "Graph is undirected" << std::endl;
        for(auto const& [u,u_neighbors] : this->GetAdjMap()) {
            for(const int& v : u_neighbors) {
                if (u < v) {
                    std::cout << u << "-" << v << "(" << this->GetWeight({u, v}) << ")"<< std::endl;
                }
            }
        }
    }
}

void Graph::WriteGraph(std::string output_file) const {
    std::ofstream output_filehandle(output_file);
    if (this->directed) {
        for(auto const& [u,u_neighbors] : this->GetAdjMap()) {
            for(const int& v : u_neighbors) {
                output_filehandle << u << "\t" << v << "\t" << this->GetWeight({u, v}) << std::endl;
            }
        }
    } else {
        for(auto const& [u,u_neighbors] : this->GetAdjMap()) {
            for(const int& v : u_neighbors) {
                if (u < v) {
                    output_filehandle << u << "\t" << v << "\t" << this->GetWeight({u, v}) << std::endl;
                }
            }
        }
    }
    output_filehandle.close();
}
