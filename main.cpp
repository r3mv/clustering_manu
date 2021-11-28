#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>

const std::string DATA_FILE = "data.csv";
const int MAX_CLUSTER_SIZE = 26;
const char SEPARATOR = ';';
const int LATITUDE_COLUMN_INDEX = 16;
const int LONGITUDE_COLUMN_INDEX = 17;
  

void
importData(const std::string &filename,
	   std::vector<double> &latitudes_degree,
	   std::vector<double> &longitudes_degree) {
  latitudes_degree.clear();
  longitudes_degree.clear();
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
    double lat_degree = std::stod(split.at(LATITUDE_COLUMN_INDEX));
    double lon_degree = std::stod(split.at(LONGITUDE_COLUMN_INDEX));
    latitudes_degree.push_back(lat_degree);
    longitudes_degree.push_back(lon_degree);
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
computeDistances(const std::vector<double> &latitudes_degree,
		 const std::vector<double> &longitudes_degree,
		 std::vector<std::vector<double>> &distances_meter) {
  distances_meter = std::vector<std::vector<double>>(latitudes_degree.size(),
						     std::vector<double>(latitudes_degree.size(),std::numeric_limits<double>::max()));
  for (int i = 1; i < latitudes_degree.size(); ++i) {
    for (int j = 0; j < i; ++j) {
      // assume data is lat/lon coordinates, compute distances in m between the points
      double d = distanceInMeters(latitudes_degree[i], longitudes_degree[i],
						latitudes_degree[j], longitudes_degree[j]);
      distances_meter[i][j] = distances_meter[j][i] = d;
    }
  }
}
		 

void
computeClustering(const std::vector<std::vector<double>>& distances,
		  std::vector<int> &clusters,
		  int maxSize) {

}

int
main(int argc,char **argv){

  int numRows = -1;
  int numCols = -1;
  std::vector<double> latitudes_degree;
  std::vector<double> longitudes_degree;
  importData(DATA_FILE, latitudes_degree, longitudes_degree);

  std::vector<std::vector<double>> distances; // unoptimized n² distance matrix
  computeDistances(latitudes_degree, longitudes_degree, distances);

  std::vector<int> clusters(numRows);
  for (int i = 0;i < numRows;++i){
    clusters[i] = i;
  }

  computeClustering(distances, clusters, MAX_CLUSTER_SIZE);

  return 0;

}
