#include <regex>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <deque>
#include <cmath>
#include <limits>

#include "PersonInfo.hpp"

const int MAX_CLUSTER_SIZE = 26;
const char SEPARATOR = ';';
const int LAST_NAME_COLUMN_INDEX = 2;
const int FIRST_NAME_COLUMN_INDEX = 3;
const int EMAIL_COLUMN_INDEX = 12;
const int IEN_COLUMN_INDEX=13;
const int LATITUDE_COLUMN_INDEX = 16;
const int LONGITUDE_COLUMN_INDEX = 17;
  

void
importData(const std::string &filename,
	   std::vector<PersonInfo> &persons){
  persons.clear();
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
    std::string &firstName = split.at(FIRST_NAME_COLUMN_INDEX);
    std::string &lastName = split.at(LAST_NAME_COLUMN_INDEX);
    std::string &email = split.at(EMAIL_COLUMN_INDEX);
    std::string &ienCode = split.at(IEN_COLUMN_INDEX);
    std::string &lat_str = split.at(LATITUDE_COLUMN_INDEX);
    std::replace(lat_str.begin(), lat_str.end(), ',','.');
    std::string &lon_str = split.at(LONGITUDE_COLUMN_INDEX);
    std::replace(lon_str.begin(), lon_str.end(), ',','.');
    double lat_degree = std::stod(lat_str);
    double lon_degree = std::stod(lon_str);
    persons.push_back(PersonInfo(firstName, lastName, email, ienCode, lat_degree, lon_degree));

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
exportToGeoJSON(const std::string &outfilename, const std::vector<PersonInfo> &persons) {
  std::ofstream outfile(outfilename);
  outfile << "var geoData = {" << std::endl
	  << "\"type\": \"FeatureCollection\"," << std::endl
	  << "\"features\": [" << std::endl;
  bool first = true;
  std::string buffer;
  for (const PersonInfo&p : persons) {
    if (first) {
      first = false;
    } else {
      outfile << "," << std::endl;
    }
    outfile << p;
  }
  outfile << "]};";
  outfile.close();
}


void
computeFlyweightDistances(const std::vector<PersonInfo>&persons,
		 std::vector<std::vector<double>> &distances_meter) {
  distances_meter = std::vector<std::vector<double>>(persons.size(),
						     std::vector<double>(persons.size(),std::numeric_limits<double>::max()));
  for (int i = 1; i < persons.size(); ++i) {
    for (int j = 0; j < i; ++j) {
      // assume data is lat/lon coordinates, compute distances in m between the points
      double d = persons.at(j).flyweightDistance(persons.at(i));
      distances_meter[i][j] = distances_meter[j][i] = d;
    }
  }
}

void
computeRoutingDistance(const std::vector<PersonInfo>&persons,
		       std::vector<std::vector<double>>&distances){
  distances = std::vector<std::vector<double>>(persons.size(),
					       std::vector<double>(persons.size(), std::numeric_limits<double>::max()));
  for (int i = 1;i < persons.size(); ++i) {
    for (int j = 0; j < i; ++j) {
      double d = persons.at(j).routingDistance(persons.at(i));
      distances[i][j] = distances[j][i] = d;
    }
  }
}

void
computeIenDistance(const std::vector<PersonInfo>&persons,
		   std::vector<std::vector<double>>&distances){
  distances = std::vector<std::vector<double>>(persons.size(),
					       std::vector<double>(persons.size(), std::numeric_limits<double>::max()));
  for (int i = 1;i < persons.size(); ++i) {
    for (int j = 0; j < i; ++j) {
      double d = persons.at(j).ienDistance(persons.at(i));
      distances[i][j] = distances[j][i] = d;
    }
  }
}					       

void
exportDistanceMatrix(const std::vector<std::vector<double>> &distances,
		     const std::string &outfileName,
		     const std::string &distanceName) {
  std::ofstream outfile(outfileName);
  outfile << "var " << distanceName << "= {" << std::endl
	  << "\"numElem\":" << distances.size() << std::endl
	  << "\"distances\":[";
 
  for (int i = 0;i < distances.size();++i) {
    if (i == 0) {
      outfile << "[";
    } else {
      outfile << ",[";
    }
    for(int j = 0;j < distances.size(); ++j) {
      if (j > 0) {
	outfile << ",";
      }
      outfile << distances[i][j];
    }
    outfile << "]";
  }
  outfile << "};";
  outfile.close();
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
    std::cout << "Usage: " << argv[0] << " <data_file.csv>" << std::endl;
    return EXIT_SUCCESS;
  }
  try {
    const std::string dataFile = argv[1];
    std::vector<PersonInfo> persons;
    importData(dataFile, persons);
    int numElements = persons.size();
    
    // const std::string jsonDataFile = std::regex_replace(dataFile, std::regex("\\.csv"), ".js");
    // exportDataToJson(dataFile, jsonDataFile);

    const std::string jsonGeoData = "geodata.js";
    exportToGeoJSON(jsonGeoData, persons);

        
    std::vector<std::vector<double>> distances; // unoptimized n² distance matrix
    computeFlyweightDistances(persons, distances);

    exportDistanceMatrix(distances, "flyweight.js", "flyweight");
    
    
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
