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
 * \file log_stream_uniq.cpp
 * \brief Implementation of the log_stream_uniq class.
 * \author Julien Jorge
 */
#include <claw/logger/log_stream_uniq.hpp>

#include <claw/claw_gettext.hpp>

#include <sstream>

/**
 * \brief Constructor.
 * \param s The stream in which the messages are finally written. It will be
 *        deleted in the destructor.
 */
claw::log_stream_uniq::log_stream_uniq(log_stream* s)
  : m_stream(s)
  , m_repetition_count(0)
{}

/**
 * \brief Destructor.
 */
claw::log_stream_uniq::~log_stream_uniq()
{
  delete m_stream;
}

/**
 * \brief Write a string in the stream.
 * \param str The sring to write.
 */
void claw::log_stream_uniq::write(const std::string& str)
{
  std::string::size_type p = str.find_first_of('\n');

  if(p == std::string::npos)
    m_current_line += str;
  else
    {
      ++p; // includes the '\n'
      m_current_line += str.substr(0, p);

      output_current_line();

      if(p != str.length())
        write(str.substr(p));
    }
}

/**
 * \brief Flush the stream
 */
void claw::log_stream_uniq::flush()
{
  m_stream->flush();
}

/**
 * \brief Output the current line, if not in the history.
 */
void claw::log_stream_uniq::output_current_line()
{
  if(m_current_line == m_previous_line)
    ++m_repetition_count;
  else
    {
      if(m_repetition_count > 0)
        {
          std::ostringstream oss;
          oss << claw_gettext("(Previous line repeated ") << m_repetition_count
              << claw_ngettext(" time)", " times)", m_repetition_count)
              << '\n';

          m_stream->write(oss.str());
        }

      m_repetition_count = 0;

      m_previous_line = m_current_line;
      m_stream->write(m_current_line);
    }

  m_current_line.clear();
}
