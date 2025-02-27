#!/bin/sh
rm ./cluster_merger
rm -r build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
# cmake -DCMAKE_BUILD_TYPE=Debug ..
cp compile_commands.json ../
make -j 1
ln -s $(readlink -f ./bin/cluster_merger) ../cluster_merger
