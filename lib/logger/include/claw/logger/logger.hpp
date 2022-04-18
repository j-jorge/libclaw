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
 * \file logger.hpp
 * \brief Some basic classes for logging.
 * \author Julien Jorge
 */
#ifndef __CLAW_LOGGER_HPP__
#define __CLAW_LOGGER_HPP__

#include <claw/logger/log_level.hpp>
#include <claw/logger/log_stream.hpp>

#include <list>
#include <mutex>
#include <ostream>
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
   * \brief A class implementing a logging system.
   *
   * Messages are sent to the log system. If the importance (level) of a
   * message is lower or equal to a given threshold, the message is printed.
   * Otherwise, it is ignored.
   *
   * Message printing is managed by log_stream classes. The logger_system can
   * contain several log_stream. None checking is done when adding a log_stream
   * to see if it is already in the system.
   *
   * \author Julien Jorge
   */
  class log_system
  {
  public:
    typedef log_stream stream_type;
    typedef std::list<stream_type*> stream_list_type;

  public:
    CLAW_LOGGER_EXPORT log_system();
    CLAW_LOGGER_EXPORT ~log_system();
    CLAW_LOGGER_EXPORT void clear();

    CLAW_LOGGER_EXPORT void merge(stream_type* s);
    CLAW_LOGGER_EXPORT void remove(const stream_type* s);
    CLAW_LOGGER_EXPORT void set(stream_type* s);
    CLAW_LOGGER_EXPORT void set_level(int lvl);
    CLAW_LOGGER_EXPORT void set_level(const log_level& lvl);

    CLAW_LOGGER_EXPORT void flush();

    template <typename T>
    log_system& operator<<(const T& that);

    CLAW_LOGGER_EXPORT log_system& operator<<(const log_level& that);
    CLAW_LOGGER_EXPORT log_system& operator<<(log_system& (*pf)(log_system&));

  private:
    void print(const std::string& s);

  private:
    /** \brief The mutex used to avoid changing the content of the logs
        by two sources simultaneously. */
    std::mutex m_mutex;

    /** \brief The level of log. Messages are ignored if their level is greater
        than this level. */
    int m_log_level;

    /** \brief The current message level, for operator << */
    int m_message_level;

    /** \brief The streams in which we write de log informations. */
    stream_list_type m_stream;

  }; // class log_system

  CLAW_LOGGER_EXPORT extern log_system logger;

  CLAW_LOGGER_EXPORT log_system& lendl(log_system& log);

}

namespace std
{
  CLAW_LOGGER_EXPORT claw::log_system& endl(claw::log_system& log);
}

// template methods
#include <claw/logger/logger.tpp>

#endif // __CLAW_LOGGER_HPP__
