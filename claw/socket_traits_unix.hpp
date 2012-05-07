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
 * \file socket_traits_unix.hpp
 * \brief Unix interface for using sockets.
 * \author Julien Jorge
 */
#ifndef __CLAW_SOCKET_TRAITS_UNIX_HPP__
#define __CLAW_SOCKET_TRAITS_UNIX_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

#include <claw/assert.hpp>

namespace claw
{
  /**
   * \brief Unix interface for using sockets.
   * \author Julien Jorge
   */
  class socket_traits_unix
  {
  public:
    /** \brief Type of the system description of the socket. */
    typedef int descriptor;

  public:
    /** \brief Invalid socket descriptor. */
    static const descriptor invalid_socket = -1;

  public:
    /*------------------------------------------------------------------------*/
    /**
     * \brief Initialize the use of the socket library.
     * \return true if the initialization is successful.
     */
    static bool init()
    {
      return true;
    } // socket_traits_unix::init()

    /*------------------------------------------------------------------------*/
    /**
     * \brief Close the socket library.
     * \return true if the operation is successful.
     */
    static bool release()
    {
      return true;
    } // socket_traits_unix::release()

    /*------------------------------------------------------------------------*/
    /**
     * \brief Open a socket.
     * \return The descriptor on the loaded socket.
     */
    static descriptor open()
    {
      descriptor fd = invalid_socket;

      fd = socket(AF_INET, SOCK_STREAM, 0);

      return fd;
    } // socket_traits_unix::open()

    /*------------------------------------------------------------------------*/
    /**
     * \brief Close a socket.
     * \param d The descriptor of the socket to close.
     * \return true if the socket has been closed.
     */
    static bool close( descriptor d )
    {
      return ::close(d) == 0; 
    } // socket_traits_unix::close()

    /*------------------------------------------------------------------------*/
    /**
     * \brief Connect a socket to a port.
     * \param d The descriptor of the socket to connect.
     * \param address The adress to connect to.
     * \param port The port to connect to.
     * \return true if the connection is available.
     */
    static bool connect( descriptor d, const std::string& address, int port )
    {
      CLAW_PRECOND( d != invalid_socket );

      bool result = false;
      struct hostent* hp = gethostbyname(address.c_str());

      if (hp)
	{
	  struct sockaddr_in sa;

	  memset (&sa, '\0', sizeof(sa));
	  sa.sin_family = hp->h_addrtype;
	  sa.sin_port = htons(port);
	  memcpy( &sa.sin_addr, hp->h_addr, hp->h_length );
      
	  if (::connect(d, (struct sockaddr*)&sa, (socklen_t)sizeof(sa)) != -1)
	    result = true;
	}

      return result;
    } // socket_traits_unix::connect()

    /*------------------------------------------------------------------------*/
    /**
     * \brief Open a socket for incoming connexions.
     * \param d The descriptor of the socket to open.
     * \param port The port to connect to.
     * \param queue_size The size of the queue for incoming connexions.
     * \return true if the socket has been opened.
     */
    static bool listen( descriptor d, int port, unsigned int queue_size )
    {
      CLAW_PRECOND( d != invalid_socket );

      struct sockaddr_in addr;
      
      memset (&addr, '\0', sizeof(addr));
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = htonl(INADDR_ANY);
      
      if ( bind(d, (struct sockaddr*)&addr, sizeof(addr)) != -1 )
	return ::listen(d, queue_size) != -1;
      else
	return false;
    } // socket_traits_unix::listen()

    /*------------------------------------------------------------------------*/
    /**
     * \brief Select a socket for reading.
     * \param d The descriptor of the socket to read.
     * \param time_limit Maximum of seconds to wait before considering there's
     *        nothing to read. If \a time_limit is negative, the method wait
     *        until there is something to read.
     * \return true if the socket is ready to be read.
     */
    static bool select_read( descriptor d, int time_limit = -1 )
    {
      CLAW_PRECOND( d != invalid_socket );

      struct timeval tv, *ptv;
      fd_set fds;

      if ( time_limit < 0 )
        ptv = NULL;
      else
        {
          tv.tv_sec  = time_limit;
          tv.tv_usec = 0;
          
          ptv = &tv;
        }

      FD_ZERO(&fds);
      FD_SET(d, &fds);

      select( d+1, &fds, NULL, NULL, ptv );

      return FD_ISSET( d, &fds );
    } // socket_traits_unix::select_read()

    /*------------------------------------------------------------------------*/
    /**
     * \brief Accept an incoming connexion.
     * \param d The descriptor of the socket to listen.
     * \return The descriptor of the incoming connexion.
     */
    static descriptor accept( descriptor d )
    {
      return ::accept( d, NULL, NULL );
    } // socket_traits_unix::accept()

    /*------------------------------------------------------------------------*/
    /**
     * \brief Tell if a descriptor is a valid socket descriptor.
     * \param d The descriptor to test.
     */
    static bool valid_descriptor( descriptor d )
    {
      return d != invalid_socket;
    } // socket_traits_unix::valid_descriptor()

    /*------------------------------------------------------------------------*/
    /**
     * \brief Tell if a descriptor is a opened socket.
     * \param d The descriptor to test.
     */
    static bool is_open( descriptor d )
    {
      struct stat buf;

      return fstat(d, &buf) == 0;
    } // socket_traits_unix::is_open()

  }; // class socket_traits_unix

  typedef socket_traits_unix socket_traits;
} // namespace claw

#endif // __CLAW_SOCKET_TRAITS_UNIX_HPP__
