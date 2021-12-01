#ifndef GEOUTILS_HPP
#define GEOUTILS_HPP


double
degreeToRadian(double val_degree);

/**
 * lat and lon are signed decimal degrees without compass direction
 * where negative indicates west/south (e.g. 40.7486, -73.9864)
 * @return distance in meters
 * @see http://www.movable-type.co.uk/scripts/latlong.html
 */
double
flyweightDistance(double lat1_degree, double lon1_degree,
		 double lat2_degree, double lon2_degree);

#endif
