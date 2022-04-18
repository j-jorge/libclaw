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
 * \file coordinate_2d.hpp
 * \brief Coordinates in a two dimensional space.
 * \author Julien Jorge
 */
#ifndef __CLAW_COORDINATE_2D_HPP__
#define __CLAW_COORDINATE_2D_HPP__

namespace claw
{
  namespace math
  {
    /**
     * \brief Coordinates in a two dimensional space.
     * \author Julien Jorge
     */
    template <typename T>
    class coordinate_2d
    {
    public:
      /** \brief The type of the values we store. */
      typedef T value_type;

      /** \brief The type of the current class. */
      typedef coordinate_2d<value_type> self_type;

    public:
      coordinate_2d();

      template <typename U>
      coordinate_2d(const coordinate_2d<U>& that);

      coordinate_2d(const value_type& _x, const value_type& _y);

      template <typename U>
      coordinate_2d<U> cast_value_type_to() const;

      void set(const value_type& _x, const value_type& _y);
      value_type distance(const self_type& p) const;

      void rotate(const self_type& center, double angle);
      double slope_angle(const self_type& to) const;

      bool operator==(const self_type& vect) const;
      bool operator!=(const self_type& vect) const;
      self_type operator+(const self_type& vect) const;
      self_type operator-(const self_type& vect) const;
      self_type& operator+=(const self_type& vect);
      self_type& operator-=(const self_type& vect);

      self_type operator*(const value_type& v) const;
      self_type operator/(const value_type& v) const;
      self_type& operator*=(const value_type& v);
      self_type& operator/=(const value_type& v);

    public:
      /** \brief X-coordinate. */
      value_type x;

      /** \brief Y-coordinate. */
      value_type y;

    }; // class coordinate_2d

    template <typename T>
    coordinate_2d<T> operator-(const coordinate_2d<T>& that);

    template <typename T, typename U>
    coordinate_2d<T> operator*(U v, const coordinate_2d<T>& self);

  }
}

#include <claw/coordinate_2d.tpp>

#endif // __CLAW_COORDINATE_2D_HPP__
