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
 * \file bitmap.cpp
 * \brief Implementation of the bitmap class.
 * \author Julien Jorge
 */
#include <claw/graphic/bitmap.hpp>

/**
 * \brief Constructor. Creates an empty image.
 * \param w Image's width.
 * \param h Image's height.
 * \pre w > 0 and h > 0
 */
claw::graphic::bitmap::bitmap(unsigned int w, unsigned int h)
  : claw::graphic::image(w, h)
{}

/**
 * \brief Copy constructor.
 * \param that Image to copy from.
 */
claw::graphic::bitmap::bitmap(const image& that)
  : claw::graphic::image(that)
{}

/**
 * \brief Constructor. Load an image from a bitmap file.
 * \param f Bitmap file.
 */
claw::graphic::bitmap::bitmap(std::istream& f)
{
  reader r(*this, f);
}

/**
 * \brief Save the bitmap in a file.
 * \param f Destination file.
 */
void claw::graphic::bitmap::save(std::ostream& f) const
{
  writer w(*this, f);
}
