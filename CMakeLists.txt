cmake_minimum_required(VERSION 3.23)
#[[ In-source build check ]]
if("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed.")
    message(WARNING "Remove CMakeFiles/ and CMakeCache.txt in the\
            source root before building again.")
else()
    #[[ Set project version ]]
    project(consensus_clustering)
    find_package(OpenMP REQUIRED)

    set(CLUSTER_MERGER_MAJOR_VERSION 0)
    set(CLUSTER_MERGER_MINOR_VERSION 1)
    set(CLUSTER_MERGER_PATCH_VERSION 0)
    set(CLUSTER_MERGER_VERSION
        ${CLUSTER_MERGER_MAJOR_VERSION}.
        ${CLUSTER_MERGER_MINOR_VERSION}.
        ${CLUSTER_MERGER_PATCH_VERSION})


    #[[ CONSTANTS ]]
    # Compiler options
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clangd lsp
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -march=native")
    # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -DNDEBUG -march=native")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -ggdb -fno-omit-frame-pointer")
    # File paths
    set(SOURCE_MAIN ${CMAKE_SOURCE_DIR}/src/main.cpp)
    set(SOURCE_INTERNAL_LIBS
        ${CMAKE_SOURCE_DIR}/src/weighted_merger.cpp
        ${CMAKE_SOURCE_DIR}/src/disjoint_merger.cpp
        ${CMAKE_SOURCE_DIR}/src/cluster_merger.cpp
        ${CMAKE_SOURCE_DIR}/src/graph.cpp
    )
    #[[ CONSTANTS END ]]

    #[[ Define the output binary ]]
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    add_executable(cluster_merger ${SOURCE_MAIN})

    #[[ Link internal libraries ]]
    add_library(internal_libs ${SOURCE_INTERNAL_LIBS})
    target_include_directories(internal_libs
        PUBLIC ${CMAKE_SOURCE_DIR}/includes
        PUBLIC ${CMAKE_SOURCE_DIR}/src
    )
    target_link_libraries(internal_libs PUBLIC OpenMP::OpenMP_CXX)
    target_link_libraries(cluster_merger PUBLIC internal_libs)

    #[[ Link libraries ]]
    add_subdirectory(libs)
    target_link_libraries(cluster_merger PUBLIC external_libs)

endif("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
