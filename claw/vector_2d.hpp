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
 * \file vector_2d.hpp
 * \brief Two dimensional vector.
 * \author Julien Jorge
 */
#ifndef __CLAW_VECTOR_2D_HPP__
#define __CLAW_VECTOR_2D_HPP__

#include <claw/coordinate_2d.hpp>

namespace claw
{
  namespace math
  {
    /**
     * \brief Two dimensional vector.
     * \author Julien Jorge
     */
    template<class T>
    class vector_2d :
      public coordinate_2d<T>
    {
    public:
      /** \brief The type of the values we store. */
      typedef T value_type;

      /** \brief The type of the current class. */
      typedef vector_2d<value_type> self_type;

      /** \brief The type of the parent class. */
      typedef coordinate_2d<value_type> super;

    public:
      vector_2d();
      template<class U> vector_2d( const coordinate_2d<U>& that );
      template<class U> vector_2d
      ( const coordinate_2d<U>& p1, const coordinate_2d<U>& p2 );
      vector_2d(const value_type& _x, const value_type& _y);

      value_type length() const;
      void normalize();
      self_type get_orthonormal_clockwise() const;
      self_type get_orthonormal_anticlockwise() const;

      value_type dot_product(const self_type& vect) const;

    }; // class vector_2d
  } // namespace math
} // namespace claw

#include <claw/impl/vector_2d.tpp>

#endif // __CLAW_VECTOR_2D_HPP__
