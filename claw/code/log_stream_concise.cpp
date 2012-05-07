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
 * \file log_stream_concise.cpp
 * \brief Implementation of the log_stream_concise class.
 * \author Julien Jorge
 */
#include <claw/log_stream_concise.hpp>

#include <algorithm>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param s The stream in which the messages are finally written. It will be
 *        deleted in the destructor.
 * \param max_history_size The maximum number of lines in the history.
 */
claw::log_stream_concise::log_stream_concise
( log_stream* s, std::size_t max_history_size )
  : m_stream(s), m_max_history_size(max_history_size)
{

} // log_stream_concise::log_stream_concise()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
claw::log_stream_concise::~log_stream_concise()
{
  delete m_stream;
} // log_stream_concise::~log_stream_concise()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a string in the stream.
 * \param str The sring to write.
 */
void claw::log_stream_concise::write( const std::string& str )
{
  std::string::size_type p = str.find_first_of('\n');

  if ( p == std::string::npos )
    m_current_line += str;
  else
    {
      ++p; // includes the '\n'
      m_current_line += str.substr(0, p);

      output_current_line();

      if ( p != str.length() )
        write( str.substr(p) );
    }
} // log_stream_concise::write()

/*----------------------------------------------------------------------------*/
/**
 * \brief Flush the stream
 */
void claw::log_stream_concise::flush()
{
  m_stream->flush();
} // log_stream_concise::flush()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output the current line, if not in the history.
 */
void claw::log_stream_concise::output_current_line()
{
  if ( std::find
       (m_previous_lines.begin(), m_previous_lines.end(), m_current_line)
       == m_previous_lines.end() )
    {
      m_previous_lines.push_back( m_current_line );
      m_stream->write( m_current_line );

      if (m_previous_lines.size() > m_max_history_size)
	m_previous_lines.pop_front();
    }

  m_current_line.clear();
} // log_stream_concise::output_current_line()
