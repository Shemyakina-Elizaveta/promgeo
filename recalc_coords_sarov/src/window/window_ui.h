#ifndef __window_h__
#define __window_h__

#include <windows.h>
#include <string>


/* Input window class */
class window_ui
{

private:

  HWND hwnd = nullptr;

  /* Handles of controls (to access them from different methods) */
  HWND
    hTahEdit = nullptr,
    hDate1Picker = nullptr,
    hDate2Picker = nullptr;

  /* Fonts */
  HFONT
    hFontTitle = nullptr,
    hFontNormal = nullptr,
    hFontHint = nullptr;

  /* Data entered by the user */
  std::string tah_name, first_date, second_date;
  bool is_data_ready_flag = false;

  /* Layout constants */
  const int 
    PADDING = 20,
    LABEL_WIDTH = 220,
    FIELD_HEIGHT = 36,
    BTN_HEIGHT = 44,
    BTN_WIDTH = 250;

  /***
   * Convert (wchar_t *) to (std::string) function.
   * ARGUMENTS:
   *    - input string:
   *        const wchar_t *wstr;
   * RETURNS:
   *    (std::string) result string.
   ***/
  std::string WStringToUtf8( const wchar_t *wstr );

  /***
   * Formatting from SYSTEMTIME to YYYY-MM-DD format function.
   * ARGUMENTS:
   *    - SYSTEMTIME string:
   *        const SYSTEMTIME &st;
   * RETURNS:
   *    (std::string) string in format YYYY-MM-DD.
   ***/
  std::string SystemTimeToString( const SYSTEMTIME &st );


  /***
   * Static function-Windows message router.
   * ARGUMENTS:
   *    - window handle:
   *        HWND hwnd;
   *    - message:
   *        UINT msg;
   *    - additional message parameters:
   *        WPARAM wParam, LPARAM lParam;
   * RESULTS:
   *    (LRESULT) the return value is the result of the message processing.
   ***/
  static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

   /***
   * Non-static handler where messages are redirected to.
   * ARGUMENTS:
   *    - window handle:
   *        HWND hwnd;
   *    - message:
   *        UINT msg;
   *    - additional message parameters:
   *        WPARAM wParam, LPARAM lParam;
   * RESULTS:
   *    (LRESULT) the return value is the result of the message processing.
   ***/
  LRESULT HandleMessage( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

  /***
   * The function of recalculating positions and sizes when the window changes.
   * ARGUMENTS:
   *    - the unique identifier (handle) of the window:
   *        HWND hwnd;
   *    - window width:
   *        int width;
   *    - window height:
   *        int height;
   * RETURNS: None.
   ***/
  void ResizeControls( HWND hwnd, int width, int height );


  /***
   * Font and control creation function (called once at WM_CREATE).
   * ARGUMENTS:
   *    - the unique identifier (handle) of the window:
   *        HWND hwnd;
   * RETURNS: None.
   ***/
  void CreateInterface( HWND hwnd );




public:

  /***
   * Public method for launching a window from main().
   * ARGUMENTS:
   *    - Handle to the current instance of the application:
   *        HINSTANCE hInst;
   *    - Controls how the window is to be shown:
   *        int nCmdShow;
   * RETURNS:
   *    (int) program completion code.
   ***/
  int Run( HINSTANCE hInst, int nCmdShow );


  /***
   * Return taheometer name function.
   * ARGUMENTS: None;
   * RETURNS:
   *    (std::string) taheometer name.
   ***/
  std::string get_tah_name( void ) const
  {
    return tah_name;
  } /* End of 'get_tah_name' function */


  /***
   * Return first date function.
   * ARGUMENTS: None;
   * RETURNS:
   *    (std::string) first date.
   ***/
  std::string get_first_date( void ) const
  {
    return first_date;
  } /* End of 'get_first_date' function */


  /***
   * Return second date function.
   * ARGUMENTS: None;
   * RETURNS:
   *    (std::string) second date.
   ***/
  std::string get_second_date( void ) const
  {
    return second_date;
  } /* End of 'get_second_date' function */


  /***
   * Getting a flag indicating whether the data is ready.
   * ARGUMENTS: None;
   * RETURNS:
   *    (bool) flag.
   ***/
  bool is_data_ready( void ) const
  {
    return is_data_ready_flag;
  } /* End of 'is_data_ready' function */

  
  /***
   * Convert (std::string) to (wchar_t *) function.
   * ARGUMENTS:
   *    - input string:
   *        const std::string& str_utf8;
   * RETURNS:
   *    (std::wstring) result string.
   ***/
  std::wstring Utf8toWString( const std::string &str_utf8 );
}; /* End of 'window_ui' class */


#endif /* __window_h__ */