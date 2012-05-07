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
 * \file exception.hpp
 * \brief A simple class to use as exception with string message.
 * \author Julien Jorge
 */
#ifndef __CLAW_EXCEPTION_HPP__
#define __CLAW_EXCEPTION_HPP__

#include <exception>
#include <string>

namespace claw
{
  /**
   * \brief A simple class to use as exception with string message.
   * \author Julien Jorge
   */
  class exception:
    public std::exception
  {
  public:
    /**
     * \brief Constructor.
     * \param msg A short description of the problem.
     */
    exception( const std::string& msg ) throw()
      : m_msg(msg)
    { }

    /** \brief Desctructor. */
    ~exception() throw() {}

    /** \brief Get a short description of the problem. */
    const char* what() const throw() { return m_msg.c_str(); }

  private:
    /** A short description of the problem. */
    const std::string m_msg;

  }; // class exception
  
  /**
   * \brief Exception thrown when accessing bad formated data.
   * \author Julien Jorge
   */
  class bad_format:
    public exception
  {
  public:
    /**
     * \brief Constructor.
     * \param msg A short description of the problem.
     */
    bad_format( const std::string& msg ) throw()
      : exception(msg)
    { }
  }; // class bad_format

} // namespace claw

/**
 * \brief Create an exception and add the name of the current function to the
 *        message.
 * \param m A short description of the problem.
 */
#define CLAW_EXCEPTION( m )                                     \
  claw::exception( std::string(__FUNCTION__) + ": " + m )

#endif // __CLAW_EXCEPTION_HPP__
