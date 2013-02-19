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
  contact: julien.jorge@gamned.org
*/
/**
 * \file claw/code/tween/easing/easing_sine.cpp
 * \brief Implementation of the claw::tween::easing_sine_func function.
 * \author Julien Jorge
 */
#include <claw/tween/easing/easing_sine.hpp>

#include <boost/math/constants/constants.hpp>
#include <cmath>

/**
 * \brief The default function.
 * \param t The ratio of the date, in (0, 1), representing the total
 *        progression of the tweener.
 */
double claw::tween::easing_sine_func( double t )
{
  const double pi( boost::math::constants::pi<double>() );
  return 1 - std::cos(t * pi / 2);
} // easing_sine_func()
