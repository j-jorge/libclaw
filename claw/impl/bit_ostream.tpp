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
 * \file bit_ostream.tpp
 * \brief Implementation of the claw::bit_ostream class.
 * \author Julien Jorge
 */
#include <climits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The stream in which we write.
 */
template<typename Stream>
claw::bit_ostream<Stream>::bit_ostream( stream_type& f )
  : m_stream(f), m_pending(0), m_pending_length(0)
{

} // bit_ostream::bit_ostream()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
template<typename Stream>
claw::bit_ostream<Stream>::~bit_ostream()
{
  if (m_pending_length != 0)
    m_stream.write( (char*)&m_pending, sizeof(m_pending) );
} // bit_ostream::~bit_ostream()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write some bits.
 * \param buf A buffer from which we read the bits.
 * \param n The number of bits to write.
 */
template<typename Stream>
void claw::bit_ostream<Stream>::write( const char* buf, unsigned int n )
{
  if ( n == 0 )
    return;

  unsigned int cur_size = 0;
  unsigned char data = *buf;

  while ( n != 0 )
    {
      while( (m_pending_length != CHAR_BIT) && (n!=0) )
        {
          unsigned int bits =
            std::min(CHAR_BIT - (unsigned int)m_pending_length, n);

          if ( CHAR_BIT - cur_size < bits )
            bits = CHAR_BIT - cur_size;

          unsigned int mask = (1 << bits) - 1;

          m_pending |= (data & mask) << m_pending_length;
          cur_size += bits;
          m_pending_length += bits;
          data >>= bits;
          n -= bits;

          if ( (cur_size == CHAR_BIT) && (n!=0) )
            {
              ++buf;
              cur_size = 0;
              data = *buf;
            }
        }

      if ( m_pending_length == CHAR_BIT )
        {
          m_stream.write( (char*)&m_pending, sizeof(m_pending) );
          m_pending = 0;
          m_pending_length = 0;
        }
    }
} // bit_ostream::write()
