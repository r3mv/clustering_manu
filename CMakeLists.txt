cmake_minimum_required(VERSION 3.0)
project(clustering_manu)            # this sets the project name

# These instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables 
# `sources` and `data`.
file(GLOB_RECURSE sources *.cpp *.hpp)


add_executable(clusteringManu main.cpp ${sources})

