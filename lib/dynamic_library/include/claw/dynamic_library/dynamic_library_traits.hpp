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
 * \file dynamic_library_traits.hpp
 * \brief Include the good interface for dynamic libraries for your system.
 * \author Julien Jorge
 */
#ifndef __CLAW_DYNAMIC_LIBRARY_TRAITS_HPP__
#define __CLAW_DYNAMIC_LIBRARY_TRAITS_HPP__

/**
 * \class claw::dynamic_library_traits
 *
 * \brief Common interface for platform specific methods needed for using
 *        dynamic library.
 *
 * The claw::dynamic_library_traits implements platform specific methods for
 * dynamic library usage through a common interface. This interface must
 * include :
 * - a type named \c handle, representing the system handle to a library,
 * - a method <tt> handle open(std::string) </tt> for opening a library. This
 *   method can throw an exception if a problem occurs,
 * - a method <tt> handle auto_open(std::string) </tt> for opening the current
 *   program. the string will be the name (path) of the current program. This
 *   method can throw an exception if a problem occurs,
 * - a method <tt> void close(handle) </tt> for closing a library,
 * - a method <tt> <em> implementation_defined_pointer </em>
 *   get_symbol(handle, std::string) </tt> for getting a pointer to a symbol of
 *   the library,
 * - a methof <tt> bool have_symbol(handle, std::string) </tt> saying if a
 *   library has a specific symbol,
 * - a method <tt> bool valid_handle(handle) </tt> which tell if an handle
 *   represent a valid handle.
 *
 * All these methods must be defined as <tt> static </tt>.
 */

#ifdef _WIN32
#include <claw/dynamic_library/dynamic_library_traits_win32.hpp>
#else
#include <claw/dynamic_library/dynamic_library_traits_unix.hpp>
#endif

#endif // __CLAW_DYNAMIC_LIBRARY_TRAITS_HPP__
