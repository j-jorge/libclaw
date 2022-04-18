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
 * \file dynamic_library_traits_unix.hpp
 * \brief Unix interface for using dynamic libraries.
 * \author Julien Jorge
 */
#ifndef __CLAW_DYNAMIC_LIBRARY_TRAITS_UNIX_HPP__
#define __CLAW_DYNAMIC_LIBRARY_TRAITS_UNIX_HPP__

#include <claw/exception.hpp>
#include <dlfcn.h>
#include <string>

namespace claw
{
  /**
   * \brief Unix interface for using dynamic libraries.
   * \author Julien Jorge
   */
  class dynamic_library_traits_unix
  {
  public:
    /** \brief Type of the system handle to the library. */
    typedef void* handle;

  public:
    /**
     * \brief Open a library.
     * \param name The name of the library to open.
     * \return The handle on the loaded library.
     */
    static handle open(const std::string& name)
    {
      handle h = dlopen(name.c_str(), RTLD_LAZY);

      if(!valid_handle(h))
        throw claw::exception(dlerror());

      return h;
    }

    /**
     * \brief Open the current program.
     * \param name (not used) The name of the current program.
     * \return The handle on the loaded library.
     */
    static handle auto_open(const std::string& name)
    {
      handle h = dlopen(NULL, RTLD_LAZY);

      if(!valid_handle(h))
        throw claw::exception(dlerror());

      return h;
    }

    /**
     * \brief Close a library.
     * \param h The handle of the library to close.
     */
    static void close(handle h)
    {
      dlclose(h);
    }

    /**
     * \brief Get a symbol from a library.
     * \param h Handle of the library.
     * \param name The name of the symbol to load.
     */
    template <class T>
    static T get_symbol(handle h, const std::string& name)
    {
      /* HACK : ISO standard doesn't allow to cast from a pointer to an object
         to a pointer to a function. */
      T result;
      *(void**)(&result) = dlsym(h, name.c_str());

      return result;
    }

    /**
     * \brief Tell if a symbol is in the library.
     * \param h Handle of the library.
     * \param name The name of the symbol to find.
     */
    static bool have_symbol(handle h, const std::string& name)
    {
      return dlsym(h, name.c_str()) != NULL;
    }

    /**
     * \brief Tell if an handle is a valid library handle.
     * \param h The handle to test.
     */
    static bool valid_handle(handle h)
    {
      return h != NULL;
    }

  }; // class dynamic_library_traits_unix

  /** \brief The traits to access the dynamic libraries in Unix system. */
  typedef dynamic_library_traits_unix dynamic_library_traits;

}

#endif // __CLAW_DYNAMIC_LIBRARY_TRAITS_UNIX_HPP__
