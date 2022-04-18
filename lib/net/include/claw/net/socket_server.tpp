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
 * \file socket_server.tpp
 * \brief Implantation of the template methods of the claw::net::socket_server
 *        class.
 * \author Julien Jorge
 */

/**
 * \brief Accept an incoming connection.
 * \param sock (out) The socket created for this connection.
 * \param time_limit Maximum of seconds to wait before considering there won't
 *        be any connection. if \a time_limit is negative, the server wait
 * until there is a connection.
 */
template <typename SocketStream>
void claw::net::socket_server::accept(SocketStream& sock, int time_limit)
{
  if(socket_traits::select_read(m_descriptor, time_limit))
    sock.open(socket_traits::accept(m_descriptor));
}
