#include <iostream>
#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

#include "sql_db.h"


/***
 * Get information from database function.
 * ARGUMENTS: None;
 * RETURNS: None.
 ***/
void sql_db::get_info_from_db( void )
{
} /* End of 'sql_db::get_info_from_db' function */


/***
 * Get taheometer ID from taheometer name function.
 * ARGUMENTS:
 *    - taheometer name:
 *        std::string tah_name;
 * RETURNS:
 *    (int) taheometer id.
 ***/
int sql_db::get_tah_id( std::string tah_name )
{
  sql::mysql::MySQL_Driver *driver = nullptr;
  sql::Connection *connection = nullptr;
  int recieved_tah_id = -1;

  try
  {
    /*  Connect to sql */
    driver = sql::mysql::get_driver_instance();
    connection = driver->connect(HOST, USER, PASSWORD);

    /* Connect to schema */
    connection->setSchema(SCHEMA);
    std::cout << "Connection to the database " << SCHEMA << " is successful\n";

    /* Set russian language */
    sql::Statement *stmt = connection->createStatement();
    stmt->execute("SET NAMES utf8mb4");
    delete stmt;

    /* Set query to get information */
    std::string query = "SELECT ID FROM taheometer WHERE tah_name = ?;"; 
    sql::PreparedStatement *pstmt = connection->prepareStatement(query);
    std::string correct_tah_name = cp1251_to_utf8(tah_name);
    
    pstmt->setString(1, correct_tah_name);

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

  if (connection)
    delete connection;

  return recieved_tah_id;
} /* End of 'sql_db::get_tah_id' function */


/***
 * Convertation text from cp1251 to utf8 function.
 * ARGUMENTS:
 *    - string in cp1251 charset:
 *        std::string &str_cp1251;
 * RETURNS:
 *    (std::string) string in utf8 charset.
 ***/
std::string sql_db::cp1251_to_utf8( const std::string& str_cp1251 )
{
  int size_wide = MultiByteToWideChar(CP_ACP, 0, str_cp1251.c_str(), -1, NULL, 0);
  wchar_t* wide_str = new wchar_t[size_wide];
  MultiByteToWideChar(CP_ACP, 0, str_cp1251.c_str(), -1, wide_str, size_wide);

  int size_utf8 = WideCharToMultiByte(CP_UTF8, 0, wide_str, -1, NULL, 0, NULL, NULL);
  char* utf8_str = new char[size_utf8];
  WideCharToMultiByte(CP_UTF8, 0, wide_str, -1, utf8_str, size_utf8, NULL, NULL);

  std::string result(utf8_str);
  
  delete[] wide_str;
  delete[] utf8_str;
  return result;
} /* End of 'sql_db::cp1251_to_utf8' function */