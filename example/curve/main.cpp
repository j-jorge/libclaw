/*
  CLAW - a C++ Library Absolutely Wonderful

  CLAW is a free library without any particular aim but being useful to
  anyone.

  Copyright (C) 2005-2011 Julien Jorge

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  contact: julien.jorge@stuff-o-matic.com
*/
/**
 * \file
 * \brief Example program for the Bézier curves of Claw.
 * \author Julien Jorge
 */

#include <claw/curve.hpp>

#include <claw/graphic/png.hpp>
#include <claw/string_algorithm.hpp>

#include <fstream>
#include <iostream>

typedef claw::math::coordinate_2d<double> coordinate_type;
typedef claw::math::curve<coordinate_type> curve_type;

/**
 * \brief Read the points of a curve from a stream.
 *
 * The stream must describe the points in the following format:
 * x y in_x in_y out_x out_y
 *
 * Where (x, y) is the coordinate of the control point, (in_x, in_y) is the
 * coordinate of the direction of the curve when entering the point and
 * (out_x, out_y) is the direction of the curve when leaving the point.
 *
 * If the in and out points are not set, they are set to (x, y) by default.
 *
 * \param c The curve to which the points are added.
 * \param is The stream from which the curve is read.
 */
claw::math::coordinate_2d<std::size_t> read_curve(curve_type& c,
                                                  std::istream& is)
{
  claw::math::coordinate_2d<std::size_t> max(0, 0);

  std::string line;

  while(claw::text::getline(is, line))
    {
      std::istringstream iss(line);
      coordinate_type position;

      if(iss >> position.x >> position.y)
        {
          max.x = std::max((std::size_t)position.x, max.x);
          max.y = std::max((std::size_t)position.y, max.y);

          coordinate_type in, out;

          if(iss >> in.x >> in.y >> out.x >> out.y)
            c.push_back(curve_type::control_point(position, in, out));
          else
            c.push_back(curve_type::control_point(position));
        }
    }

  return max;
}

/**
 * \brief Draw the curve with a given precision.
 * \param result The image in which the curve is drawn.
 * \param c The curve to draw.
 * \param step The step between two points of the curve.
 */
void draw_curve(claw::graphic::image& result, const curve_type& c, double step)
{
  for(curve_type::const_iterator it = c.begin(); it != c.end(); ++it)
    {
      const curve_type::section s(c.get_section(it));

      for(double t(0); t <= 1; t += step)
        {
          const claw::math::coordinate_2d<int> p(s.get_point_at(t));

          if(((0 <= p.x) && (p.x < (int)result.width()))
             && ((0 <= p.y) && (p.y < (int)result.height())))
            result[p.y][p.x] = claw::graphic::black_pixel;
        }
    }
}

/**
 * \brief Draw a curve in an image.
 */
int main(int argc, char* argv[])
{
  if(argc != 2)
    {
      std::cerr << "usage:\n"
                << '\t' << argv[0] << " step\n\n"
                << "The program reads the coordinates of the curve on stdin.\n"
                << "Each line represents a point in the following format:\n"
                << "\tx y input_x input_y output_x output_y\n"
                << "The curve is drawn in an image saved as output.png with a"
                << " detail of step.\n\n"
                << "example:\n"
                << "\tcat example | " << argv[0] << "0.001\n\n"
                << "where the content of example is:\n"
                << "0 0 0 0 75 25\n"
                << "50 75 75 75 25 75\n"
                << "100 0 25 25 100 0";
      std::cerr << std::endl;
      return 1;
    }

  double step(1);
  std::istringstream iss(argv[1]);

  iss >> step;

  curve_type c;

  claw::math::coordinate_2d<std::size_t> bounds = read_curve(c, std::cin);
  claw::graphic::png result(bounds.x, bounds.y);

  std::fill(result.begin(), result.end(), claw::graphic::white_pixel);

  draw_curve(result, c, step);

  std::ofstream f("output.png");
  result.save(f);

  return 0;
}
