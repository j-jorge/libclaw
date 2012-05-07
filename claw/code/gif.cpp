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
 * \file gif.cpp
 * \brief Implementation of the claw::graphic::gif class.
 * \author Julien Jorge
 */
#include "claw/gif.hpp"

#include <algorithm>
#include <claw/functional.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the file contains a global palette.
 */
bool claw::graphic::gif::screen_descriptor::has_global_color_table() const
{
  return (packed & 0x80) != 0;
} // gif::screen_descriptor::has_global_color_table()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the palette, if any.
 */
unsigned int claw::graphic::gif::screen_descriptor::color_palette_size() const
{
  if ( !has_global_color_table() )
    return 0;
  else
    return 1 << ((packed & 0x07) + 1);
} // gif::screen_descriptor::color_palette_size()




/*----------------------------------------------------------------------------*/
/**
 * \brief Get the disposal method of the frame.
 */
claw::graphic::gif::graphic_control_extension::disposal_method
claw::graphic::gif::graphic_control_extension::get_disposal_method() const
{
  switch( (packed & 0x1C) >> 2 )
    {
    case 0: return dispose_none;
    case 1: return dispose_do_not_dispose;
    case 2: return dispose_background;
    case 3: return dispose_previous;
    default:
      return dispose_previous;
    }
} // gif::graphic_control_extension::get_disposal_method()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the palette contains a transparent color.
 */
bool
claw::graphic::gif::graphic_control_extension::has_transparent_color() const
{
  return (packed & 0x01) != 0;
} // gif::graphic_control_extension::has_transparent_color()




/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the file contains a global palette.
 */
bool claw::graphic::gif::image_descriptor::has_color_table() const
{
  return (packed & 0x80) != 0;
} // gif::image_descriptor::has_color_table()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the data is interlaced.
 */
bool claw::graphic::gif::image_descriptor::is_interlaced() const
{
  return (packed & 0x40) != 0;
} // gif::image_descriptor::is_interlaced()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the palette, if any.
 */
unsigned int claw::graphic::gif::image_descriptor::color_palette_size() const
{
  if ( !has_color_table() )
    return 0;
  else
    return 1 << ((packed & 0x07) + 1);
} // gif::image_descriptor::color_palette_size()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
claw::graphic::gif::gif()
{

} // gif::gif() [copy constructor]

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that Image to copy from.
 */
claw::graphic::gif::gif( const gif& that )
  : image(that)
{
  frame_list::const_iterator it;

  for (it=that.m_frame.begin(); it!=that.m_frame.end(); ++it)
    m_frame.push_back( new frame(**it) );
} // gif::gif() [copy constructor]

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor. Load an image from a gif file.
 * \param f Gif file.
 */
claw::graphic::gif::gif( std::istream& f )
{
  reader(*this, m_frame, f);
} // gif::gif() [constructor, from file]

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
claw::graphic::gif::~gif()
{
  std::for_each
    ( m_frame.begin(), m_frame.end(), claw::delete_function<frame*>() );
  m_frame.clear();
} // gif::~gif()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment.
 * \param that The gif to copy from.
 */
claw::graphic::gif& claw::graphic::gif::operator=( const gif& that )
{
  gif tmp(that);
  std::swap(tmp, *this);
  return *this;
} // gif::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap the content of two gifs.
 * \param that The gif to swap with.
 */
void claw::graphic::gif::swap( gif& that )
{
  super::swap(that);
  std::swap(m_frame, that.m_frame);
} // gif::swap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the frame sequence.
 */
claw::graphic::gif::frame_iterator claw::graphic::gif::frame_begin()
{
  return frame_iterator(m_frame.begin());
} // gif::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the end of the frame sequence.
 */
claw::graphic::gif::frame_iterator claw::graphic::gif::frame_end()
{
  return frame_iterator(m_frame.end());
} // gif::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the frame sequence.
 */
claw::graphic::gif::const_frame_iterator claw::graphic::gif::frame_begin() const
{
  return const_frame_iterator(m_frame.begin());
} // gif::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the frame sequence.
 */
claw::graphic::gif::const_frame_iterator claw::graphic::gif::frame_end() const
{
  return const_frame_iterator(m_frame.end());
} // gif::end()




/*----------------------------------------------------------------------------*/
/**
 * \brief Swap the content of two gifs.
 * \param a The gif to swap with \a b.
 * \param b The gif to swap with \a a.
 */
void std::swap( claw::graphic::gif& a, claw::graphic::gif& b )
{
  a.swap(b);
} // swap()
