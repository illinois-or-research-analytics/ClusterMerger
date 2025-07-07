#include <iostream>

#include "argparse.h"
#include "disjoint_merger.h"
#include "weighted_merger.h"
#include "library.h"


int main(int argc, char* argv[]) {
    argparse::ArgumentParser main_program("cluster-merger");

    argparse::ArgumentParser dm("Disjoint");
    dm.add_description("Disjoint merger");

    dm.add_argument("--clustering-list")
        .required()
        .help("List of clustering files");
    dm.add_argument("--num-processors")
        .default_value(int(1))
        .help("Number of processors")
        .scan<'d', int>();
    dm.add_argument("--output-file")
        .required()
        .help("Output clustering file");
    dm.add_argument("--log-file")
        .required()
        .help("Output log file");
    dm.add_argument("--log-level")
        .default_value(int(1))
        .help("Log level where 0 = silent, 1 = info, 2 = verbose")
        .scan<'d', int>();


    argparse::ArgumentParser wm("Weighted");
    wm.add_description("Weighted merger");

    wm.add_argument("--edgelist")
        .required()
        .help("Input edgelist");
    wm.add_argument("--clustering-list")
        .required()
        .help("List of clustering files");
    wm.add_argument("--weighting-strategy")
        .default_value(int(1))
        .help("strategy for weighting edges")
        .scan<'d', int>();
    wm.add_argument("--threshold")
        .default_value(float(-1))
        .help("Threshold")
        .scan<'f', float>();
    wm.add_argument("--num-processors")
        .default_value(int(1))
        .help("Number of processors")
        .scan<'d', int>();
    wm.add_argument("--output-file")
        .required()
        .help("Output clustering file");
    wm.add_argument("--output-weighted-graph")
        .default_value("")
        .help("Output weighted graph");
    wm.add_argument("--log-file")
        .required()
        .help("Output log file");
    wm.add_argument("--log-level")
        .default_value(int(1))
        .help("Log level where 0 = silent, 1 = info, 2 = verbose")
        .scan<'d', int>();

    main_program.add_subparser(dm);
    main_program.add_subparser(wm);
    try {
        main_program.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << main_program;
        std::exit(1);
    }

    if(main_program.is_subcommand_used(dm)) {
        std::string clustering_list = dm.get<std::string>("--clustering-list");
        int num_processors = dm.get<int>("--num-processors");
        std::string output_file = dm.get<std::string>("--output-file");
        std::string log_file = dm.get<std::string>("--log-file");
        int log_level = dm.get<int>("--log-level") - 1; // so that enum is cleaner
        ClusterMerger* dm = new DisjointMerger(clustering_list, num_processors, output_file, log_file, log_level);
        dm->main();
        delete dm;
    } else if(main_program.is_subcommand_used(wm)) {
        std::string edgelist = wm.get<std::string>("--edgelist");
        std::string clustering_list = wm.get<std::string>("--clustering-list");
        int weighting_strategy = wm.get<int>("--weighting-strategy");
        float threshold = wm.get<float>("--threshold");
        int num_processors = wm.get<int>("--num-processors");
        std::string output_file = wm.get<std::string>("--output-file");
        std::string output_weighted_graph = wm.get<std::string>("--output-weighted-graph");
        std::string log_file = wm.get<std::string>("--log-file");
        int log_level = wm.get<int>("--log-level") - 1; // so that enum is cleaner
        ClusterMerger* wm = new WeightedMerger(edgelist, clustering_list, weighting_strategy, threshold, num_processors, output_file, output_weighted_graph, log_file, log_level);
        wm->main();
        delete wm;
    }
}
