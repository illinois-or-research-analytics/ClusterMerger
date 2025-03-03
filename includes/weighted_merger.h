#ifndef WEIGHTED_MERGER_H
#define WEIGHTED_MERGER_H


#include "cluster_merger.h"
#include "graph.h"
#include <condition_variable>
#include <thread>

#include <set>


class WeightedMerger : public ClusterMerger {
    public:
        WeightedMerger(std::string edgelist, std::string clustering_list, int num_processors, std::string output_file, std::string log_file, int log_level) : ClusterMerger(num_processors, output_file, log_file, log_level), edgelist(edgelist), clustering_list(clustering_list) {
            this->graph = new Graph(edgelist);
            this->InitClusteringList();
        };

        int main() override;

        virtual ~WeightedMerger() {
            delete this->graph;
        }

        void InitClusteringList();
        void InitGraph();

        void ConstructGraph();
        void NodeWorker();

    protected:
        std::queue<int> input_queue;
        std::map<std::pair<int, int>, float> output_map;
        std::mutex input_queue_mutex;
        std::mutex output_map_mutex;
        std::string edgelist;
        std::string clustering_list;
        std::vector<std::map<int,int>> clustering_list_vec;
        Graph* graph;
};

#endif
