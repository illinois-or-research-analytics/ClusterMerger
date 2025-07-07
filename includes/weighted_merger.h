#ifndef WEIGHTED_MERGER_H
#define WEIGHTED_MERGER_H


#include "cluster_merger.h"
#include "graph.h"
#include <condition_variable>
#include <thread>

#include <set>


class WeightedMerger : public ClusterMerger {
    public:
        WeightedMerger(std::string edgelist, std::string clustering_list, int weighting_strategy, float threshold, int num_processors, std::string output_file, std::string output_weighted_graph, std::string log_file, int log_level) : ClusterMerger(num_processors, output_file, log_file, log_level), edgelist(edgelist), clustering_list(clustering_list), output_weighted_graph(output_weighted_graph), weighting_strategy(weighting_strategy), threshold(threshold) {
            this->graph = new Graph(edgelist, false);
            this->InitClusteringList();
            if (this->threshold < 0) {
                this->threshold = 0;
            }
        };

        int main() override;

        virtual ~WeightedMerger() {
            delete this->graph;
        }

        void InitClusteringList();
        void InitGraph();

        void ConstructGraph();
        void NodeWorker();
        float ScoreEdge(std::pair<int, int> edge);
        float ScoreEdgeTypeZero(std::pair<int, int> edge);
        float ScoreEdgeTypeOne(std::pair<int, int> edge);
        float ScoreEdgeTypeTwo(std::pair<int, int> edge);

    protected:
        std::queue<int> input_queue;
        std::map<std::pair<int, int>, float> output_map;
        std::mutex input_queue_mutex;
        std::mutex output_map_mutex;
        std::string edgelist;
        std::string clustering_list;
        std::string output_weighted_graph;
        std::vector<std::map<int,int>> clustering_list_vec;
        Graph* graph;
        int weighting_strategy;
        float threshold;
};

#endif
