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
 * \file non_copyable.hpp
 * \brief Inherit from this class to forbid copy constructor in your class.
 * \author Julien Jorge
 */
#ifndef __CLAW_NON_COPYABLE_HPP__
#define __CLAW_NON_COPYABLE_HPP__

namespace claw
{
  namespace pattern
  {
    /**
     * \brief Inherit from this class to forbid construction of your class by
     *        copy.
     * \author Julien Jorge
     */
    class non_copyable
    {
    public:
      /** \brief Default constructor. */
      non_copyable()
      {}

    private:
      /**
       * \brief Copy constructor is declared private and non implemented.
       * \param that Instance to copy from.
       */
      non_copyable(const non_copyable& that);

      /**
       * \brief Assignment operator is declared private and non implemented.
       * \param that Instance to copy from.
       */
      non_copyable& operator=(const non_copyable& that);

    }; // non_copyable
  }

  namespace concept = pattern;

}

#endif // __CLAW_NON_COPYABLE_HPP__
