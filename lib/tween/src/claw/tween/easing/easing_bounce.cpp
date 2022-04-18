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
 * \file claw/code/tween/easing/easing_bounce.cpp
 * \brief Implementation of the claw::tween::easing_bounce_func function.
 * \author Julien Jorge
 */
#include <claw/tween/easing/easing_bounce.hpp>

/**
 * \brief The default function.
 * \param t The ratio of the date, in (0, 1), representing the total
 *        progression of the tweener.
 */
double claw::tween::easing_bounce_func(double t)
{
  const double v = 1 - t;
  double c;
  double d;

  if(v < (1 / 2.75))
    {
      c = v;
      d = 0;
    }
  else if(v < (2 / 2.75))
    {
      c = v - 1.5 / 2.75;
      d = 0.75;
    }
  else if(v < (2.5 / 2.75))
    {
      c = v - 2.25 / 2.75;
      d = 0.9375;
    }
  else
    {
      c = v - 2.625 / 2.75;
      d = 0.984375;
    }

  return 1 - (7.5625 * c * c + d);
}
