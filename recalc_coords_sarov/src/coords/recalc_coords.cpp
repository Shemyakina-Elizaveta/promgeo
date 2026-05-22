#include "recalc_coords.h"



/***
 * Convert (std::string) to (std::wstring) function.
 * ARGUMENTS:
 *    - input string:
 *        const std::string& str_utf8;
 * RETURNS:
 *    (std::wstring) result string.
 ***/
std::wstring utf8_to_wstring( const std::string &str_utf8 )
{
  if (str_utf8.empty())
    return L"";

  int size_needed = MultiByteToWideChar(CP_UTF8, 0, str_utf8.c_str(), -1, NULL, 0);
  std::wstring wstr(size_needed - 1, 0);
  
  MultiByteToWideChar(CP_UTF8, 0, str_utf8.c_str(), -1, &wstr[0], size_needed - 1);
  return wstr;
} /* End of 'utf8_to_wstring' function */


/***
 * Converting coordinates of one point from spherical to rectangular.
 * ARGUMENTS:
 *    - Spherical coordinates of a point:
 *        point_polar_coords point_sph[2];
 * RETURNS:
 *    (point) rectangular coordinates of a point.
 ***/
point spher_to_rect( point_polar_coords point_sph[2] )
{
  point res;
  double azim, inc, place_zero, dist;

  if (point_sph[0].azimuth < point_sph[1].azimuth)
    azim = (point_sph[0].azimuth + point_sph[1].azimuth - std::numbers::pi) / 2;
  else
    azim = (point_sph[0].azimuth + point_sph[1].azimuth + std::numbers::pi) / 2;

  place_zero = (point_sph[0].inclination + point_sph[1].inclination - 2 * std::numbers::pi) / 2;
  inc = point_sph[0].inclination - place_zero;

  dist = (point_sph[0].distance + point_sph[1].distance) / 2;

  res.x = dist * std::sin(azim) * std::sin(inc);
  res.y = dist * std::cos(azim) * std::sin(inc);
  res.z = dist * std::cos(inc);

  return res;
} /* End of 'spher_to_rect' function */

/***
 * Converting coordinates of an array of points from spherical to rectangular.
 * ARGUMENTS:
 *    - taheometer name:
 *        std::string &tah_name;
 *    - date of measurements:
 *        std::string &date;
 *    - map of final results points:
 *        std::map<std::string, point> &res_points;
 *    - map of the received points:
 *        std::map<std::string, std::map<int, point_polar_coords [2]>> &recieved_points;
 * RETURNS: None.
 ***/
void recalculate_all_points_coords( std::string &tah_name, std::string &date,
                                    std::map<std::string, point> &res_points,
                                    std::map<std::string, std::map<int, point_polar_coords [2]>> &recieved_points )
{
  OutputDebugStringA("\n\n\nфункция вызвана\n\n\n");

  std::ofstream out;
  std::wstring path = 
    L"D:/repositories/promgeo/recalc_coords_sarov/results/" + 
    utf8_to_wstring(tah_name) + L"/" + utf8_to_wstring(date) + L"/";
  std::wstring path_beg_name = path + utf8_to_wstring(tah_name + "_" + date + "_cycle-");
  std::wstring full_path_name;


  for (auto &pnt : recieved_points)
  {
    for (auto &cycle : pnt.second)
    {
      full_path_name = path_beg_name + std::to_wstring(cycle.first) + L".txt";
      OutputDebugString(full_path_name.c_str());
      
      if (!std::filesystem::exists(path))
      {
        std::filesystem::create_directories(path);
        OutputDebugStringA("\n\ncreate directories\n\n");
      }

      out.open(full_path_name, std::ios::app);
      out << std::fixed << std::setprecision(15);
      
      if (!out.is_open())
        return;

      point res = spher_to_rect(cycle.second);

      out << pnt.first << "    " << res.x << "    " << res.y << "    " << res.z << "\n";
      out.close();
    }
  }
} /* End of 'recalculate_all_points_coords' function */


