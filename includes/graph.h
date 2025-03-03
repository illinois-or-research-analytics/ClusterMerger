#ifndef GRAPH_H
#define GRAPH_H

#include <set>
#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <queue>
#include <iostream>

class Graph {
    public:
        Graph(){
        };

        Graph(std::string edgelist);
        void AddEdge(std::pair<int, int> edge);

        static inline char get_delimiter(std::string filepath) {
            std::ifstream clustering(filepath);
            std::string line;
            getline(clustering, line);
            if (line.find(',') != std::string::npos) {
                return ',';
            } else if (line.find('\t') != std::string::npos) {
                return '\t';
            } else if (line.find(' ') != std::string::npos) {
                return ' ';
            }
            throw std::invalid_argument("Could not detect filetype for " + filepath);
        }


        const std::set<int>& GetNodeSet() const;
        const std::map<int, std::set<int>>& GetAdjMap() const;
        const float& GetWeight(std::pair<int, int> edge) const;
        void GetConnectedComponents(std::queue<std::vector<int>>& output_queue) const;
        void SetWeight(std::pair<int, int> edge, float weight);
        Graph* Threshold(float t);
        void AddNode(int u);
        void PrintGraph() const;

    private:

        std::map<std::pair<int, int>, float> edge_weight_map;
        std::set<int> node_set;

    protected:
        std::map<int, std::set<int>> adj_map;
};

#endif
