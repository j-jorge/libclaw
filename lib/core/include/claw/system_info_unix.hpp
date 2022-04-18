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
 * \file system_info_unix.hpp
 * \brief A class to get some informations about the system in which your
 *        program runs.
 * \author Julien Jorge
 */
#ifndef __CLAW_SYSTEM_INFO_UNIX_HPP__
#define __CLAW_SYSTEM_INFO_UNIXHPP__

#include <cstdlib>
#include <string>

namespace claw
{
  /**
   * \brief A class to get some informations about the system in which your
   *        program runs.
   * \author Julien Jorge
   */
  class system_info
  {
  public:
    /**
     * \brief Get the name of the user (the name used for the connection).
     */
    static std::string get_user_name()
    {
      std::string result(get_environment("USER"));

      if(result.empty())
        result = get_environment("LOGNAME");

      return result;
    }

    /**
     * \brief Get the path of the user's personnal directory.
     */
    static std::string get_user_directory()
    {
      return get_environment("HOME");
    }

    /**
     * \brief Get system's paths.
     */
    template <typename Sequence>
    static void get_path(Sequence& path)
    {
      //
    }

    /**
     * \brief Get the name of out host.
     */
    static std::string get_host_name()
    {
      return get_environment("HOSTNAME");
    }

    /**
     * \brief Get the path of the directory used by the system to save
     * temporary files.
     */
    static std::string get_temporary_directory()
    {
      return get_environment("TMPDIR");
    }

    /**
     * \brief Get an environment variable.
     * \param var The name of the variable to get.
     */
    static std::string get_environment(const std::string& var)
    {
      char* value = getenv(var.c_str());
      std::string result;

      if(value)
        result = value;

      return result;
    }

  }; // class system_info
}

#endif // __CLAW_SYSTEM_INFO_UNIX_HPP__
