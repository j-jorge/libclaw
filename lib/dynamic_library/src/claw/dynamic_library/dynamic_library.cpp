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
 * \file dynamic_library.cpp
 * \brief Implementation of the claw::dynamic_library class.
 * \author Julien Jorge
 */
#include <claw/dynamic_library/dynamic_library.hpp>

#include <claw/exception.hpp>

/**
 * \brief Constructor.
 * \param name Name of the library to load.
 * \param current_program Tell if the \a name parameter is the current program.
 */
claw::dynamic_library::dynamic_library(const std::string& name,
                                       bool current_program)
{
  if(current_program)
    m_handle = dynamic_library_traits::auto_open(name);
  else
    m_handle = dynamic_library_traits::open(name);

  if(!dynamic_library_traits::valid_handle(m_handle))
    throw exception("Can't open library '" + name + "'");
}

/**
 * \brief Destructor (close the library).
 */
claw::dynamic_library::~dynamic_library()
{
  dynamic_library_traits::close(m_handle);
}

/**
 * \brief Tell if the library has a symbol.
 * \param name The symbol to find.
 */
bool claw::dynamic_library::have_symbol(const std::string& name) const
{
  return dynamic_library_traits::have_symbol(m_handle, name);
}
