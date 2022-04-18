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
 * \file basic_socket.cpp
 * \brief Implementation of the claw::net::basic_socket class.
 * \author Julien Jorge
 */
#include <claw/net/basic_socket.hpp>

/**
 * \brief Constructor.
 * \post is_open() == false
 */
claw::net::basic_socket::basic_socket()
  : m_descriptor(socket_traits::invalid_socket)
{}

/**
 * \brief Open the socket.
 * \return this is everything works fine, NULL otherwise.
 */
claw::net::basic_socket* claw::net::basic_socket::open()
{
  basic_socket* result = NULL;

  m_descriptor = socket_traits::open();

  if(socket_traits::valid_descriptor(m_descriptor))
    result = this;

  return result;
}

/**
 * \brief Close the socket.
 * \return this if everything works fine, NULL otherwise.
 */
claw::net::basic_socket* claw::net::basic_socket::close()
{
  basic_socket* result = this;

  if(is_open())
    {
      if(socket_traits::close(m_descriptor))
        m_descriptor = socket_traits::invalid_socket;
      else
        result = NULL;
    }

  return result;
}

/**
 * \brief Tell if the socket is open.
 */
bool claw::net::basic_socket::is_open() const
{
  return socket_traits::valid_descriptor(m_descriptor);
}
