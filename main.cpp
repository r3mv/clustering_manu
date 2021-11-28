#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>

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
    std::vector<std::string> split;
    std::string val;
    while (std::getline(ss, val, SEPARATOR)) {
      split.push_back(val);
    }
    if (!split.size() == 18) {
      throw std::runtime_error("Expected 18 values");
    } else {
      double lat = std::stod(split[16]);
      double lon = std::stod(split[17]);
      data.push_back(lat);
      data.push_back(lon);
      numRows++;
    }
  }
  infile.close();
}


double degToRad(double val_degree) {
  return val_degree * M_PI / 180;
}

/**
 * lat and lon
 * are signed decimal degrees without compass direction
 * where negative indicates west/south (e.g. 40.7486, -73.9864)
 * @return distance in meters
 * @see http://www.movable-type.co.uk/scripts/latlong.html
 */

double
distanceInMeters(double lat1_degree, double lon1_degree,
		 double lat2_degree, double lon2_degree) {
  double earth_radius_meters = 6371e3;
  double lat1_radian = degToRad(lat1_degree);
  double lat2_radian = degToRad(lat2_degree);
  double delta_lat_radian = degToRad(lat2_degree-lat1_degree);
  double delta_lon_radian = degToRad(lon2_degree-lon1_degree);

  double a = sin(delta_lat_radian/2)*sin(delta_lat_radian/2)+
    cos(lat1_radian)*cos(lat2_radian)*
    sin(delta_lon_radian/2)*sin(delta_lon_radian/2);
  double c = 2*atan2(sqrt(a), sqrt(1-a));
  return earth_radius_meters * c; // in metres
}

void
computeDistances(const std::vector<double> &data,
		 int numRows,
		 int numCols,
		 std::vector<double> &distances) {
  distances = std::vector<double>(data.size(), -std::numeric_limits<double>::max());
  for (int i = 1; i < numRows; ++i) {
    for (int j = 0; j < i; ++j) {
      // assume data is lat/lon coordinates, compute distances in m between the points
      
      
    }
  }
}
		 

void
computeClustering(const std::vector<double> distances,
		  std::vector<int> &clusters,
		  int maxSize) {

}

int
main(int argc,char **argv){

  int numRows = -1;
  int numCols = -1;
  std::vector<double> data;
  importData(DATA_FILE, data, numRows, numCols);

  std::vector<double> distances;
  computeDistances(data, numRows, numCols, distances);

  std::vector<int> clusters(numRows);
  for (int i = 0;i < numRows;++i){
    clusters[i] = i;
  }

  computeClustering(distances, clusters, MAX_CLUSTER_SIZE);

  return 0;

}
