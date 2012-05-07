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
 * \file buffered_ostream.tpp
 * \brief Implementation of the claw::buffered_ostream class.
 * \author Julien Jorge
 */
#include <cassert>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The file associated to the stream.
 * \param buffer_size The size of the buffer.
 */
template< typename Stream >
claw::buffered_ostream<Stream>::buffered_ostream
( stream_type& f, unsigned int buffer_size )
  : m_stream(f), m_begin(new char[buffer_size]), m_end(m_begin+buffer_size),
    m_current(m_begin)
{

} // buffered_ostream::buffered_ostream()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
template< typename Stream >
claw::buffered_ostream<Stream>::~buffered_ostream()
{
  flush();
  delete[] m_begin;
} // buffered_ostream::~buffered_ostream()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write somethnig in the buffer.
 * \param v The value to write.
 */
template< typename Stream >
template<typename T>
void claw::buffered_ostream<Stream>::write( T v )
{
  write( reinterpret_cast<const char*>(&v), sizeof(v) );
} // buffered_ostream::read_more()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a range of data in the buffer.
 * \param p The begining of the range to write.
 * \param n The length of the buffer pointed by p.
 */
template< typename Stream >
void claw::buffered_ostream<Stream>::write(const char* p, unsigned int n )
{
  while (n > 0)
    {
      unsigned int q = std::min( n, (unsigned int)(m_end - m_current) );
      const char* end = p+q;

      for ( ; p!=end ; ++p, ++m_current )
        *m_current = *p;

      n -= q;

      if (m_current == m_end)
        flush();
    }
} // buffered_ostream::write()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write the data from the buffer in the stream.
 */
template< typename Stream >
void claw::buffered_ostream<Stream>::flush()
{
  if (m_current != m_begin)
    {
      m_stream.write( m_begin, m_current - m_begin );
      m_current = m_begin;
    }
} // buffered_ostream::flush()
