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
 * \file buffered_istream.tpp
 * \brief Implementation of the claw::buffered_istream class.
 * \author Julien Jorge
 */
#include <cassert>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The file associated to the stream.
 */
template<typename Stream>
claw::buffered_istream<Stream>::buffered_istream( stream_type& f )
  : m_stream(f), m_begin(NULL), m_end(NULL), m_current(NULL),
    m_buffer_size(1024)
{
  m_begin = new char[m_buffer_size];
  m_end = m_begin;
  m_current = m_end;
} // buffered_istream::buffered_istream()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
template<typename Stream>
claw::buffered_istream<Stream>::~buffered_istream()
{
  close();

  if (m_begin)
    delete[] m_begin;
} // buffered_istream::~buffered_istream()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell how many bytes are ready in the buffer.
 */
template<typename Stream>
unsigned int claw::buffered_istream<Stream>::remaining() const
{
  return m_end - m_current;
} // buffered_istream::remaining()

/*----------------------------------------------------------------------------*/
/**
 * \brief Increase the number of ready bytes to a given number.
 * \param n The number of bytes you need.
 * \remark This method reads n - remaining() bytes from the file.
 */
template<typename Stream>
bool claw::buffered_istream<Stream>::read_more( unsigned int n )
{
  if ( n <= remaining() )
    return true;

  unsigned int r = remaining();

  // we'll reach the end of the buffer
  if ( m_current + n > m_begin + m_buffer_size )
    {
      // try to avoid reallocation
      if (n <= m_buffer_size)
        std::copy(m_current, m_end, m_begin);
      else // not enough space in the buffer
        {
          m_buffer_size = n;

          char* new_buffer = new char[m_buffer_size];

          std::copy(m_current, m_end, new_buffer);

          delete[] m_begin;

          m_begin = new_buffer;
        }

      m_current = m_begin;
      m_end = m_current + r;
    }

  m_stream.read( m_end, n-r );
  m_end += m_stream.gcount();

  return !!m_stream;
} // buffered_istream::read_more()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the input buffer.
 */
template<typename Stream>
const char* claw::buffered_istream<Stream>::get_buffer() const
{
  return m_current;
} // buffered_istream::get_buffer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the next value in the buffer and move one byte forward.
 */
template<typename Stream>
char claw::buffered_istream<Stream>::get_next()
{
  assert( remaining() >= 1 );

  char result = *m_current;
  ++m_current;

  return result;
} // buffered_istream::get_next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a range of data.
 * \param buf The buffer in which we write the read data.
 * \param n The number of bytes to read.
 */
template<typename Stream>
bool claw::buffered_istream<Stream>::read( char* buf, unsigned int n )
{
  while ( (n != 0) && !!(*this) )
    {
      if ( n > remaining() )
        read_more(m_buffer_size);

      unsigned int len = std::min(n, remaining());

      std::copy( m_current, m_current + len, buf );
      buf += len;
      n -= len;
      m_current += len;
    }

  return n==0;
} // buffered_istream::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move some bytes forward.
 * \param n The number of bytes to skip.
 */
template<typename Stream>
void claw::buffered_istream<Stream>::move( unsigned int n )
{
  assert( m_current + n <= m_end );
  m_current += n;
} // buffered_istream::move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Closes this buffer (not the stream).
 *
 * The cursor of the stream is repositioned according to the remaining data, and
 * the buffer is cleared.
 */
template<typename Stream>
void claw::buffered_istream<Stream>::close()
{
  m_stream.seekg( m_current - m_end, std::ios_base::cur );
  m_current = m_begin;
  m_end = m_begin;
} // buffered_istream::close()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is still datas in the buffer/stream.
 */
template<typename Stream>
claw::buffered_istream<Stream>::operator bool() const
{
  return m_stream || (remaining() > 0);
} // buffered_istream::operator bool()
