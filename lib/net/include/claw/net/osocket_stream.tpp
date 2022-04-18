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
 * \file osocket_stream.tpp
 * \brief Implementation of the claw::net::basic_osocket_stream class.
 * \author Julien Jorge
 */

/**
 * \brief Constructor.
 */
template <typename CharT, typename Traits>
claw::net::basic_osocket_stream<CharT, Traits>::basic_osocket_stream()
  : super(&m_buffer)
{}

/**
 * \brief Constructor.
 * \param address The address to which we will connect.
 * \param port The port number to use for the connection.
 */
template <typename CharT, typename Traits>
claw::net::basic_osocket_stream<CharT, Traits>::basic_osocket_stream(
    const std::string& address, int port)
  : super(&m_buffer)
{
  open(address, port);
}

/**
 * \brief Destructor.
 */
template <typename CharT, typename Traits>
claw::net::basic_osocket_stream<CharT, Traits>::~basic_osocket_stream()
{
  // nothing to do
}

/**
 * \brief Get the output buffer.
 */
template <typename CharT, typename Traits>
typename claw::net::basic_osocket_stream<CharT, Traits>::buffer_type*
claw::net::basic_osocket_stream<CharT, Traits>::rdbuf() const
{
  return const_cast<buffer_type*>(&m_buffer);
}

/**
 * \brief Tell if the stream is open.
 */
template <typename CharT, typename Traits>
bool claw::net::basic_osocket_stream<CharT, Traits>::is_open() const
{
  return m_buffer.is_open();
}

/**
 * \brief Connect the socket to an address.
 * \param address The address to which we will connect.
 * \param port The port number to use for the connection.
 */
template <typename CharT, typename Traits>
void claw::net::basic_osocket_stream<CharT, Traits>::open(
    const std::string& address, int port)
{
  if(!m_buffer.open(address, port))
    this->setstate(std::ios_base::failbit);
  else
    this->clear();
}

/**
 * \brief Link the socket to a file descriptor.
 * \param fd The file descriptor.
 * \remark This method should be only called by claw::net::socket_server.
 */
template <typename CharT, typename Traits>
void claw::net::basic_osocket_stream<CharT, Traits>::open(int fd)
{
  if(!m_buffer.open(fd))
    this->setstate(std::ios_base::failbit);
  else
    this->clear();
}

/**
 * \brief Close the connection.
 */
template <typename CharT, typename Traits>
void claw::net::basic_osocket_stream<CharT, Traits>::close()
{
  if(!m_buffer.close())
    this->setstate(std::ios_base::failbit);
}
