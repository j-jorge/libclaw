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
 * \file log_level.hpp
 * \brief A class to pass log information to the loggers.
 * \author Julien Jorge
 */
#ifndef __CLAW_LOG_LEVEL_HPP__
#define __CLAW_LOG_LEVEL_HPP__

#include <string>

#ifndef CLAW_LOGGER_EXPORT
#ifdef CLAW_LOGGER_NO_EXPORT
#define CLAW_LOGGER_EXPORT
#else
#ifdef _WIN32
#ifdef claw_logger_EXPORTS
#define CLAW_LOGGER_EXPORT __declspec(dllexport)
#else
#define CLAW_LOGGER_EXPORT __declspec(dllimport)
#endif // def claw_logger_EXPORTS
#else  // def _WIN32
#define CLAW_LOGGER_EXPORT
#endif // def _WIN32
#endif // def CLAW_LOGGER_NO_EXPORT
#endif // ndef CLAW_LOGGER_EXPORT

namespace claw
{
  /**
   * \brief Set the level of the next message for logger_system::operator<<().
   * \remark Levels from 0 to 15 are reserved to claw.
   */
  class CLAW_LOGGER_EXPORT log_level
  {
  public:
    log_level(int lvl);
    log_level(int lvl, const std::string& s);
    int get() const;
    std::string get_string() const;

  private:
    /** \brief Value of the level. */
    const int m_level;

    /** \brief String used for prefix for the current message. */
    const std::string m_prefix;

  }; // class log_level

  /**
   * \brief Use this level if something goes really bad and your application
   *        may crash.
   */
  CLAW_LOGGER_EXPORT extern log_level log_error;

  /**
   * \brief Use this level if a small problem occurs and you can deal with
   *        it without crashing the application.
   */
  CLAW_LOGGER_EXPORT extern log_level log_warning;

  /**
   * \brief Use this level if you want to inform the user about a situation
   * that is not problematic.
   */
  CLAW_LOGGER_EXPORT extern log_level log_verbose;

}

#endif // __CLAW_LOG_LEVEL_HPP__
