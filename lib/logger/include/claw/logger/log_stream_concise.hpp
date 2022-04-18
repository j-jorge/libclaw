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
 * \file log_stream_concise.hpp
 * \brief A log stream that does not output a message that have been recently
 *        output.
 * \author Julien Jorge
 */
#ifndef __CLAW_LOG_STREAM_CONCISE_HPP__
#define __CLAW_LOG_STREAM_CONCISE_HPP__

#include <claw/logger/log_stream.hpp>

#include <list>
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
   * \brief A log stream that does not output a message that have been recently
   *        output.
   * \author Julien Jorge
   */
  class CLAW_LOGGER_EXPORT log_stream_concise
    : public log_stream
    , public pattern::non_copyable
  {
  public:
    explicit log_stream_concise(log_stream* s,
                                std::size_t max_history_size = 25);
    virtual ~log_stream_concise();

    virtual void write(const std::string& str);
    virtual void flush();

  private:
    void output_current_line();

  private:
    /** \brief The stream in which the messages are finally written. */
    log_stream* m_stream;

    /** \brief The current line to send into the stream. */
    std::string m_current_line;

    /** \brief Recent lines sent to the stream. */
    std::list<std::string> m_previous_lines;

    /** \brief Maximum number of lines in the history. */
    std::size_t m_max_history_size;

  }; // class log_stream_concise
}

#endif // __CLAW_LOG_STREAM_CONCISE_HPP__
