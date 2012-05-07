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

  contact: julien.jorge@gamned.org
*/
/**
 * \file log_stream.hpp
 * \brief Some basic classes for logging.
 * \author Julien Jorge
 */
#ifndef __CLAW_LOG_STREAM_HPP__
#define __CLAW_LOG_STREAM_HPP__

#include <string>
#include <fstream>
#include <claw/non_copyable.hpp>

#ifndef CLAW_LOGGER_EXPORT
#  ifdef CLAW_LOGGER_NO_EXPORT
#    define CLAW_LOGGER_EXPORT
#  else
#    ifdef _WIN32
#      ifdef claw_logger_EXPORTS
#        define CLAW_LOGGER_EXPORT __declspec(dllexport)
#      else
#        define CLAW_LOGGER_EXPORT __declspec(dllimport)
#      endif // def claw_logger_EXPORTS
#    else // def _WIN32
#      define CLAW_LOGGER_EXPORT
#    endif // def _WIN32
#  endif // def CLAW_LOGGER_NO_EXPORT
#endif // ndef CLAW_LOGGER_EXPORT

namespace claw
{
  /**
   * \brief Base class for streams accepting log output.
   * \author Julien Jorge
   */
  class CLAW_LOGGER_EXPORT log_stream
  {
  public:
    virtual ~log_stream();
    virtual void write( const std::string& str ) = 0;
    virtual void flush();
  }; // class log_stream

  /**
   * \brief This class write log messages in std::clog.
   * \author Julien Jorge
   */
  class CLAW_LOGGER_EXPORT console_logger
    : public log_stream,
      public pattern::non_copyable
  {
  public:
    virtual void write( const std::string& str );
    virtual void flush();
  }; // class console_logger

  /**
   * \brief This class write log messages in a file.
   * \author Julien Jorge
   */
  class CLAW_LOGGER_EXPORT file_logger
    : public log_stream,
      public pattern::non_copyable
  {
  public:
    file_logger( const std::string& filename );
    virtual ~file_logger();

    virtual void write( const std::string& str );
    virtual void flush();

  private:
    /** \brief The file in which we write the messages. */
    std::ofstream* m_file;

  }; // class file_logger
} // namespace claw

#endif // __CLAW_LOG_STREAM_HPP__
