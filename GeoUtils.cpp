#include "GeoUtils.hpp"
#include <cmath>

double
degreeToRadian(double val_degree) {
  return val_degree * M_PI / 180;
}


double
flyweightDistance(double lat1_degree, double lon1_degree,
		 double lat2_degree, double lon2_degree)
{
  double earth_radius_meters = 6371e3;
  double lat1_radian = degreeToRadian(lat1_degree);
  double lat2_radian = degreeToRadian(lat2_degree);
  double delta_lat_radian = degreeToRadian(lat2_degree-lat1_degree);
  double delta_lon_radian = degreeToRadian(lon2_degree-lon1_degree);

  double a = sin(delta_lat_radian/2)*sin(delta_lat_radian/2)+
    cos(lat1_radian)*cos(lat2_radian)*
    sin(delta_lon_radian/2)*sin(delta_lon_radian/2);
  double c = 2*atan2(sqrt(a), sqrt(1-a));
  return earth_radius_meters * c; // in metres
}
