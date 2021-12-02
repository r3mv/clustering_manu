#ifndef PERSON_INFO_HPP
#define PERSON_INFO_HPP

#include <iostream>
#include <string>

using std::string;

class PersonInfo {

public:
  PersonInfo(const string &firstName,
	     const string &lastName,
	     const string &email,
	     const string &ienCode,
	     double latitude_degree,
	     double longitude_degree);
  virtual ~PersonInfo()
  {}

  double
  flyweightDistance(const PersonInfo &other) const;

  double
  routingDistance(const PersonInfo &other) const;

  double
  ienDistance(const PersonInfo&other) const;

  /**
   * Print PersonInfo as  GeoJSON
   */
  friend std::ostream&
  operator<<(std::ostream& out, const PersonInfo& pi);


private:

  string m_firstName;
  string m_lastName;
  string m_email;
  string m_ienCode;
  double m_latitude;
  double m_longitude;
  
};
#endif
