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


/* Получили имя тахеометра, дату 1, дату 2 ->
 * Вытащили из бд id тахеометра ->
 * Сделали запрос на получение (имя точки, {id цикла, круг[азимут, инклинайшн, расстояние]}) ->
 * Записали в map [имя точки, {все остальное}] ->
 * Вывели в файл все
 */


/* Point polar coordinates */
struct point_polar_coords
{
  double azimuth = 0;
  double inclination = 0;
  double distance = 0;
}; /* End of 'point_polar_coords' struct */

/* All information about point */
struct point_info
{
  int cycle_id = -1;
  point_polar_coords coords[2]; // 0 - left, 1 - right
}; /* End of 'point_info' structure */


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
   *    - the name of taheometer from which the measurements are taken:
   *        std::string &tah_name;
   *    - date of measurement (YYYY-MM-DD):
   *        std::string &date;
   * RETURNS:
   *    (std::map<std::string, point_info>) map for saving points.
   ***/
  std::map<std::string, std::vector<point_info>> get_points_from_db( std::string &tah_name, std::string &date );

}; /* End of 'sql_db' class */


#endif /* __sql_db_h__ */