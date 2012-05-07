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
 * \file bit_istream.tpp
 * \brief Implementation of the claw::bit_istream class.
 * \author Julien Jorge
 */
#include <climits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The stream in which we read.
 */
template<typename Stream>
claw::bit_istream<Stream>::bit_istream( stream_type& f )
  : m_stream(f), m_pending(0), m_pending_length(0)
{

} // bit_istream::bit_istream()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read some bits.
 * \param buf A buffer in which we write the bits.
 * \param n The number of bits to read.
 */
template<typename Stream>
void claw::bit_istream<Stream>::read( char* buf, unsigned int n )
{
  if ( n == 0 )
    return;

  unsigned int cur_size = 0;

  while ( (n != 0) && !!(*this) )
    {
      while( (m_pending_length != 0) && (n!=0) && !!(*this) )
        {
          unsigned int bits = std::min((unsigned int)m_pending_length, n);

          if ( CHAR_BIT - cur_size < bits )
            bits = CHAR_BIT - cur_size;

          unsigned int mask = (1 << bits) - 1;

          *buf |= (m_pending & mask) << cur_size;
          cur_size += bits;
          m_pending_length -= bits;
          m_pending >>= bits;
          n -= bits;

          if ( cur_size == CHAR_BIT )
            {
              ++buf;
              cur_size = 0;
            }
        }

      if ( m_pending_length == 0 )
        if ( m_stream.read( (char*)&m_pending, sizeof(m_pending) ) )
          m_pending_length = CHAR_BIT;
    }
} // bit_istream::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the input stream is still valid.
 */
template<typename Stream>
claw::bit_istream<Stream>::operator bool() const
{
  return m_stream || (m_pending_length > 0);
} // bit_istream::operator bool()
