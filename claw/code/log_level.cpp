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
 * \file log_level.cpp
 * \brief Implementation of the claw::log_level class.
 * \author Julien Jorge
 */
#include <claw/log_level.hpp>

#include <claw/claw_gettext.hpp>

namespace claw
{
  log_level log_error( 0, "error: " );
  log_level log_warning( 1, "warning: " );
  log_level log_verbose( 15 );
} // namespace claw

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param lvl The value of this level.
 */
claw::log_level::log_level( int lvl )
  : m_level(lvl)
{

} // log_level::log_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param lvl The value of this level.
 * \param s The messages prefix.
 */
claw::log_level::log_level( int lvl, const std::string& s )
  : m_level(lvl), m_prefix(s)
{

} // log_level::log_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the level value.
 */
int claw::log_level::get() const
{
  return m_level;
} // log_level::get()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the prefix.
 */
std::string claw::log_level::get_string() const
{
  if (m_prefix.empty())
    return m_prefix;
  else
    return claw_gettext(m_prefix.c_str());
} // log_level::get_string()
