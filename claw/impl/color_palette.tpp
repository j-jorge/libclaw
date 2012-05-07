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
 * \file color_palette.tpp
 * \brief Implementation of the claw::graphic::color_palette class.
 * \author Julien Jorge
 */
#include <cassert>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param n The size of the palette.
 */
template< typename Color >
claw::graphic::color_palette<Color>::color_palette(unsigned int n)
  : m_palette(n)
{

} // color_palette::color_palette()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of colors in the palette.
 */
template< typename Color >
unsigned int claw::graphic::color_palette<Color>::size() const
{
  return m_palette.size();
} // color_palette::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a color from the palette.
 * \param i The index of the color.
 * \pre \a i is a valid color index.
 */
template< typename Color >
typename claw::graphic::color_palette<Color>::color_type&
claw::graphic::color_palette<Color>::operator[]( unsigned int i )
{
  assert( i < m_palette.size() );

  return m_palette[i];
} // color_palette::operator[]()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of a color from the palette.
 * \param i The index of the color.
 * \pre \a i is a valid color index.
 */
template< typename Color >
const typename claw::graphic::color_palette<Color>::color_type&
claw::graphic::color_palette<Color>::operator[]( unsigned int i ) const
{
  assert( i < m_palette.size() );

  return m_palette[i];
} // color_palette::operator[]() [const]

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of a color from the palette.
 * \param i The index of the color.
 * \pre \a i is a valid color index.
 */
template< typename Color >
const typename claw::graphic::color_palette<Color>::color_type&
claw::graphic::color_palette<Color>::operator()( unsigned int i ) const
{
  assert( i < m_palette.size() );

  return m_palette[i];
} // color_palette::operator()()

