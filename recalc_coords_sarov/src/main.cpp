#include <iostream>
#include <string>
#include <windows.h>

#include "sql/sql_db.h"
#include "window/window_ui.h"


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

  std::cout << "\nДанные успешно получены:\n";
  std::cout << "Исходный пункт: " << tah_name << "\n";
  std::cout << "Дата 1: " << d1 << "\n";
  std::cout << "Дата 2: " << d2 << "\n";
  
  std::cin.get();

  return 0;
}

