#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>

#include "sql/sql_db.h"
#include "window/window_ui.h"
#include "coords/recalc_coords.h"


/***
 * Entry point for Unicode applications in Visual Studio.
 * ARGUMENTS:
 *    - Handle to the current instance of the application:
 *        HINSTANCE hInst;
 *    - Handle to the previous instance of the application (always NULL):
 *        HINSTANCE hPrevInst;
 *    - The command line for the application, excluding the program name:
 *        LPWSTR lpCmdLine;
 *    - Controls how the window is to be shown:
 *        int nCmdShow;
 * RETURNS:
 *    (int) program completion code.
 ***/
int WINAPI wWinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow )
{
  SetConsoleOutputCP(CP_UTF8);
  setlocale(LC_ALL, "Russian");

  std::cout << "Запуск окна ввода данных...\n";

  /* Get information from window */
  window_ui app;

  app.Run(hInst, nCmdShow);

  if (!app.is_data_ready())
  {
    std::cout << "Ввод отменен пользователем.\n";
    return 0;
  }

  std::string tah_name = app.get_tah_name();
  std::string d1 = app.get_first_date();
  std::string d2 = app.get_second_date();

  /* Getting information from database */
  sql_db Database;
  std::map<std::string, std::map<int, point_polar_coords [2]>> points_info_first_date, points_info_second_date;
  
  Database.get_points_from_db(points_info_first_date, tah_name, d1);
  Database.get_points_from_db(points_info_second_date, tah_name, d1);
  
  /* Recalculate points */
  std::map<std::string, point> result_first_date, result_second_date;
  
  recalculate_all_points_coords(tah_name, d1, result_first_date, points_info_first_date);
  recalculate_all_points_coords(tah_name, d1, result_second_date, points_info_second_date);

  return 0;
}

