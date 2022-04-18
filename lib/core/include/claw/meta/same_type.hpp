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
 * \file same_type.hpp
 * \brief Check if two types are the same.
 * \author Julien Jorge
 */
#ifndef __CLAW_SAME_TYPE_HPP__
#define __CLAW_SAME_TYPE_HPP__

namespace claw
{
  namespace meta
  {
    /**
     * \brief This predicate tells if two types are the sames.
     *
     * \b Template \b parameters
     * - \a T, \a U The types to compare.
     *
     * The result can be retrieved via the same_type::result value.
     *
     * \author Julien Jorge
     */
    template <typename T, typename U>
    struct same_type
    {
      enum
      {
        result = false
      };
    }; // struct same_type [false]

    template <typename T>
    struct same_type<T, T>
    {
      enum
      {
        result = true
      };
    }; // struct same_type [true]

  }
}

#endif // __CLAW_SAME_TYPE_HPP__
