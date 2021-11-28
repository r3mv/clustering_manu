#include <iostream>
#include <fstream>
#include <vector>

const std::string filename = "data.csv";
const int MAX_CLUSTER_SIZE = 26;
const char SEPARATOR = ';';
  




int
main(int argc,char **argv){

  int numRows = -1;
  int numCols = -1;
  std::vector<double> data;
  importData(data, filename, numRows, numCols);

  std::vector<double> distances;
  computeDistances(data, numRows, numCols);

  std::vector<int> clusters(numRows);
  for (int i = 0;i < numRows;++i){
    clusters[i] = i;
  }

  computeClustering(distances, clusters, MAX_CLUSTER_SIZE);

  return 0;

}
