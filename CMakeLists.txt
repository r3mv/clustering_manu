cmake_minimum_required(VERSION 3.0)
project(clustering_manu)            # this sets the project name

# These instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables 
# `sources` and `data`.
set(sources GeoUtils.cpp PersonInfo.cpp main.cpp)


add_executable(clusteringManu ${sources})
target_link_libraries(clusteringManu -lm)

