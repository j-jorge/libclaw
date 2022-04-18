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
 * \file socket_server.cpp
 * \brief Implantation of the claw::net::socket_server class.
 * \author Julien Jorge
 */
#include <claw/net/socket_server.hpp>

#include <sys/types.h>

/**
 * \brief Constructor.
 * \post is_open() == false
 */
claw::net::socket_server::socket_server()
{
  // nothing to do
}

/**
 * \brief Constructor, open the socket.
 * \param port The port to bind.
 * \param queue_size The size of the waiting queue for incoming connections.
 */
claw::net::socket_server::socket_server(int port, unsigned int queue_size)
{
  open(port, queue_size);
}

/**
 * \brief Open the socket.
 * \param port The port to bind.
 * \param queue_size The size of the waiting queue for incoming connections.
 * \return this if everything works fine, NULL otherwise.
 */
claw::net::socket_server*
claw::net::socket_server::open(int port, unsigned int queue_size)
{
  socket_server* result = NULL;

  if(basic_socket::open())
    {
      if(!socket_traits::listen(m_descriptor, port, queue_size))
        basic_socket::close();
      else
        result = this;
    }

  return result;
}

/**
 * \brief Close the socket.
 */
claw::net::socket_server* claw::net::socket_server::close()
{
  if(basic_socket::close())
    return this;
  else
    return NULL;
}

/**
 * \brief Tell if the server is open.
 */
bool claw::net::socket_server::is_open() const
{
  return basic_socket::is_open();
}

/**
 * \brief Tell if the server is working.
 */
claw::net::socket_server::operator bool() const
{
  return is_open();
}

/**
 * \brief Initialize the use of the socket library.
 * \return true if the initialization is successful.
 */
bool claw::net::socket_server::init()
{
  return socket_traits::init();
}

/**
 * \brief Close the socket library.
 * \return true if the operation is successful.
 */
bool claw::net::socket_server::release()
{
  return socket_traits::release();
}
