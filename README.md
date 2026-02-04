# ClusterMerger
ClusterMerger takes a set of input clusterings on a network and outputs a weighted network based on a specified edge-weighting scheme

## Dependencies
- C++ >= 20
- OpenMP >= 4.0
- cmake >= 3.23
- igraph (can be locally instaleld via [setup.sh](setup.sh))
- libleidenalg (can be locally instaleld via [setup.sh](setup.sh))

## One time setup
Run [setup.sh](setup.sh) to locally install [igraph](https://github.com/igraph/igraph) and [libleidenalg](https://github.com/vtraag/libleidenalg). Alternatively, just ensure that both igraph and libleidenalg libraries are discoverable by cmake.

## How to build
ClusterMerger is a standard cmake project. [easy_build_and_compile.sh](easy_build_and_compile.sh) is provided for user convenience.

## How to run
The general command is given below.
```console
./cluster_merger Weighted [--help] [--version] --edgelist VAR --clustering-list VAR [--weighting-strategy VAR] [--threshold VAR] [--num-processors VAR] --output-file VAR [--output-weighted-graph VAR] --log-file VAR [--log-level VAR]

Weighted merger

Optional arguments:
  -h, --help               shows help message and exits
  -v, --version            prints version information and exits
  --edgelist               Input edgelist [required]
  --clustering-list        List of clustering files [required]
  --weighting-strategy     strategy for weighting edges [default: 1]
  --threshold              Threshold [default: -1]
  --num-processors         Number of processors [default: 1]
  --output-file            Output clustering file [required]
  --output-weighted-graph  Output weighted graph [default: ""]
  --log-file               Output log file [required]
  --log-level              Log level where 0 = silent, 1 = info, 2 = verbose [default: 1]
```
- Input edgelist is a two column csv with source and target integer node ids on each row and a header line. The actual column values of the header row do not matter.
- Clustering list is a file with a file path to a clustering file on each line. These clustering files should be two column csvs with a header row with node\_id in the first column and cluster\_id in the second column.
- Weighting strategy is a selector for different edge weighting schemes. Specifically, edge weighting scheme 0 will weight each edge by # co-cluster / # cluster where # co-cluster is the number of input clusterings that co-clustered the endpoints of the edge and # cluster is the number of clustering methods that have both endpoints of the edge in non-singleton clusters. Otherwise, edges get a weight of 0.
- threshold is a value for which any edge with edge weight less the threshold is removed
- num processors directly controls the number of new agents that are processed in parellel for each year.
- output file is an unused argument currently.
- output weigted graph is the file path to which a three column csv edgelist is writen where the format is (source,target,weight) for each row.
- log file is the file path to which log information gets written
- log level is an integer with values 0, 1, and 2 corresponding to silent, info, and verbose modes.
