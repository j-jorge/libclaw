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
 * \file gif_frame.cpp
 * \brief Implementation of the claw::graphic::gif::frame class.
 * \author Julien Jorge
 */
#include <claw/graphic/gif.hpp>

/**
 * \brief Constructor.
 */
claw::graphic::gif::frame::frame()
  : m_delay(0)
{}

/**
 * \brief Constructor.
 * \param w The width of the frame.
 * \param h The height of the frame.
 */
claw::graphic::gif::frame::frame(std::size_t w, std::size_t h)
  : super(w, h)
  , m_delay(0)
{}

/**
 * \brief Set the time duration of this frame.
 * \param d The delay.
 */
void claw::graphic::gif::frame::set_delay(unsigned int d)
{
  m_delay = d;
}

/**
 * \brief Get the time duration of this frame.
 */
unsigned int claw::graphic::gif::frame::get_delay() const
{
  return m_delay;
}
