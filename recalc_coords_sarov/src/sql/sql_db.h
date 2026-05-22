#ifndef __sql_db_h__
#define __sql_db_h__

#include <string>
#include <map>
#include <Windows.h>


/* Connect sql */
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>


/* Point polar coordinates */
struct point_polar_coords
{
  double azimuth;
  double inclination;
  double distance;

  /* Default constructor */
  point_polar_coords( void ) : azimuth(0), inclination(0), distance(0) {}

  /* Copy constructor */
  point_polar_coords( const point_polar_coords &np )
  {
    azimuth = np.azimuth;
    inclination = np.inclination;
    distance = np.distance;
  }

  point_polar_coords & operator=( const point_polar_coords &np )
  {
    azimuth = np.azimuth;
    inclination = np.inclination;
    distance = np.distance;

    return *this;
  }
}; /* End of 'point_polar_coords' struct */

#if 0
/* All information about point */
struct point_polar_coords [2]
{
  // int cycle_id; // по идее не нужно
  point_polar_coords coords[2]; // 0 - left, 1 - right


  /* Default constructor */
  point_polar_coords [2]( void ) : /* cycle_id(-1), */ coords() {}

  /* Copy constructor */
  point_polar_coords [2]( const point_polar_coords [2] &np )
  {
    // cycle_id = np.cycle_id;
    coords[0] = np.coords[0];
    coords[1] = np.coords[1];
  }

  point_polar_coords [2] & operator=( const point_polar_coords [2] &np )
  {
    // cycle_id = np.cycle_id;
    coords[0] = np.coords[0];
    coords[1] = np.coords[1];

    return *this;
  }
}; /* End of 'point_polar_coords [2]' structure */
#endif


/* Working with a database via sql class */ 
class sql_db
{
private:

  /* Constants for connecting to the server */
  const std::string HOST = "tcp://127.0.0.1:3306";
  const std::string USER = "root";
  const std::string PASSWORD = "PromGeo042004.";
  const std::string SCHEMA = "test_import_db";

  sql::mysql::MySQL_Driver *driver = nullptr;
  sql::Connection *connection = nullptr;

  
public:


  /* Default Constructor */
  sql_db( void );

  /* Destructor */
  ~sql_db( void );

  /***
   * Get taheometer ID from taheometer name function.
   * ARGUMENTS:
   *    - taheometer name:
   *        std::string &tah_name;
   * RETURNS:
   *    (int) taheometer_id.
   ***/
  int get_tah_id( std::string &tah_name );

  /***
   * Get all points information from database function.
   * ARGUMENTS:
   *    - map for saving points:
   *        std::map<std::string, std::map<int, point_polar_coords [2]>> &saved_points;
   *    - the name of taheometer from which the measurements are taken:
   *        std::string &tah_name;
   *    - date of measurement (YYYY-MM-DD):
   *        std::string &date;
   * RETURNS: None.
   ***/
  void get_points_from_db( std::map<std::string, std::map<int, point_polar_coords [2]>> &saved_points, std::string &tah_name, std::string &date );

  // <имя, <индекс цикла, информация>>

}; /* End of 'sql_db' class */


#endif /* __sql_db_h__ */