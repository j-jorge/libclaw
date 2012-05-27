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
 * \file claw/coordinate_traits.hpp
 * \brief The coordinate traits provide an access to the members of the
 *        structures representing a coordinate in a 2D space.
 * \author Julien Jorge
 */
#ifndef __CLAW_MATH_COORDINATE_TRAITS_HPP__
#define __CLAW_MATH_COORDINATE_TRAITS_HPP__

#include <claw/coordinate_2d.hpp>

namespace claw
{
  namespace math
  {
    /**
     * \brief The coordinate traits provide an access to the members of the
     *        structures representing a coordinate in a 2D space.
     *
     * \author Julien Jorge
     */
    template< typename C >
    struct coordinate_traits;

    /**
     * \brief Specialization of the coordinate_traits for
     *        claw::math::coordinate_2d.
     * \author Julien Jorge
     */
    template<typename T>
    struct coordinate_2d< claw::math::coordinate_2d<T> >
    {
      /** \brief The type of the coordinate. */
      typedef claw::math::coordinate_2d<T> coordinate_type;

      /** \brief The type of the components of the coordinate. */
      typedef typename coordinate_type::value_type value_type;

      /**
       * \brief Get the component of a coordinate on the x-axis.
       * \param c The coordinate from which the component is taken.
       */
      static value_type get_x( const coordinate_type& c )
      {
        return c.x;
      } // get_x()

      /**
       * \brief Get the component of a coordinate on the y-axis.
       * \param c The coordinate from which the component is taken.
       */
      static value_type get_y( const coordinate_type& c )
      {
        return c.y;
      } // get_y()

      /**
       * \brief Create a new coordinate from with the components sets to the
       *        provided values.
       * \param x The position on the x axis.
       * \param y The position on the y axis.
       */
      static coordinate_type make_coordinate( value_type x, value_type y )
      {
        return coordinate_type(x, y);
      } // make_coordinate()

    } // struct coordinate_traits [claw::math::coordinate_2d]

  } // namespace math
} // namespace claw

#endif // __CLAW_MATH_COORDINATE_TRAITS_HPP__
