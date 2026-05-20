#include <iostream>
#include <locale>

#include "window_ui.h" 

#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")



/***
 * Convert (wchar_t *) to (std::string) function.
 * ARGUMENTS:
 *    - input string:
 *        const wchar_t *wstr;
 * RETURNS:
 *    (std::string) result string.
 ***/
std::string window_ui::WStringToUtf8( const wchar_t *wstr )
{
  if (!wstr)
    return "";
  
  int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
  std::string str(size, 0);
  
  WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &str[0], size, nullptr, nullptr);
  
  return str;
} /* End of 'window_ui::WStringToUtf8' function */


/***
 * Formatting from SYSTEMTIME to YYYY-MM-DD format function.
 * ARGUMENTS:
 *    - SYSTEMTIME string:
 *        const SYSTEMTIME &st;
 * RETURNS:
 *    (std::string) string in format YYYY-MM-DD.
 ***/
std::string window_ui::SystemTimeToString( const SYSTEMTIME &st )
{
  char buffer[11];

  snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay);
  return std::string(buffer);
} /* End of 'window_ui::SystemTimeToString' function */





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
LRESULT CALLBACK window_ui::WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  window_ui *ptr_this = nullptr;

  if (msg == WM_CREATE)
  {
    CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);

    ptr_this = reinterpret_cast<window_ui *>(pCreate->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ptr_this));
  }
  else
    ptr_this = reinterpret_cast<window_ui *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

  if (ptr_this)
    return ptr_this->HandleMessage(hwnd, msg, wParam, lParam);
  
  return DefWindowProc(hwnd, msg, wParam, lParam);
} /* End of 'window_ui::WndProc' function */

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
LRESULT window_ui::HandleMessage( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch (msg)
  {
    case WM_CREATE:
      CreateInterface(hwnd);
      return 0;

    case WM_SIZE:
      if (wParam != SIZE_MINIMIZED)
        ResizeControls(hwnd, LOWORD(lParam), HIWORD(lParam));
      return 0;

    case WM_GETMINMAXINFO:
    {
      MINMAXINFO *mmi = (MINMAXINFO *)lParam;
      
      mmi->ptMinTrackSize.x = 600;
      mmi->ptMinTrackSize.y = 380;
      return 0;
    }

    case WM_CTLCOLORSTATIC:
    {
      HDC hdc = (HDC)wParam;
      
      // Пример кастомизации цвета подсказки, если нужно
      if ((HWND)lParam == GetDlgItem(hwnd, 104))  // ID подсказки
      {
         SetTextColor(hdc, RGB(110, 110, 110));
         SetBkMode(hdc, TRANSPARENT);
         
         return (LRESULT)GetStockObject(NULL_BRUSH);
      }

      return (LRESULT)GetStockObject(NULL_BRUSH);
    }

    case WM_COMMAND:
    {
      if (LOWORD(wParam) == 1)  // Кнопка "Сохранить"
      {
        wchar_t buf[256];
        
        GetWindowTextW(hTahEdit, buf, 256);
        tah_name = WStringToUtf8(buf);

        if (tah_name.empty())
        {
          MessageBoxW(hwnd, L"Введите исходный пункт!", L"Ошибка", MB_ICONWARNING | MB_OK);
          SetFocus(hTahEdit);

          return 0;
        }

        SYSTEMTIME st1, st2;

        if (SendMessage(hDate1Picker, DTM_GETSYSTEMTIME, 0, (LPARAM)&st1) == GDT_VALID)
          first_date = SystemTimeToString(st1);
       
        if (SendMessage(hDate2Picker, DTM_GETSYSTEMTIME, 0, (LPARAM)&st2) == GDT_VALID)
          second_date = SystemTimeToString(st2);

        if (CompareFileTime((FILETIME*)&st1, (FILETIME*)&st2) > 0)
        {
          MessageBoxW(hwnd, L"Вторая дата не может быть раньше первой!", L"Ошибка", MB_ICONWARNING | MB_OK);
          return 0;
        }

        is_data_ready_flag = true;
        PostQuitMessage(0);
      }

      if (LOWORD(wParam) == 2) // Кнопка "Отмена"
          PostQuitMessage(0);

      return 0;
    }

    case WM_DESTROY:
    {
      // Очистка ресурсов шрифтов
      if (hFontTitle)
        DeleteObject(hFontTitle);

      if (hFontNormal)
        DeleteObject(hFontNormal);

      if (hFontHint)
        DeleteObject(hFontHint);

      PostQuitMessage(0);
      return 0;
    }
  }

  return DefWindowProc(hwnd, msg, wParam, lParam);
} /* End of 'window_ui::HandleMessage' function */

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
void window_ui::ResizeControls( HWND hwnd, int width, int height )
{
  int fieldLeft = PADDING + LABEL_WIDTH + 15;
  int fieldWidth = width - 2 * PADDING - LABEL_WIDTH - 15;
  
  MoveWindow(GetDlgItem(hwnd, 100), PADDING, PADDING, width - 2 * PADDING, 45, TRUE);
  
  MoveWindow(GetDlgItem(hwnd, 101), fieldLeft, 78, fieldWidth, FIELD_HEIGHT, TRUE); // LocEdit
  MoveWindow(GetDlgItem(hwnd, 102), fieldLeft, 128, fieldWidth, FIELD_HEIGHT, TRUE); // Date1
  MoveWindow(GetDlgItem(hwnd, 103), fieldLeft, 178, fieldWidth, FIELD_HEIGHT, TRUE); // Date2
  MoveWindow(GetDlgItem(hwnd, 104), fieldLeft, 225, fieldWidth, 20, TRUE); // Hint
  
  int totalBtnWidth = 2 * BTN_WIDTH + 20;
  int btnStartX = (width - totalBtnWidth) / 2;
  MoveWindow(GetDlgItem(hwnd, 1), btnStartX, 260, BTN_WIDTH, BTN_HEIGHT, TRUE); // Ok
  MoveWindow(GetDlgItem(hwnd, 2), btnStartX + BTN_WIDTH + 20, 260, BTN_WIDTH, BTN_HEIGHT, TRUE); // Cancel
} /* End of 'window_ui::ResizeControls' function */

/***
 * Font and control creation function (called once at WM_CREATE).
 * ARGUMENTS:
 *    - the unique identifier (handle) of the window:
 *        HWND hwnd;
 * RETURNS: None.
 ***/
void window_ui::CreateInterface( HWND hwnd )
{
  INITCOMMONCONTROLSEX icex;

  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC = ICC_DATE_CLASSES;
  InitCommonControlsEx(&icex);

  hFontTitle = CreateFontW(-28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
  hFontNormal = CreateFontW(-18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
  hFontHint = CreateFontW(-13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

  // Заголовок
  HWND hTitle = CreateWindowW(L"STATIC", L"Данные об измерениях", WS_CHILD | WS_VISIBLE | SS_CENTER, PADDING, PADDING, 300, 45, hwnd, (HMENU)100, 0, 0);
  SendMessage(hTitle, WM_SETFONT, (WPARAM)hFontTitle, TRUE);

  // Метки
  HWND hStaticLoc = CreateWindowW(L"STATIC", L"Исходный пункт:", WS_CHILD | WS_VISIBLE | SS_RIGHT, PADDING, 80, LABEL_WIDTH, 30, hwnd, 0, 0, 0);
  SendMessage(hStaticLoc, WM_SETFONT, (WPARAM)hFontNormal, TRUE);

  HWND hStaticDate1 = CreateWindowW(L"STATIC", L"Дата первого измерения:", WS_CHILD | WS_VISIBLE | SS_RIGHT, PADDING, 130, LABEL_WIDTH, 30, hwnd, 0, 0, 0);
  SendMessage(hStaticDate1, WM_SETFONT, (WPARAM)hFontNormal, TRUE);

  HWND hStaticDate2 = CreateWindowW(L"STATIC", L"Дата второго измерения:", WS_CHILD | WS_VISIBLE | SS_RIGHT, PADDING, 180, LABEL_WIDTH, 30, hwnd, 0, 0, 0);
  SendMessage(hStaticDate2, WM_SETFONT, (WPARAM)hFontNormal, TRUE);

  // Поля ввода (сохраняем хендлы в private поля класса!)
  hTahEdit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_TABSTOP, PADDING + LABEL_WIDTH + 15, 78, 250, FIELD_HEIGHT, hwnd, (HMENU)101, 0, 0);
  SendMessage(hTahEdit, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
  SendMessage(hTahEdit, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(10, 10));
  SetWindowTextW(hTahEdit, L"Т1.2");

  hDate1Picker = CreateWindowW(DATETIMEPICK_CLASSW, L"", WS_CHILD | WS_VISIBLE | DTS_SHORTDATEFORMAT | WS_TABSTOP, PADDING + LABEL_WIDTH + 15, 128, 250, FIELD_HEIGHT, hwnd, (HMENU)102, 0, 0);
  SendMessage(hDate1Picker, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
  SendMessage(hDate1Picker, DTM_SETFORMAT, 0, (LPARAM)L"yyyy-MM-dd");
  SYSTEMTIME st = {};
  st.wYear = 2021;
  st.wMonth = 10;
  st.wDay = 16;
  SendMessage(hDate1Picker, DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&st);


  hDate2Picker = CreateWindowW(DATETIMEPICK_CLASSW, L"", WS_CHILD | WS_VISIBLE | DTS_SHORTDATEFORMAT | WS_TABSTOP, PADDING + LABEL_WIDTH + 15, 178, 250, FIELD_HEIGHT, hwnd, (HMENU)103, 0, 0);
  SendMessage(hDate2Picker, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
  SendMessage(hDate2Picker, DTM_SETFORMAT, 0, (LPARAM)L"yyyy-MM-dd");
  SendMessage(hDate2Picker, DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&st);

  // Подсказка
  HWND hHint = CreateWindowW(L"STATIC", L"Формат даты: ГГГГ-ММ-ДД", WS_CHILD | WS_VISIBLE | SS_LEFT, PADDING + LABEL_WIDTH + 15, 225, 250, 20, hwnd, (HMENU)104, 0, 0);
  SendMessage(hHint, WM_SETFONT, (WPARAM)hFontHint, TRUE);

  // Кнопки
  HWND hOkBtn = CreateWindowW(L"BUTTON", L"Сохранить и продолжить", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP, 100, 260, BTN_WIDTH, BTN_HEIGHT, hwnd, (HMENU)1, 0, 0);
  SendMessage(hOkBtn, WM_SETFONT, (WPARAM)hFontNormal, TRUE);

  HWND hCancelBtn = CreateWindowW(L"BUTTON", L"Отмена", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 100 + BTN_WIDTH + 20, 260, BTN_WIDTH, BTN_HEIGHT, hwnd, (HMENU)2, 0, 0);
  SendMessage(hCancelBtn, WM_SETFONT, (WPARAM)hFontNormal, TRUE);

  // Начальная раскладка
  RECT rc;
  
  GetClientRect(hwnd, &rc);
  ResizeControls(hwnd, rc.right, rc.bottom);
} /* End of 'window_ui::CreateInterface' function */

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
int window_ui::Run( HINSTANCE hInst, int nCmdShow )
{
  WNDCLASSW wc = {};

  wc.lpfnWndProc = window_ui::WndProc;
  wc.hInstance = hInst;
  wc.lpszClassName = L"GeoInputClass";
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  RegisterClassW(&wc);

  hwnd = CreateWindowExW(0, L"GeoInputClass", L"Информация о приборе",
      WS_OVERLAPPEDWINDOW, 
      CW_USEDEFAULT, CW_USEDEFAULT, 650, 400, 
      NULL, NULL, hInst, this); 

  if (!hwnd)
    return 1;

  RECT rect;
  int sw = GetSystemMetrics(SM_CXSCREEN);
  int sh = GetSystemMetrics(SM_CYSCREEN);
  
  GetWindowRect(hwnd, &rect);
  SetWindowPos(hwnd, NULL, 
               (sw - (rect.right - rect.left)) / 2, 
               (sh - (rect.bottom - rect.top)) / 2, 
               0, 0, SWP_NOSIZE | SWP_NOZORDER);

  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  MSG msg;

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return static_cast<int>(msg.wParam);
} /* End of 'window_ui::Run' function */

