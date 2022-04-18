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
 * \file logger.cpp
 * \brief Implementation of the claw::log_system class.
 * \author Julien Jorge
 */
#include <claw/logger/logger.hpp>

#include <algorithm>

namespace claw
{
  /** \brief The default log system provided by claw. */
  log_system logger;
}

/**
 * \brief Default constructor.
 *
 * The logger is initialized with a console logger.
 */
claw::log_system::log_system()
  : m_log_level(-1)
  , m_message_level(0)
{}

/**
 * \brief Destructor.
 */
claw::log_system::~log_system()
{
  clear();
}

/**
 * \brief Delete the streams.
 */
void claw::log_system::clear()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  stream_list_type::iterator it;

  for(it = m_stream.begin(); it != m_stream.end(); ++it)
    delete *it;

  m_stream.clear();
}

/**
 * \brief Add an other output stream.
 * \param s Dynamicaly allocated logger_stream.
 */
void claw::log_system::merge(stream_type* s)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  m_stream.push_front(s);
}

/**
 * \brief Remove a stream.
 * \param s The stream to remove.
 * \remark The search is done on the address of the pointer \a s.
 */
void claw::log_system::remove(const stream_type* s)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  stream_list_type::iterator it
      = std::find(m_stream.begin(), m_stream.end(), s);

  if(it != m_stream.end())
    m_stream.erase(it);
}

/**
 * \brief Set the output stream.
 * \param s Dynamicaly allocated logger_stream.
 */
void claw::log_system::set(stream_type* s)
{
  clear();

  std::lock_guard<std::mutex> lock(m_mutex);

  m_stream.push_front(s);
}

/**
 * \brief Change the level of log.
 * \param lvl New level.
 */
void claw::log_system::set_level(int lvl)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  m_log_level = lvl;
}

/**
 * \brief Change the level of log.
 * \param lvl New level.
 */
void claw::log_system::set_level(const log_level& lvl)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  m_log_level = lvl.get();
}

/**
 * \brief Flush all log streams.
 */
void claw::log_system::flush()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  if(m_message_level <= m_log_level)
    {
      stream_list_type::iterator it;

      for(it = m_stream.begin(); it != m_stream.end(); ++it)
        (*it)->flush();
    }
}

/**
 * \brief Change the level of the next mesasges.
 * \param that The new level.
 */
claw::log_system& claw::log_system::operator<<(const log_level& that)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  m_message_level = that.get();

  if(m_message_level <= m_log_level)
    print(that.get_string());

  return *this;
}

/**
 * \brief Apply a stream modifier function to the log_system.
 * \param pf The function to apply.
 */
claw::log_system& claw::log_system::operator<<(log_system& (*pf)(log_system&))
{
  return pf(*this);
}

/**
 * \brief Writes a string in the underlying streams. The function does not
 * check the mutex. It is the responsability of the caller to lock it. \param s
 * The string to write.
 */
void claw::log_system::print(const std::string& s)
{
  stream_list_type::iterator it;

  for(it = m_stream.begin(); it != m_stream.end(); ++it)
    (*it)->write(s);
}

/**
 * \brief Add a new line caracter to a logger and flush it.
 * \param log The logger to flush.
 */
claw::log_system& claw::lendl(claw::log_system& log)
{
  return log << std::endl;
}

claw::log_system& std::endl(claw::log_system& log)
{
  (log << "\n").flush();
  return log;
}
