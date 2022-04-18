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
 * \file basic_socketbuf.tpp
 * \brief Implantation of the claw::net::basic_socketbuf class.
 * \author Julien Jorge
 */
#include <claw/assert.hpp>

template <typename CharT, typename Traits>
const size_t claw::net::basic_socketbuf<CharT, Traits>::s_buffer_size = 256;

/**
 * \brief Constructor.
 * \param read_limit Number of second to wait before considering nothing will
 *        come in the socket. Negative values mean infinity.
 * \post is_open() == false;
 */
template <typename CharT, typename Traits>
claw::net::basic_socketbuf<CharT, Traits>::basic_socketbuf(int read_limit)
  : m_read_limit(read_limit)
  , m_input_buffer(NULL)
  , m_input_buffer_size(0)
  , m_output_buffer(NULL)
  , m_output_buffer_size(0)
{
  create_buffers();
}

/**
 * \brief Destructor.
 */
template <typename CharT, typename Traits>
claw::net::basic_socketbuf<CharT, Traits>::~basic_socketbuf()
{
  close();
  destroy_buffers();
}

/**
 * \brief Initialise the socket.
 * \param address Address to open.
 * \param port The port to connect to.
 * \return this if everything works fine, NULL otherwise.
 */
template <typename CharT, typename Traits>
typename claw::net::basic_socketbuf<CharT, Traits>::self_type*
claw::net::basic_socketbuf<CharT, Traits>::open(const std::string& address,
                                                int port)
{
  self_type* result = NULL;

  if(!this->is_open())
    if(basic_socket::open())
      {
        if(connect(address, port))
          result = this;
        else
          close();
      }

  return result;
}

/**
 * \brief Link the socket to a file descriptor.
 * \param d The file descriptor.
 * \return this if everything works fine, NULL otherwise.
 * \remark This method should be only called by
 * claw::net::basic_*socket_stream. \remark If this socket was open and \a fd
 * is valid, the socket will be closed before that \a d will be assigned.
 */
template <typename CharT, typename Traits>
typename claw::net::basic_socketbuf<CharT, Traits>::self_type*
claw::net::basic_socketbuf<CharT, Traits>::open(socket_traits::descriptor d)
{
  self_type* result = NULL;

  if(socket_traits::is_open(d))
    {
      if(this->is_open())
        {
          if(close())
            {
              result = this;
              m_descriptor = d;
            }
        }
      else
        {
          result = this;
          m_descriptor = d;
        }
    }

  return result;
}

/**
 * \brief Close the socket.
 * \post is_open() == false;
 */
template <typename CharT, typename Traits>
typename claw::net::basic_socketbuf<CharT, Traits>::self_type*
claw::net::basic_socketbuf<CharT, Traits>::close()
{
  if(basic_socket::close())
    return this;
  else
    return NULL;
}

/**
 * \brief Tell if the socket is open.
 */
template <typename CharT, typename Traits>
bool claw::net::basic_socketbuf<CharT, Traits>::is_open() const
{
  return basic_socket::is_open();
}

/**
 * \brief Set the number of second to wait before considering nothing will come
 *        in the socket.
 * \param read_limit The number of seconds. Negative values mean infinity.
 */
template <typename CharT, typename Traits>
void claw::net::basic_socketbuf<CharT, Traits>::set_read_time_limit(
    int read_limit)
{
  m_read_limit = read_limit;
}

/**
 * \brief Write the buffered data in the socket.
 * \pre is_open()
 */
template <typename CharT, typename Traits>
int claw::net::basic_socketbuf<CharT, Traits>::sync()
{
  CLAW_PRECOND(is_open());
  CLAW_PRECOND(buffered());

  ssize_t write_count = 0;
  ssize_t length = (this->pptr() - this->pbase()) * sizeof(char_type);
  int_type result = 0;

  if(length > 0)
    write_count = send(m_descriptor, static_cast<const char*>(this->pbase()),
                       length, 0);

  if(write_count >= 0)
    this->setp(m_output_buffer, m_output_buffer + m_output_buffer_size);
  else
    result = -1;

  return result;
}

/**
 * \brief Fill the input buffer.
 * \pre is_open() && gptr() != NULL
 * \return The next symbol to be read ( *gptr() ) if there is data coming from
 *         the socket, traits::eof() otherwise.
 */
template <typename CharT, typename Traits>
typename claw::net::basic_socketbuf<CharT, Traits>::int_type
claw::net::basic_socketbuf<CharT, Traits>::underflow()
{
  CLAW_PRECOND(buffered());
  CLAW_PRECOND(this->gptr() >= this->egptr());

  ssize_t read_count;
  ssize_t length = m_input_buffer_size * sizeof(char_type);
  int_type result = traits_type::eof();

  if(!is_open())
    return result;

  if(socket_traits::select_read(m_descriptor, m_read_limit))
    read_count
        = recv(m_descriptor, static_cast<char*>(m_input_buffer), length, 0);
  else
    read_count = -1;

  if(read_count > 0)
    {
      this->setg(m_input_buffer, m_input_buffer, m_input_buffer + read_count);
      result = this->sgetc();
    }
  else
    this->setg(m_input_buffer, m_input_buffer + m_input_buffer_size,
               m_input_buffer + m_input_buffer_size);

  return result;
}

/**
 * \brief Synchronize the output buffer (ie. write in the socket).
 * \param c
 */
template <typename CharT, typename Traits>
typename claw::net::basic_socketbuf<CharT, Traits>::int_type
claw::net::basic_socketbuf<CharT, Traits>::overflow(int_type c)
{
  CLAW_PRECOND(is_open());
  CLAW_PRECOND(buffered());

  int_type result = traits_type::eof();

  if(sync() == 0)
    {
      result = traits_type::not_eof(c);

      if(!traits_type::eq_int_type(c, traits_type::eof()))
        this->sputc(c);
    }

  return result;
}

/**
 * \brief Connect the socket to a port.
 * \param addr The address to connect to.
 * \param port The port to connect to.
 * \return true if the connection is established.
 * \pre m_fd is a valid socket descriptor.
 */
template <typename CharT, typename Traits>
bool claw::net::basic_socketbuf<CharT, Traits>::connect(
    const std::string& addr, int port)
{
  CLAW_PRECOND(socket_traits::valid_descriptor(m_descriptor));

  return socket_traits::connect(m_descriptor, addr, port);
}

/**
 * \brief Create the buffers.
 * \pre pbase() == eback() == NULL
 */
template <typename CharT, typename Traits>
void claw::net::basic_socketbuf<CharT, Traits>::create_buffers()
{
  CLAW_PRECOND(this->pbase() == NULL);
  CLAW_PRECOND(this->eback() == NULL);

  m_input_buffer_size = m_output_buffer_size = s_buffer_size;

  m_input_buffer = new char_type[m_input_buffer_size];
  m_output_buffer = new char_type[m_output_buffer_size];

  this->setg(m_input_buffer, m_input_buffer + m_input_buffer_size,
             m_input_buffer + m_input_buffer_size);
  this->setp(m_output_buffer, m_output_buffer + m_output_buffer_size);
}

/**
 * \brief Destroy the buffers.
 * \post pbase() == eback() == NULL
 */
template <typename CharT, typename Traits>
void claw::net::basic_socketbuf<CharT, Traits>::destroy_buffers()
{
  if(m_input_buffer)
    {
      delete[] m_input_buffer;
      m_input_buffer = NULL;
    }

  if(m_output_buffer)
    {
      delete[] m_output_buffer;
      m_output_buffer = NULL;
    }

  this->setg(NULL, NULL, NULL);
  this->setp(NULL, NULL);
}

/**
 * \brief Tell if we use buffered input and output.
 * \remark Should always be true !
 */
template <typename CharT, typename Traits>
bool claw::net::basic_socketbuf<CharT, Traits>::buffered() const
{
  return this->pbase() && this->pptr() && this->epptr() && this->eback()
         && this->gptr() && this->egptr();
}
