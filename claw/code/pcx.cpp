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
 * \file pcx.cpp
 * \brief Implementation of the pcx class.
 * \author Julien Jorge
 */
#include <claw/pcx.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor. Creates an empty image.
 * \param w Image's width.
 * \param h Image's height.
 * \pre w > 0 and h > 0
 */
claw::graphic::pcx::pcx( unsigned int w, unsigned int h )
  : claw::graphic::image(w, h)
{

} // pcx::pcx() [constructor]

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that Image to copy from.
 */
claw::graphic::pcx::pcx( const image& that )
  : claw::graphic::image(that)
{

} // pcx::pcx() [copy constructor]

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor. Load an image from a pcx file.
 * \param f Pcx file.
 */
claw::graphic::pcx::pcx( std::istream& f )
{
  reader(*this, f);
} // pcx::pcx() [constructor, from file]

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the content of the image in a stream.
 * \param os The stream in which we write.
 */
void claw::graphic::pcx::save( std::ostream& os ) const
{
  writer(*this, os);
} // pcx::save()
