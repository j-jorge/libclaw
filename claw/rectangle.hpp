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
 * \file rectangle.hpp
 * \brief A class representing a rectangle by his x,y coordinates, width and
 *        height.
 * \author Julien Jorge
 */
#ifndef __CLAW_RECTANGLE_HPP__
#define __CLAW_RECTANGLE_HPP__

#include <claw/coordinate_2d.hpp>
#include <claw/box_2d.hpp>

namespace claw
{
  namespace math
  {
    /**
     * \brief A class representing a rectangle by his x,y coordinates, width and
     *        height.
     *
     * This class considers that the y-axis increases from the top to the bottom
     * (like a screen).
     *
     * \author Julien Jorge
     */
    template<class T>
    class rectangle
    {
    public:
      /** \brief The type of the values we store. */
      typedef T value_type;

      /** \brief The type of the current class. */
      typedef rectangle<value_type> self_type;

    public:
      rectangle();

      template<typename  U>
      rectangle( const rectangle<U>& that );

      template<typename  U>
      rectangle( const box_2d<U>& that );

      rectangle( const value_type& _x, const value_type& _y,
                 const value_type& _width, const value_type& _height);

      template<typename U>
      rectangle( const coordinate_2d<U>& pos, const value_type& _width,
                 const value_type& _height );

      template<typename U>
      rectangle( const coordinate_2d<U>& pos, const coordinate_2d<U>& size );

      template<typename U>
      rectangle<U> cast_value_type_to() const;

      bool operator==( const self_type& that ) const;
      bool operator!=( const self_type& that ) const;
      
      value_type area() const;
      bool includes( const coordinate_2d<value_type>& p ) const;
      bool includes( const self_type& r ) const;
      bool intersects( const self_type& r ) const;
      self_type intersection( const self_type& r ) const;
      self_type join( const self_type& r ) const;

      void set( const value_type& new_x, const value_type& new_y,
                const value_type& new_width, const value_type& new_height );

      value_type left() const;
      value_type right() const;
      value_type bottom() const;
      value_type top() const;

      coordinate_2d<value_type> size() const;

    private:
      void x_intersection( const self_type& r, self_type& result ) const;
      void y_intersection( const self_type& r, self_type& result ) const;

    public:
      /** \brief value_typeop left coordinates. */
      coordinate_2d<value_type> position;

      /** \brief Width. */
      value_type width;

      /** \brief Height. */
      value_type height;

    }; // class rectangle
  } // namespace math
} // namespace claw

#include <claw/impl/rectangle.tpp>

#endif // __CLAW_RECTANGLE_HPP__
