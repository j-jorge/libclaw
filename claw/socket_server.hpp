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
 * \file socket_server.hpp
 * \brief Class used to receive incoming connections.
 * \author Julien Jorge
 */
#ifndef __CLAW_SOCKET_SERVER_HPP__
#define __CLAW_SOCKET_SERVER_HPP__

#include <string>
#include <claw/basic_socket.hpp>
#include <claw/isocket_stream.hpp>
#include <claw/osocket_stream.hpp>
#include <claw/socket_stream.hpp>

namespace claw
{
  namespace net
  {
    /**
     * \brief Class used to receive incoming connections.
     * \author Julien Jorge
     */
    class socket_server : private basic_socket
    {
    public:
      socket_server();
      socket_server( int port, unsigned int queue_size = s_default_queue_size );

      socket_server* open( int port,
                           unsigned int queue_size = s_default_queue_size );
      socket_server* close();

      bool is_open() const;

      operator bool() const;

      template <typename SocketStream>
      void accept( SocketStream& sock, int time_limit );

      static bool init();
      static bool release();

    private:
      /** \brief Default queue size for calls to listen. */
      static const unsigned int s_default_queue_size = 16;

    }; // class socket_server
  } // namespace net
} // namespace claw

#include <claw/impl/socket_server.tpp>

#endif // __CLAW_SOCKET_SERVER_HPP__
