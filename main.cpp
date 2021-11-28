#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

const std::string DATA_FILE = "data.csv";
const int MAX_CLUSTER_SIZE = 26;
const char SEPARATOR = ';';
  

void
importData(const std::string &filename,
	   std::vector<double> &data,
	   int &numRows,
	   int &numCols) {

  numCols = 2;
  numRows = 0;
  std::string buffer;
  std::ifstream infile(filename);
  while (infile.good()) {
    infile >> buffer;
    std::stringstream ss(buffer);
    std::vector<string> split;
    std::string val;
    while (std::getline(ss, val, SEPARATOR)) {
      split.push_back(val);
    }
    if (!split.size == 18) {
      throw std::runtime_error("Expected 18 values");
    } else {
      double lat = std::atoi(split[16]);
      double lon = std::atoi(split[17]);
      data.push_back(lat);
      data.push_back(lon);
      numRows++;
    }
  }
  infile.close();
}


int
main(int argc,char **argv){

  int numRows = -1;
  int numCols = -1;
  std::vector<double> data;
  importData(DATA_FILE, data, numRows, numCols);

  std::vector<double> distances;
  computeDistances(data, numRows, numCols);

  std::vector<int> clusters(numRows);
  for (int i = 0;i < numRows;++i){
    clusters[i] = i;
  }

  computeClustering(distances, clusters, MAX_CLUSTER_SIZE);

  return 0;

}
