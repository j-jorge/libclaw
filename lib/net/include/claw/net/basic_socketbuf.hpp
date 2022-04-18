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
 * \file basic_socketbuf.hpp
 * \brief Socket buffer to be used with std::basic_socket_stream, for easy
 *        socket reading and writing.
 * \author Julien Jorge
 */
#ifndef __CLAW_BASIC_SOCKETBUF_HPP__
#define __CLAW_BASIC_SOCKETBUF_HPP__

#include <claw/net/basic_socket.hpp>

#include <streambuf>
#include <string>

namespace claw
{
  namespace net
  {
    /**
     * \brief Socket buffer to be used with std::basic_stream, for easy socket
     *        reading and writing.
     * \author Julien Jorge
     */
    template <typename CharT, typename Traits>
    class basic_socketbuf
      : private basic_socket
      , public std::basic_streambuf<CharT, Traits>
    {
    public:
      /** \brief The type of the characters in the stream. */
      typedef CharT char_type;

      /** \brief The traits carry some informations about the char_type. */
      typedef Traits traits_type;

      /** \brief The type used to represent integers. */
      typedef typename traits_type::int_type int_type;

      /** \brief The type used to represent a position in the stream. */
      typedef typename traits_type::pos_type pos_type;

      /** \brief The type used to represent a delta in the positions. */
      typedef typename traits_type::off_type off_type;

      /** \brief The type of this implementation. */
      typedef basic_socketbuf<char_type, traits_type> self_type;

    public:
      basic_socketbuf(int read_limit = -1);
      virtual ~basic_socketbuf();

      self_type* open(const std::string& addr, int port);
      self_type* open(socket_traits::descriptor d);
      self_type* close();

      bool is_open() const;

      void set_read_time_limit(int read_limit);

    protected:
      virtual int sync();
      virtual int_type underflow();
      virtual int_type overflow(int_type c = traits_type::eof());

    private:
      bool connect(const std::string& addr, int port);

      void create_buffers();
      void destroy_buffers();

      bool buffered() const;

    private:
      /** \brief Number of second to wait before considering nothing will come
          in the socket. Negative values mean infinity. */
      int m_read_limit;

      /** \brief Input buffer. */
      char_type* m_input_buffer;

      /** \brief Size of the input buffer. */
      size_t m_input_buffer_size;

      /** \brief Output buffer. */
      char_type* m_output_buffer;

      /** \brief Size of the output buffer. */
      size_t m_output_buffer_size;

      /** \brief Default size for the buffers. */
      static const size_t s_buffer_size;

    }; // class basic_socketbuf
  }
}

#include <claw/net/basic_socketbuf.tpp>

#endif // __CLAW_BASIC_SOCKETBUF_HPP__
