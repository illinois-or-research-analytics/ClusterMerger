#ifndef DISJOINT_MERGER_H
#define DISJOINT_MERGER_H

#include "cluster_merger.h"
#include <set>


class DisjointMerger : public ClusterMerger {
    public:
        DisjointMerger(std::string clustering_list, int num_processors, std::string output_file, std::string log_file, int log_level) : ClusterMerger(num_processors, output_file, log_file, log_level), clustering_list(clustering_list) {
            this->InitClusteringList();
        };

        int main() override;

        void InitClusteringList();
        void GreedyMerge();


    protected:
        std::string clustering_list;
        std::queue<std::map<int,std::vector<int>>> clustering_list_queue;
};

#endif
