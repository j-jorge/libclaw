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
 * \file claw/tween/easing/impl/symmetric_easing.tpp
 * \brief Implementation of the claw::tween::symmetric_easing class.
 * \author Julien Jorge
 */

/**
 * \brief Apply the easing at the beginning of the tweener.
 * \param t The ratio of the date, in (0, 1), representing the total
 * progression of the tweener.
 */
template <double (*Function)(double)>
double claw::tween::symmetric_easing<Function>::ease_in(double t)
{
  return Function(t);
}

/**
 * \brief Apply the easing at the end of the tweener.
 * \param t The ratio of the date, in (0, 1), representing the total
 * progression of the tweener.
 */
template <double (*Function)(double)>
double claw::tween::symmetric_easing<Function>::ease_out(double t)
{
  return 1 - ease_in(1 - t);
}

/**
 * \brief Apply the easing at the beginning and the end of the tweener.
 * \param t The ratio of the date, in (0, 1), representing the total
 * progression of the tweener.
 */
template <double (*Function)(double)>
double claw::tween::symmetric_easing<Function>::ease_in_out(double t)
{
  if(t < 0.5)
    return ease_in(2 * t) / 2;
  else
    return 0.5 + ease_out(2 * t - 1) / 2;
}
