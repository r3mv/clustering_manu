#include <regex>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <deque>
#include <cmath>
#include <limits>

const int MAX_CLUSTER_SIZE = 26;
const char SEPARATOR = ';';
const int LAST_NAME_COLUMN_INDEX = 2;
const int FIRST_NAME_COLUMN_INDEX = 3;
const int EMAIL_COLUMN_INDEX = 12;
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
  while (std::getline(infile, buffer)) {
    std::stringstream ss(buffer);
    std::vector<std::string> split;
    std::string val;
    while (std::getline(ss, val, SEPARATOR)) {
      split.push_back(val);
    }
    // read coordinates replace , with .
    std::string &lat_str = split.at(LATITUDE_COLUMN_INDEX);
    std::replace(lat_str.begin(), lat_str.end(), ',','.');
    std::string &lon_str = split.at(LONGITUDE_COLUMN_INDEX);
    std::replace(lon_str.begin(), lon_str.end(), ',','.');
    double lat_degree = std::stod(lat_str);
    double lon_degree = std::stod(lon_str);
    latitudes_degree.push_back(lat_degree);
    longitudes_degree.push_back(lon_degree);
  }
  infile.close();
}

void
exportDataToJson(const std::string &infilename, const std::string &outfilename) {
  std::ifstream infile(infilename);
  std::ofstream outfile(outfilename);
  outfile << "var dataPoints = [";
  bool first = true;
  std::string buffer;
  while (std::getline(infile, buffer)) {
    if (first) {
      first = false;
    } else {
      outfile << "," << std::endl;
    }
    std::stringstream ss(buffer);
    std::vector<std::string> split;
    std::string val;
    while (std::getline(ss, val, SEPARATOR)) {
      split.push_back(val);
    }
    std::string &firstName = split.at(FIRST_NAME_COLUMN_INDEX);
    std::string &lastName = split.at(LAST_NAME_COLUMN_INDEX);
    std::string &email = split.at(EMAIL_COLUMN_INDEX);
    std::string &lat_str = split.at(LATITUDE_COLUMN_INDEX);
    std::replace(lat_str.begin(), lat_str.end(), ',','.');
    std::string &lon_str = split.at(LONGITUDE_COLUMN_INDEX);
    std::replace(lon_str.begin(), lon_str.end(), ',','.');
    double lat_degree = std::stod(lat_str);
    double lon_degree = std::stod(lon_str);
    outfile << "{ "
	    << "firstName : \"" << firstName << "\","
	    << "lastName : \"" << lastName << "\","
	    << "email : \'" << email << "\',"
	    << "latitude : " << lat_degree << ","
	    << "longitude : " << lon_degree
	    << "}";
  }
  infile.close();
  outfile << "];";
  outfile.close();
  
}


void
exportDataToGeoJson(const std::string &infilename, const std::string &outfilename) {
  std::ifstream infile(infilename);
  std::ofstream outfile(outfilename);
  outfile << "var geoData = {" << std::endl
	  << "\"type\": \"FeatureCollection\"," << std::endl
	  << "\"features\": [" << std::endl;
  
  int cpt = 0;
  bool first = true;
  std::string buffer;
  while (std::getline(infile, buffer)) {
    if (first) {
      first = false;
    } else {
      outfile << "," << std::endl;
    }
    std::stringstream ss(buffer);
    std::vector<std::string> split;
    std::string val;
    while (std::getline(ss, val, SEPARATOR)) {
      split.push_back(val);
    }
    std::string &firstName = split.at(FIRST_NAME_COLUMN_INDEX);
    std::string &lastName = split.at(LAST_NAME_COLUMN_INDEX);
    std::string &email = split.at(EMAIL_COLUMN_INDEX);
    std::string &lat_str = split.at(LATITUDE_COLUMN_INDEX);
    std::replace(lat_str.begin(), lat_str.end(), ',','.');
    std::string &lon_str = split.at(LONGITUDE_COLUMN_INDEX);
    std::replace(lon_str.begin(), lon_str.end(), ',','.');
    double lat_degree = std::stod(lat_str);
    double lon_degree = std::stod(lon_str);
    outfile << "{ " << std::endl
	    << "\"type\": \"Feature\"," << std::endl
	    << "\"id\": " << cpt++  << "," << std::endl
	    << "\"geometry\": {" << std::endl
	    << "    \"type\": \"Point\"," << std::endl
	    << "    \"coordinates\": [" << lon_degree << "," << lat_degree << "]" << std::endl
	    << "}," << std::endl
	    << "\"properties\": {" << std::endl
	    << "\"firstName\" : \"" << firstName << "\"," << std::endl
	    << "\"lastName\" : \"" << lastName << "\"," << std::endl
	    << "\"email\" : \"" << email << "\"" << std::endl
	    << "}" << std::endl
      	    << "}" << std::endl;
  }
  infile.close();
  outfile << "]};";
  outfile.close();
  
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
		 

struct Merge {
  int index1;
  int index2;
  double distance;

  Merge(int index1 = -1,
	int index2 = -1,
	double distance = std::numeric_limits<double>::max()):
    index1(index1),
    index2(index2),
    distance(distance)
  {}

  virtual
  ~Merge(){}

  friend std::ostream& operator<<(std::ostream &out, const Merge& m) {
    out << m.index1 << " " << m.index2 << ":" << m.distance;
    return out;
  }
};


double
findMin(const std::vector<std::vector<double>> &distances,
	int &i1,
	int &i2){

  int numElem = distances.size();
  double min = std::numeric_limits<double>::max();
  for (int i = 1; i < numElem; ++i) {
    for(int j =0; j < i; ++j) {
      if (distances[i][j] < min) {
	min = distances[i][j];
	i1 = j;
	i2 = i;
      }
    }
  }
  return min;
}

void
updateDistanceMatrix(std::vector<std::vector<double>> &distances,
		     const Merge &m,
		     std::vector<std::deque<int>> &clusters) {
  // merge elements of index2 cluster into index1
  // 1. update distance matrix to large value for elements in the same cluster
  for (int index1 : clusters[m.index1]) {
    for (int index2 : clusters[m.index2]) {
      distances[index1][index2] = distances[index2][index1] = std::numeric_limits<double>::max();
    }
  }
  // 2. copy element list
  std::copy(clusters[m.index2].begin(), clusters[m.index2].end(),std::back_inserter(clusters[m.index1]));
  // 3. merge old cluster
  clusters[m.index2].clear();

  // compute minimal distance from cluster 1 to other clusters
  // and update distance matrix
  const std::deque<int> &cluster1=clusters[m.index1];
  for (int i = 0; i < clusters.size(); ++i) {
    if (i != m.index1) {
      const std::deque<int> &clusterX = clusters[i];
      double min = std::numeric_limits<double>::max();
      for (int elemX : clusterX) {
	for (int elemOfCluster1 : cluster1) {
	  min = std::min(min, distances[elemX][elemOfCluster1]);
	}
      }
      // update distance matrix from cluster 1 to cluster X
      for (int elemX : clusterX) {
	for (int elemOfCluster1 : cluster1) {
	  distances[elemX][elemOfCluster1] = distances[elemOfCluster1][elemX] = min;
	}
      }
    }
  }
}


/**
 * Agglomerative hierachical clustering based on distances
 * with single linkage
 */
void
computeLinkage(std::vector<std::vector<double>>& distances,
		  std::vector<Merge> &linkages) {

  int i1 = -1;
  int i2 = -1;
  int numElem = distances[0].size();
  linkages.clear();
  std::vector<std::deque<int>> clusters(numElem);
  for (int i = 0;i < numElem; ++i) {
    clusters[i].push_back(i);
  }
  while(linkages.size() != numElem-1) {
    double minDist = findMin(distances, i1,i2);
    Merge m(i1, i2, minDist);
    updateDistanceMatrix(distances, m, clusters);
    linkages.push_back(m);
  }
}

int
main(int argc,char **argv){

  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <data_file>" << std::endl;
    return EXIT_SUCCESS;
  }
  try {
    // std::cout<< distanceInMeters(44.531082109, -0.341531541732,
    // 				44.4230431057, -0.540077855567) << std::endl;

    const std::string dataFile = argv[1];
    const std::string jsonDataFile = std::regex_replace(dataFile, std::regex("\\.csv"), ".js");
    exportDataToJson(dataFile, jsonDataFile);

    const std::string jsonGeoData = "geodata.js";
    exportDataToGeoJson(dataFile, jsonGeoData);

    
    std::vector<double> latitudes_degree;
    std::vector<double> longitudes_degree;
    importData(dataFile, latitudes_degree, longitudes_degree);
    int numElements = latitudes_degree.size();
    
    std::vector<std::vector<double>> distances; // unoptimized n² distance matrix
    computeDistances(latitudes_degree, longitudes_degree, distances);

    std::vector<Merge> linkages;
    computeLinkage(distances, linkages);
    int cpt = 0;
    for (Merge &m : linkages) {
      std::cout << "Merge " << ++cpt << "=" << m << std::endl;
    }
    return EXIT_SUCCESS;
  }
  catch (std::exception &e) {
    std::cerr << "Exception raised:" << e.what();
    return EXIT_FAILURE;
  }
}



// Mettre dans le clustering des gens qui sont dans la même circonscription
// code IEN
// possibilité d'affecter qqun à un autre groupe
// savoir combien il y a de personnes par groupe
