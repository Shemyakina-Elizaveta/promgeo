#ifndef __sql_db_h__
#define __sql_db_h__

#include <string>
#include <Windows.h>


/* Math vector struct */
struct Point
{
public:
  double x, y;

  /* Constructor */
  Point( void ) : x(0), y(0) {}
  Point( double nx, double ny ) : x(nx), y(ny) {}
}; /* End of 'Point' struct */


/* Working with a database via sql class */ 
class sql_db
{
private:

  /* Constants for connecting to the server */
  const std::string HOST = "tcp://127.0.0.1:3306";
  const std::string USER = "root";
  const std::string PASSWORD = "PromGeo042004.";
  const std::string SCHEMA = "test_import_db";

  /***
   * Convertation text from cp1251 to utf8 function.
   * ARGUMENTS:
   *    - string in cp1251 charset:
   *        std::string &str_cp1251;
   * RETURNS:
   *    (std::string) string in utf8 charset.
   ***/
  std::string cp1251_to_utf8( const std::string& str_cp1251 );

  
public:

  /***
   * Get information from database function.
   * ARGUMENTS: None;
   * RETURNS: None.
   ***/
  void get_info_from_db( void );

  /***
   * Get taheometer ID from taheometer name function.
   * ARGUMENTS:
   *    - taheometer name:
   *        std::string tah_name;
   * RETURNS:
   *    (int) taheometer_id.
   ***/
  int get_tah_id( std::string tah_name );
}; /* End of 'sql_db' class */


#endif /* __sql_db_h__ */