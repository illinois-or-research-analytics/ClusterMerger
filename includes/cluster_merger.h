#ifndef CLUSTER_MERGER_H
#define CLUSTER_MERGER_H

#include <cmath>
#include <chrono>
#include <condition_variable>
#include <random>
#include <thread>
#include <map>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <queue>


enum Log {info, debug, error = -1};

class ClusterMerger {
    private:
        static bool file_is_empty(std::string filepath) {
            std::ifstream clustering(filepath);
            return clustering.peek() == std::ifstream::traits_type::eof();
        }

    public:
        ClusterMerger(int num_processors, std::string output_file, std::string log_file, int log_level) : num_processors(num_processors), output_file(output_file), log_file(log_file), log_level(log_level) {
            if(this->log_level > -1) {
                this->start_time = std::chrono::steady_clock::now();
                this->log_file_handle.open(this->log_file);
            }
            this->num_calls_to_log_write = 0;
        };

        virtual ~ClusterMerger() {
            if(this->log_level > -1) {
                this->log_file_handle.close();
            }
        }

        virtual int main() = 0;
        int WriteToLogFile(std::string message, Log message_type);
        void WriteClusterQueue();


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

        static inline std::map<int, std::vector<int>> ReadCommunities(std::string existing_clustering) {
            std::map<int, std::vector<int>> partition_map;
            std::map<int, std::vector<int>> non_singleton_partition_map;

            if (file_is_empty(existing_clustering)) {
                return non_singleton_partition_map;
            }

            char delimiter = ClusterMerger::get_delimiter(existing_clustering);
            std::ifstream existing_clustering_file(existing_clustering);
            std::string line;
            int line_no = 0;
            while(std::getline(existing_clustering_file, line)) {
                std::stringstream ss(line);
                std::string current_value;
                std::vector<std::string> current_line;
                while(std::getline(ss, current_value, delimiter)) {
                    current_line.push_back(current_value);
                }
                std::string node_id = current_line[0];
                if (line_no == 0) {
                    line_no ++;
                    continue;
                }
                int cluster_id = std::atoi(current_line[1].c_str());
                int integer_node_id = std::atoi(node_id.c_str());
                partition_map[cluster_id].push_back(integer_node_id);
                line_no ++;
            }
            for (auto const& [cluster_id, cluster_vec] : partition_map) {
                if (cluster_vec.size() > 1) {
                    non_singleton_partition_map[cluster_id] = cluster_vec;
                }
            }
            return non_singleton_partition_map;
        }


    protected:
        int num_processors;
        std::string output_file;
        std::string log_file;
        std::chrono::steady_clock::time_point start_time;
        std::ofstream log_file_handle;
        int log_level;
        int num_calls_to_log_write;
        static inline std::queue<std::vector<int>> output_cluster_queue;
};

#endif
