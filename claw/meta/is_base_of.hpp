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
 * \file is_base_of.hpp
 * \brief Check if a class is derived from an other class.
 * \author Julien Jorge
 */
#ifndef __CLAW_IS_BASE_OF_HPP__
#define __CLAW_IS_BASE_OF_HPP__

namespace claw
{
  namespace meta
  {
    /**
     * \brief This predicate tells if a class is derived from an other class.
     *
     * \b Template \b parameters
     * - \a Base, the supposed base class,
     * - \a Derived, the derived type to check.
     *
     * The result can be retrieved via the is_base_of::result value.
     *
     * \author Julien Jorge
     */
    template<typename Base, typename Derived>
    struct is_base_of
    {
    private:
      static Derived* m_d;

    private:
      static char check( Base* );
      static long check( ... );

    public:
      enum
	{
	  result = ( sizeof(check( m_d )) == sizeof(char) )
	};
    }; // struct is_base_of [false]

  } // namespace meta
} // namespace claw

#endif // __CLAW_IS_BASE_OF_HPP__
