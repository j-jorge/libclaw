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
 * \file socket_stream.tpp
 * \brief Implementation of the claw::net::basic_socket_stream class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param read_delay Number of second to wait before considering nothing will
 *        come in the socket. Negative values mean infinity.
 */
template<typename CharT, typename Traits>
claw::net::basic_socket_stream<CharT, Traits>::basic_socket_stream
( int read_delay )
  : m_buffer(read_delay)
{
  this->init(&m_buffer);
} // basic_socket_stream::basic_socket_stream()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param address The address to which we will connect.
 * \param port The port number to use for the connection.
 * \param read_delay Number of second to wait before considering nothing will
 *        come in the socket. Negative values mean infinity.
 */
template<typename CharT, typename Traits>
claw::net::basic_socket_stream<CharT, Traits>::basic_socket_stream
( const char* address, int port, int read_delay )
  : m_buffer(read_delay)
{
  this->init(&m_buffer);
  open(address, port);
} // basic_socket_stream::basic_socket_stream()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
template<typename CharT, typename Traits>
claw::net::basic_socket_stream<CharT, Traits>::~basic_socket_stream()
{
  // nothing to do
} // basic_socket_stream::~basic_socket_stream()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the buffer.
 */
template<typename CharT, typename Traits>
typename claw::net::basic_socket_stream<CharT, Traits>::buffer_type*
claw::net::basic_socket_stream<CharT, Traits>::rdbuf() const
{
  return const_cast<buffer_type*>(&m_buffer);
} // basic_socket_stream::rdbuf()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the stream is open.
 */
template<typename CharT, typename Traits>
bool claw::net::basic_socket_stream<CharT, Traits>::is_open() const
{
  return m_buffer.is_open();
} // basic_socket_stream::is_open()

/*----------------------------------------------------------------------------*/
/** 
 * \brief Set the number of second to wait before considering nothing will come
 *        in the socket.
 * \param read_limit The number of seconds. Negative values mean infinity.
 */
template<typename CharT, typename Traits>
void claw::net::basic_socket_stream<CharT, Traits>::set_read_time_limit
( int read_limit )
{
  m_buffer.set_read_time_limit(read_limit);
} // // basic_socket_stream::set_read_time_limit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Connect the socket to an address.
 * \param address The address to which we will connect.
 * \param port The port number to use for the connection.
 */
template<typename CharT, typename Traits>
void claw::net::basic_socket_stream<CharT, Traits>::open
( const char* address, int port )
{
  if ( !m_buffer.open(address, port) )
    this->setstate(std::ios_base::failbit);
  else
    this->clear();
} // basic_socket_stream::open()

/*----------------------------------------------------------------------------*/
/**
 * \brief Link the socket to a file descriptor.
 * \param fd The file descriptor.
 * \remark This method should be only called by claw::net::socket_server.
 */
template<typename CharT, typename Traits>
void claw::net::basic_socket_stream<CharT, Traits>::open( int fd )
{
  if ( !m_buffer.open(fd) )
    this->setstate(std::ios_base::failbit);
  else
    this->clear();
} // basic_socket_stream::open()

/*----------------------------------------------------------------------------*/
/**
 * \brief Close the connection.
 */
template<typename CharT, typename Traits>
void claw::net::basic_socket_stream<CharT, Traits>::close()
{
  if ( !m_buffer.close() )
    this->setstate(std::ios_base::failbit);
} // basic_socket_stream::close()

