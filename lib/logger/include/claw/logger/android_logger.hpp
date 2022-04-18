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
 * \file
 * \brief Log messages using Android's log system.
 * \author Julien Jorge
 */
#ifdef __ANDROID__

#ifndef __CLAW_ANDROID_LOGGER_HPP__
#define __CLAW_ANDROID_LOGGER_HPP__

#include <claw/logger/log_stream.hpp>

#include <claw/non_copyable.hpp>

namespace claw
{
  /**
   * \brief This class write log messages using Android's log system.
   * \author Julien Jorge
   */
  class android_logger
    : public claw::log_stream
    , public claw::pattern::non_copyable
  {
  public:
    explicit android_logger(const std::string& prefix);

    virtual void write(const std::string& str);
    virtual void flush();

  private:
    /** \brief The prefix of the lines sent to the logger. */
    const std::string m_prefix;

    /** \brief The strings writen in the log and not yet followed by a new
        line. */
    std::string m_output;

  }; // class file_logger
}

#endif // __CLAW_ANDROID_LOGGER_HPP__

#else // __ANDROID__
#error "claw::android_logger cannot be used outside Android applications."
#endif // __ANDROID__
