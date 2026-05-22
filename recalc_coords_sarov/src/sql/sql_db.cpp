#include <iostream>
#include <string>
#include <fstream>


#include "sql_db.h"


/* Default Constructor */
sql_db::sql_db( void )
{
  try
  {
    driver = sql::mysql::get_driver_instance();
    connection = driver->connect(HOST, USER, PASSWORD);

    /* Connect to schema */
    connection->setSchema(SCHEMA);
    std::cout << "Connection to the database " << SCHEMA << " is successful\n";

    /* Set russian language */
    sql::Statement *stmt = connection->createStatement();
    stmt->execute("SET NAMES utf8mb4");
    delete stmt;
  }
  catch (sql::SQLException &e)
  {
    std::cerr << "SQL Error: " << e.what() << "\n";
    std::cerr << "MySQL Code: " << e.getErrorCode() << "\n";
    std::cerr << "SQLState: " << e.getSQLState() << "\n";
  }
} /* End of 'sql_db::sql_db' function */

/* Destructor */
sql_db::~sql_db( void )
{
  if (connection)
    delete connection;
} /* End of 'sql_db::~sql_db' function */


/***
 * Get taheometer ID from taheometer name function.
 * ARGUMENTS:
 *    - taheometer name:
 *        std::string &tah_name;
 * RETURNS:
 *    (int) taheometer id.
 ***/
int sql_db::get_tah_id( std::string &tah_name )
{
  int recieved_tah_id = -1;

  try
  {
    /* Set query to  get information */
    std::string query = "SELECT ID FROM taheometer WHERE tah_name = ?;"; 
    sql::PreparedStatement *pstmt = connection->prepareStatement(query);
    
    pstmt->setString(1, tah_name);

    sql::ResultSet *res = pstmt->executeQuery();

    if (res->next())
    {
      recieved_tah_id = res->getInt("ID");
      std::cout << "Find taheometer ID: " << recieved_tah_id << "\n";
    }

    delete res;
    delete pstmt; 

  }
  catch (sql::SQLException &e)
  {
    std::cerr << "SQL Error: " << e.what() << "\n";
    std::cerr << "MySQL Code: " << e.getErrorCode() << "\n";
    std::cerr << "SQLState: " << e.getSQLState() << "\n";
  }

  return recieved_tah_id;
} /* End of 'sql_db::get_tah_id' function */


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
void sql_db::get_points_from_db( std::map<std::string, std::map<int, point_polar_coords [2]>> &saved_points, std::string &tah_name, std::string &date )
{
  int tah_id = get_tah_id(tah_name);
  
  try
  {
    std::string query = 
      "SELECT "
	    "points.name, "
      "points.id, "
      "measurement.cycle_id, "
      "measurement.circle, "
      "measurement.status, "
      "measurement.azimuth, "
      "measurement.inclination, "
      "measurement.distance "
      "FROM measurement "
      "JOIN cycle ON measurement.cycle_id = cycle.ID "
      "JOIN points ON measurement.point_id = points.ID "
      "WHERE "
      "    cycle.begin > ? AND "
      "    cycle.begin < ? AND "
      "    measurement.status = 1 AND "
      "    points.type = 0 AND "
      "    measurement.tah_id = ?;";

    sql::PreparedStatement *pstmt = connection->prepareStatement(query);
    std::string 
      correct_start_date_time = date + " 00:00:00 ",
      correct_end_date_time = date + " 23:59:59 ";
    
    pstmt->setString(1, correct_start_date_time);
    pstmt->setString(2, correct_end_date_time);
    pstmt->setInt(3, tah_id);

    sql::ResultSet *res = pstmt->executeQuery();

    while (res->next())
    {
      std::string point_name = res->getString("name");
      int circle = res->getInt("circle");
      int cycle_id = res->getInt("cycle_id");
      
      // saved_points[point_name][cycle_id][circle].azimuth = 
      saved_points[point_name][cycle_id][circle].azimuth = res->getDouble("azimuth");
      saved_points[point_name][cycle_id][circle].inclination = res->getDouble("inclination");
      saved_points[point_name][cycle_id][circle].distance = res->getDouble("distance");
    }

    delete res;
    delete pstmt; 
  }
  catch (sql::SQLException &e)
  {
    std::cerr << "SQL Error: " << e.what() << "\n";
    std::cerr << "MySQL Code: " << e.getErrorCode() << "\n";
    std::cerr << "SQLState: " << e.getSQLState() << "\n";
  }

} /* End of 'sql_db::get_points_from_db' function */

