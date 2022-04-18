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
 * \brief Implementation of the claw::android_logger class.
 * \author Julien Jorge
 */
#ifdef __ANDROID__

#include "claw/logger/android_logger.hpp"

#include <android/log.h>

/**
 * \brief Constructs a new logger to write in Android logs with a given prefix.
 * \param prefix The prefix of the lines sent to the logger.
 */
claw::android_logger::android_logger(const std::string& prefix)
  : m_prefix(prefix)
{}

/**
 * \brief Prints something in the log.
 */
void claw::android_logger::write(const std::string& str)
{
  m_output += str;

  const std::string::size_type pos(m_output.find_last_of('\n'));

  if(pos != std::string::npos)
    {
      std::string log_string(m_output.substr(0, pos));
      m_output = m_output.substr(pos + 1);

      __android_log_print(ANDROID_LOG_INFO, m_prefix.c_str(), "%s",
                          log_string.c_str());
    }
}

/**
 * \brief Prints the buffered logs.
 */
void claw::android_logger::flush()
{
  if(m_output.empty())
    return;

  __android_log_print(ANDROID_LOG_INFO, m_prefix.c_str(), "%s",
                      m_output.c_str());

  m_output.clear();
}

#endif // __ANDROID__
