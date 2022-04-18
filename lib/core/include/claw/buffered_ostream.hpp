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
 * \file buffered_ostream.hpp
 * \brief This class is made to help wrinting in ostreams with a buffer.
 * \author Julien Jorge
 */
#ifndef __CLAW_BUFFERED_OSTREAM_HPP__
#define __CLAW_BUFFERED_OSTREAM_HPP__

namespace claw
{
  /**
   * \brief This class is made to help writing in ostreams with a buffer.
   * \author Julien Jorge
   */
  template <typename Stream>
  class buffered_ostream
  {
  private:
    /** \brief The type of the stream we will write. */
    typedef Stream stream_type;

  public:
    buffered_ostream(stream_type& f, unsigned int buffer_size = 1024);
    ~buffered_ostream();

    template <typename T>
    void write(T v);

    void write(const char* p, unsigned int n);

    void flush();

  private:
    /** \brief The stream we're writing. */
    stream_type& m_stream;

    /** \brief Pointer to the begining of the buffer. */
    char* const m_begin;

    /** \brief Pointer to the first invalid byte after the end of the
        buffer. */
    char* const m_end;

    /** \brief Pointer to the current not already read valid byte. */
    char* m_current;

  }; // class buffered_ostream
}

#include <claw/buffered_ostream.tpp>

#endif // __CLAW_BUFFERED_OSTREAM_HPP__
