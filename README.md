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
- Weighting strategy is a selector for different edge weighting schemes. Edge weighting scheme 0 will weight each edge by # co-cluster / # cluster where # co-cluster is the number of input clusterings that co-clustered the endpoints of the edge and # cluster is the number of clustering methods that have both endpoints of the edge in non-singleton clusters. Edge weighting scheme 1 will simply weight each edge by the integer value of # co-cluster. In all edge weighting schemes, all edges have zero edge weight by default to prevent divide by zero errors. For example, if there is an edge in the input graph whose endpoints were never clustered by any of the input clustering methods, it would have a weight of $\frac{0}{0}$ for weighting scheme 0 so the default value of 0 gets applied to the edge weight.
- threshold is a value for which any edge with edge weight less the threshold is removed. Do not leave this as the default value before reading the section below about thresholding for additinonal information.
- num processors directly controls the number of vertices that are processed in parellel
- output file is an unused argument currently.
- output weigted graph is the file path to which a three column csv edgelist is writen where the format is (source,target,weight) for each row.
- log file is the file path to which log information gets written
- log level is an integer with values 0, 1, and 2 corresponding to silent, info, and verbose modes.


## Thresholding
Regardless of the weighting strategy, each edge is only kept in the graph if the weight of the edge is at least the threshold value. This implies the following.
- To keep every edge, set this value to -1 as no edge can have a negative weight. All edge weights will be at least -1.
- To keep every non-zero edge, set this value to be in the range $(0, w_{min})$ where $w_{min}$ is the minimum possible weight given the weighting strategy and the number of input clusterings. For example, if the weighting strategy is 0, then the minimum possible weight is $\frac{1}{n}$.
