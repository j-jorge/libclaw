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
 * \file box_2d.hpp
 * \brief A rectangle represented by two points in a 2D space.
 * \author Julien Jorge
 */
#ifndef __CLAW_BOX_2D_HPP__
#define __CLAW_BOX_2D_HPP__

#include <claw/coordinate_2d.hpp>

namespace claw
{
  namespace math
  {
    template<class T> class rectangle;

    /**
     * \brief A rectangle represented by two points in a 2D space.
     * \author Julien Jorge
     */
    template <class T>
    class box_2d
    {
    public:
      /** \brief The type of the values we store. */
      typedef T value_type;

      /** \brief The type of the coordinates of the points representing the
          corners. */
      typedef coordinate_2d<value_type> point_type;

      /** \brief The type of the current class. */
      typedef box_2d<value_type> self_type;

    public:
      box_2d();
      box_2d( const self_type& that );
      box_2d( const rectangle<value_type>& that );
      box_2d( const point_type& p1, const point_type& p2 );
      box_2d( const value_type& x1, const value_type& y1,
              const value_type& x2, const value_type& y2 );

      void set( const value_type& x1, const value_type& y1,
                const value_type& x2, const value_type& y2 );

      template<typename U>
      box_2d<U> cast_value_type_to() const;

      value_type area() const;
      bool includes( const coordinate_2d<value_type>& p ) const;
      bool includes( const self_type& r ) const;
      bool intersects( const self_type& r ) const;
      self_type intersection( const self_type& r ) const;
      self_type join( const self_type& r ) const;
      bool empty() const;

      value_type top() const;
      value_type bottom() const;
      value_type left() const;
      value_type right() const;
      point_type top_left() const;
      point_type top_right() const;
      point_type bottom_left() const;
      point_type bottom_right() const;

      void top( const value_type& p );
      void bottom( const value_type& p );
      void left( const value_type& p );
      void right( const value_type& p );
      void top_left( const coordinate_2d<value_type>& p );
      void top_right( const coordinate_2d<value_type>& p );
      void bottom_left( const coordinate_2d<value_type>& p );
      void bottom_right( const coordinate_2d<value_type>& p );

      void shift_x( const value_type& d );
      void shift_y( const value_type& d );

      value_type width() const;
      value_type height() const;

      coordinate_2d<value_type> size() const;

      bool operator==(const self_type& vect) const;
      bool operator!=(const self_type& vect) const;
      self_type operator+(const point_type& vect) const;
      self_type operator-(const point_type& vect) const;
      self_type& operator+=(const point_type& vect);
      self_type& operator-=(const point_type& vect);

    private:
      void x_intersection( const self_type& r, self_type& result ) const;
      void y_intersection( const self_type& r, self_type& result ) const;

    public:
      /** \brief The first of the two points, representing one corner. */
      point_type first_point;

      /** \brief The second of the two points, representing an other corner. */
      point_type second_point;

    }; // class box_2d
  } // namespace math
} // namespace claw

#include <claw/impl/box_2d.tpp>

#endif // __CLAW_BOX_2D_HPP__
