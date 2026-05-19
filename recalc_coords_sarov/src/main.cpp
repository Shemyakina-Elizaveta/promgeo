#include <iostream>
#include <string>
#include <windows.h>

#include "sql/sql_db.h"
#include "window/window.h"


/***
 * Main function.
 * ARGUMENTS: None;
 * RETURNS: 
 *    (int) Program completion code.
 ***/
int main( void )
{
  /* Settings to work with russian language */
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
  setlocale(LC_ALL, "Russian");


  sql_db Database;

  std::cout << Database.get_tah_id("Т2.7") << "\n";
  std::cout << "Ok\n";

  return 0;
} /* End of 'main' function */