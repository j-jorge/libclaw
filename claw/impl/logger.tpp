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
 * \file logger.tpp
 * \brief Template methods for the claw::log_system class.
 * \author Julien Jorge
 */
#include <sstream>

/*---------------------------------------------------------------------------*/
/**
 * \brief Log something.
 * \param t The thing to log...
 * \remark T must support operator<<(std::ostream&, const T&);
 */
template<class T>
claw::log_system& claw::log_system::operator<<( const T& t )
{
  if (m_message_level <= m_log_level)
    {
      std::ostringstream oss;
      oss << t;

      typename stream_list_type::iterator it;

      for (it = m_stream.begin(); it!=m_stream.end(); ++it)
	(*it)->write(oss.str());
    }

  return *this;
} // log_system::operator<<()
