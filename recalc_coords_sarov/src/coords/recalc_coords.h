#ifndef __recalc_coords_h__
#define __recalc_coords_h__

#include <map>
#include <cmath>
#include <numbers>
#include <filesystem>
#include <fstream>
#include <windows.h>
#include <string>
#include <iomanip>

#include "../sql/sql_db.h"

/* Point struct */
struct point
{
  double x, y, z;

  /* Default constructor */
  point( void ) : x(0), y(0), z(0) {}

  point( double nx, double ny, double nz ) : x(nx), y(ny), z(nz) {}
}; /* End of 'point' struct */


/***
 * Converting coordinates of one point from spherical to rectangular.
 * ARGUMENTS:
 *    - Spherical coordinates of a point:
 *        point_polar_coords point_sph[2];
 * RETURNS:
 *    (point) rectangular coordinates of a point.
 ***/
point spher_to_rect( point_polar_coords point_sph[2] );

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
                                    std::map<std::string, std::map<int, point_polar_coords [2]>> &recieved_points );
#endif /* __recalc_coords_h__ */
