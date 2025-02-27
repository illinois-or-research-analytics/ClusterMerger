#include "disjoint_merger.h"

void DisjointMerger::InitClusteringList() {
    std::ifstream clustering_list_file(this->clustering_list);
    std::string line;
    while(std::getline(clustering_list_file, line)) {
        this->clustering_list_queue.push(ClusterMerger::ReadCommunities(line));
    }
    this->WriteToLogFile("Read " + std::to_string(this->clustering_list_queue.size()) + " clusterings", Log::info);
}

void DisjointMerger::GreedyMerge() {
    std::set<int> node_set;
    int num_communities = 0;
    while(!this->clustering_list_queue.empty()) {
        std::map<int,std::vector<int>> current_clustering = this->clustering_list_queue.front();
        this->clustering_list_queue.pop();
        for (auto const& [cluster_id, cluster_vec] : current_clustering) {
            std::vector<int> current_cluster;
            for (size_t i = 0; i < cluster_vec.size() ; i ++) {
                if (!node_set.contains(cluster_vec[i])) {
                    current_cluster.push_back(cluster_vec[i]);
                }
                node_set.insert(cluster_vec[i]);
            }
            if (current_cluster.size() > 1) {
                this->output_cluster_queue.push(current_cluster);
                num_communities ++;
            }
        }
    }
    this->WriteToLogFile("Found " + std::to_string(num_communities) + " clusters", Log::info);
}

int DisjointMerger::main() {
    this->GreedyMerge();
    this->WriteClusterQueue();
    return 0;
}
