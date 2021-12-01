#include "PersonInfo.hpp"
#include "GeoUtils.hpp"

PersonInfo::PersonInfo(const string &firstName,
		       const string &lastName,
		       const string &email,
		       const string &ienCode,
		       double lat,
		       double lon):
  m_firstName(firstName),
  m_lastName(lastName),
  m_email(email),
  m_ienCode(ienCode),
  m_latitude(lat),
  m_longitude(lon)
{}


double
PersonInfo::flyweightDistance(const PersonInfo &other){
  return ::flyweightDistance(other.m_latitude,
			   other.m_longitude,
			   m_latitude,
			   m_longitude);
}

double
PersonInfo::routingDistance(const PersonInfo &other){
  return 0.0;
}

double
PersonInfo::ienDistance(const PersonInfo&other){
  if (other.m_ienCode == m_ienCode) {
    return 0.0;
  } else {
    return 1.0;
  }
}
