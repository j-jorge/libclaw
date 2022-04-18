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
 * \file bit_ostream.hpp
 * \brief This class is made to help writing datas of custom bit length.
 * \author Julien Jorge
 */
#ifndef __CLAW_BIT_OSTREAM_HPP__
#define __CLAW_BIT_OSTREAM_HPP__

namespace claw
{
  /**
   * \brief This class is made to help writing datas of custom bit length.
   * \author Julien Jorge
   */
  template <typename Stream>
  class bit_ostream
  {
  private:
    /** \brief The type of the stream we will write. */
    typedef Stream stream_type;

  public:
    bit_ostream(stream_type& f);
    ~bit_ostream();

    void write(const char* buf, unsigned int n);

  private:
    /** \brief The stream we're reading. */
    stream_type& m_stream;

    /** \brief Some bits available for writing. */
    unsigned char m_pending;

    /** \brief The number of valid bits in m_pending. */
    unsigned char m_pending_length;

  }; // class bit_ostream
}

#include <claw/bit_ostream.tpp>

#endif // __CLAW_BIT_OSTREAM_HPP__
