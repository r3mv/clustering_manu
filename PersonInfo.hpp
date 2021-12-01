#ifndef PERSON_INFO_HPP
#define PERSON_INFO_HPP

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
  flyweightDistance(const PersonInfo &other);

  double
  routingDistance(const PersonInfo &other);

  double
  ienDistance(const PersonInfo&other);


private:

  string m_firstName;
  string m_lastName;
  string m_email;
  string m_ienCode;
  double m_latitude;
  double m_longitude;
  
};
#endif
