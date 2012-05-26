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
 * \file isocket_stream.hpp
 * \brief A class to use any socket as a classic standard input stream.
 * \author Julien Jorge
 */
#ifndef __CLAW_ISOCKET_STREAM_HPP__
#define __CLAW_ISOCKET_STREAM_HPP__

#include <claw/basic_socketbuf.hpp>
#include <istream>

namespace claw
{
  namespace net
  {
    /**
     * \brief A class to use any socket as a classic standard input stream.
     * \author Julien Jorge
     */
    template< typename CharT, typename Traits = std::char_traits<CharT> >
    class basic_isocket_stream : public std::basic_istream<CharT, Traits>
    {
    public:
      /** \brief The type of the characters in the stream. */
      typedef CharT                          char_type;

      /** \brief The traits carry some informations about the char_type. */
      typedef Traits                         traits_type;

      /** \brief The type used to represent integers. */
      typedef typename traits_type::int_type int_type;

      /** \brief The type used to represent a position in the stream. */
      typedef typename traits_type::pos_type pos_type;

      /** \brief The type used to represent a delta in the positions. */
      typedef typename traits_type::off_type off_type;

      /** \brief The type of the buffer manipulated by this stream. */
      typedef basic_socketbuf<char_type, traits_type> buffer_type;

    private:
      /** \brief The type of the parent class. */
      typedef std::basic_istream<CharT, Traits> super;

    public:
      basic_isocket_stream();
      basic_isocket_stream( const std::string& address, int port );

      ~basic_isocket_stream();

      buffer_type* rdbuf() const;

      bool is_open() const;

      void open( const std::string& address, int port );
      void open( int fd );
      void close();

    private:
      /** \brief The buffer used for input operations. */
      buffer_type m_buffer;

    }; // class basic_isocket_stream

    /** \brief Implementation of the basic_isocket_stream for the char type. */
    typedef basic_isocket_stream<char> isocket_stream;

    /** \brief Implementation of the basic_isocket_stream for the wide
        characters. */
    typedef basic_isocket_stream<wchar_t> wisocket_stream;

  } // namespace net
} // namespace claw

#include <claw/impl/isocket_stream.tpp>

#endif // __CLAW_ISOCKET_STREAM_HPP__
