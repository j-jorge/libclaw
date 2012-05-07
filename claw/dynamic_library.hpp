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
 * \file dynamic_library.hpp
 * \brief A class to use dynamic libraries.
 * \author Julien Jorge
 */
#ifndef __CLAW_DYNAMIC_LIBRARY_HPP__
#define __CLAW_DYNAMIC_LIBRARY_HPP__

#include <claw/dynamic_library_traits.hpp>

namespace claw
{
  /**
   * \brief A class to use dynamic libraries.
   * \author Julien Jorge
   */
  class dynamic_library
  {
  public:
    dynamic_library( const std::string& name, bool current_program = false );
    ~dynamic_library();

    bool have_symbol( const std::string& name ) const;

    template<class T>
    T get_symbol( const std::string& name ) const;

  private:
    /** \brief Handle of the library. */
    dynamic_library_traits::handle m_handle;
  }; // class dynamic_library
} // namespace claw

#include <claw/impl/dynamic_library.tpp>

#endif // __CLAW_DYNAMIC_LIBRARY_HPP__
