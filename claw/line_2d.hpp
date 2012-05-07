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
 * \file line_2d.hpp
 * \brief A straight line in a two dimensional space.
 * \author Julien Jorge
 */
#ifndef __CLAW_LINE_2D_HPP__
#define __CLAW_LINE_2D_HPP__

#include <claw/coordinate_2d.hpp>
#include <claw/vector_2d.hpp>

namespace claw
{
  namespace math
  {
    /**
     * \brief A straight line in a two dimensional space.
     * \author Julien Jorge
     */
    template<class T>
    class line_2d
    {
    public:
      /** \brief The type of the values we store. */
      typedef T value_type;

      /** \brief The type of the current class. */
      typedef line_2d<value_type> self_type;

      /** \brief The type of the reference point. */
      typedef coordinate_2d<value_type> point_type;

      /** \brief The type of the direction vector. */
      typedef vector_2d<value_type> direction_type;

    public:
      line_2d();
      template<class U> line_2d( const line_2d<U>& that );
      line_2d( const point_type& _origin, const direction_type& _direction );
      line_2d( const value_type& ox, const value_type& oy,
               const value_type& dx, const value_type& dy );

      bool parallel( const self_type& that ) const;
      bool orthogonal( const self_type& that ) const;
      point_type intersection( const self_type& that ) const;

      value_type y_value( const value_type& x ) const;

    public:
      /** \brief A reference point on the line. */
      point_type origin;

      /** \brief Direction. */
      direction_type direction;

    }; // class point_2d
  } // namespace math
} // namespace claw

#include <claw/impl/line_2d.tpp>

#endif // __CLAW_LINE_2D_HPP__
