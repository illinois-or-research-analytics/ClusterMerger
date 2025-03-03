#include "weighted_merger.h"

void WeightedMerger::InitClusteringList() {
    std::ifstream clustering_list_file(this->clustering_list);
    std::string line;
    while(std::getline(clustering_list_file, line)) {
        std::map<int, std::vector<int>> current_non_singleton_clustering = ClusterMerger::ReadCommunities(line);
        std::map<int, int> node_to_cluster_id_map;
        for (auto const& [cluster_id, cluster_vec] : current_non_singleton_clustering) {
            for (size_t i = 0; i < cluster_vec.size(); i ++) {
                node_to_cluster_id_map[cluster_vec[i]] = cluster_id;
                this->graph->AddNode(cluster_vec[i]);
            }
        }
        this->clustering_list_vec.push_back(node_to_cluster_id_map);
    }
    this->WriteToLogFile("Read " + std::to_string(this->clustering_list_vec.size()) + " clusterings", Log::info);
}

void WeightedMerger::NodeWorker() {
    while (true) {
        std::unique_lock<std::mutex> input_queue_lock{this->input_queue_mutex};
        int current_node_id = this->input_queue.front();
        this->input_queue.pop();
        input_queue_lock.unlock();
        if (current_node_id == -1) { // sentinel node
            return;
        }

        std::map<int, float> current_edge_map;
        for(const int& v : this->graph->GetAdjMap().at(current_node_id)) {
            int co_cluster_count = 0;
            int cluster_count = 0;
            for(size_t i = 0; i < this->clustering_list_vec.size(); i ++) {
                const std::map<int, int>& current_clustering = this->clustering_list_vec.at(i);
                if (current_clustering.contains(current_node_id) && current_clustering.contains(v)) {
                    cluster_count += 1;
                    if (current_clustering.at(current_node_id) == current_clustering.at(v)) {
                        co_cluster_count += 1;
                    }
                }
            }
            if (co_cluster_count > 0) {
                current_edge_map[v] = (float)co_cluster_count / cluster_count;
            }
        }

        if (current_edge_map.size() > 0) {
            std::unique_lock<std::mutex> output_map_lock{this->output_map_mutex};
            for (auto const& [v, weight] : current_edge_map) {
                this->output_map[{current_node_id, v}] = weight;
            }
            output_map_lock.unlock();
        }
    }
}

void WeightedMerger::ConstructGraph() {
    for(auto const& [u,u_neighbors] : this->graph->GetAdjMap()) {
        this->input_queue.push(u); // input queue only has citing nodes
    }
    std::vector<std::thread> thread_vector;
    for(int i = 0; i < this->num_processors; i ++) {
        this->input_queue.push(-1);
    }
    for(int i = 0; i < this->num_processors; i ++) {
        thread_vector.push_back(std::thread(&WeightedMerger::NodeWorker, this));
    }
    for(int i = 0; i < this->num_processors; i ++) {
        thread_vector[i].join();
    }
    for (auto const& [edge, weight] : this->output_map) {
        this->graph->AddEdge(edge);
        this->graph->SetWeight(edge, weight);
    }
    this->WriteToLogFile("Graph constructed with " + std::to_string(this->graph->GetNodeSet().size()) + " nodes", Log::info);
    /* for(auto const& [u,u_neighbors] : this->graph->GetAdjMap()) { */
    /*     for(const int& v : u_neighbors) { */
    /*         this->WriteToLogFile(std::to_string(u) + "-" + std::to_string(v) + " : " + std::to_string(this->graph->GetWeight({u, v})), Log::info); */
    /*     } */
    /* } */
}

int WeightedMerger::main() {
    this->ConstructGraph();
    this->graph->PrintGraph();
    this->graph = this->graph->Threshold(0.5); // edge weight < 0.5 is removed
    this->graph->PrintGraph();
    this->graph->GetConnectedComponents(this->output_cluster_queue);
    this->WriteClusterQueue();
    return 0;
}
