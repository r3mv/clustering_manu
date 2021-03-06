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
PersonInfo::flyweightDistance(const PersonInfo &other) const{
  return ::flyweightDistance(other.m_latitude,
			   other.m_longitude,
			   m_latitude,
			   m_longitude);
}

double
PersonInfo::routingDistance(const PersonInfo &other) const {
  return 0.0;
}

double
PersonInfo::ienDistance(const PersonInfo&other) const{
  if (other.m_ienCode == m_ienCode) {
    return 0.0;
  } else {
    return 1.0;
  }
}

std::ostream&
operator<<(std::ostream &out, const PersonInfo& pi) {
  out << "{ " << std::endl
      << "\"type\": \"Feature\"," << std::endl
      << "\"geometry\": {" << std::endl
      << "    \"type\": \"Point\"," << std::endl
      << "    \"coordinates\": [" << pi.m_longitude << "," << pi.m_latitude << "]" << std::endl
      << "}," << std::endl
      << "\"properties\": {" << std::endl
      << "\"firstName\" : \"" << pi.m_firstName << "\"," << std::endl
      << "\"lastName\" : \"" << pi.m_lastName << "\"," << std::endl
      << "\"email\" : \"" << pi.m_email << "\"" << std::endl
      << "\"ienCode\": \"" << pi.m_ienCode << "\"" << std::endl
      << "}" << std::endl
      << "}" << std::endl;
  return out;
}

