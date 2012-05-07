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
 * \file socket_stream.hpp
 * \brief A class to use any socket as a classic standard stream.
 * \author Julien Jorge
 */
#ifndef __CLAW_SOCKET_STREAM_HPP__
#define __CLAW_SOCKET_STREAM_HPP__

#include <claw/basic_socketbuf.hpp>
#include <iostream>

namespace claw
{
  namespace net
  {
    /**
     * \brief A class to use any socket as a classic standard stream.
     * \author Julien Jorge
     */
    template< typename CharT, typename Traits = std::char_traits<CharT> >
    class basic_socket_stream : public std::basic_iostream<CharT, Traits>
    {
    public:
      typedef CharT                          char_type;
      typedef Traits                         traits_type;
      typedef typename traits_type::int_type int_type;
      typedef typename traits_type::pos_type pos_type;
      typedef typename traits_type::off_type off_type;

      typedef basic_socketbuf<char_type, traits_type> buffer_type;

    public:
      explicit basic_socket_stream( int read_delay = -1 );
      basic_socket_stream( const char* address, int port, int read_delay = -1 );

      ~basic_socket_stream();

      buffer_type* rdbuf() const;

      bool is_open() const;
      void set_read_time_limit( int read_limit );

      void open( const char* address, int port );
      void open( int fd );
      void close();

    private:
      /** \brief The buffer used for operations. */
      buffer_type m_buffer;
    }; // class basic_socket_stream

    typedef basic_socket_stream<char> socket_stream;
    typedef basic_socket_stream<wchar_t> wsocket_stream;
  } // namespace net
} // namespace claw

#include <claw/impl/socket_stream.tpp>

#endif // __CLAW_SOCKET_STREAM_HPP__
