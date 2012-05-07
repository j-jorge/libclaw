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
 * \file buffered_istream.hpp
 * \brief This class is made to help reading istreams with a buffer.
 * \author Julien Jorge
 */
#ifndef __CLAW_BUFFERED_ISTREAM_HPP__
#define __CLAW_BUFFERED_ISTREAM_HPP__

#include<iostream>

namespace claw
{
  /**
   * \brief This class is made to help reading istreams with a buffer.
   * \author Julien Jorge
   */
  template< typename Stream >
  class buffered_istream
  {
  private:
    /** \brief The type of the stream we will read. */
    typedef Stream stream_type;

  public:
    buffered_istream( stream_type& f );
    ~buffered_istream();
    
    unsigned int remaining() const;
    bool read_more( unsigned int n );
    
    const char* get_buffer() const;
    char get_next();
    bool read( char* buf, unsigned int n );

    void move( unsigned int n );

    void close();

    operator bool() const;

  private:
    /** \brief The stream we're reading. */
    stream_type& m_stream;
    
    /** \brief Pointer to the begining of the buffer. */
    char* m_begin;
    
    /** \brief Pointer to the first invalid byte after the end of the
        buffer. */
    char* m_end;
    
    /** \brief Pointer to the current not already read valid byte. */
    char* m_current;
    
    /** \brief The size of the allocated buffer. */
    unsigned int m_buffer_size;

  }; // class buffered_istream
} // namespace claw

#include <claw/impl/buffered_istream.tpp>

#endif // __CLAW_BUFFERED_ISTREAM_HPP__
