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
 * \file basic_socket.hpp
 * \brief Common interface for claw::net::basic_socketbuf and
 *        claw::net::socket_server
 * \author Julien Jorge
 */
#ifndef __CLAW_BASIC_SOCKET_HPP__
#define __CLAW_BASIC_SOCKET_HPP__

#include <string>
#include <claw/socket_traits.hpp>

namespace claw
{
  namespace net
  {
    /**
     * \brief Common interface for claw::net::basic_socketbuf and
     *        claw::net::socket_server
     * \author Julien Jorge
     */
    class basic_socket
    {
    public:
      basic_socket();

      basic_socket* open();
      basic_socket* close();

      bool is_open() const;

    protected:
      /** \brief File descriptor. */
      socket_traits::descriptor m_descriptor;

    }; // class basic_socket
  } // namespace net
} // namespace claw
#endif // __CLAW_BASIC_SOCKET_HPP__
