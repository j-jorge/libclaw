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
 * \file bitmap_writer.cpp
 * \brief Implementation of the claw::graphic::bitmap::writer class.
 * \author Julien Jorge
 */
#include <claw/graphic/bitmap.hpp>

#include <algorithm>

/**
 * \brief Constructor.
 * \param img The image to save.
 */
claw::graphic::bitmap::writer::writer(const image& img)
  : m_image(img)
{}

/**
 * \brief Constructor.
 * \param img The image to save.
 * \param f The file in which we save the data.
 */
claw::graphic::bitmap::writer::writer(const image& img, std::ostream& f)
  : m_image(img)
{
  save(f);
}

/**
 * \brief Save the bitmap in a file.
 * \param f Destination file.
 */
void claw::graphic::bitmap::writer::save(std::ostream& f) const
{
  header h;

  init_header(h);

  f.write(reinterpret_cast<char*>(&h), sizeof(header));

  save_data(f);
}

/**
 * \brief Saves a 24 bpp bitmap file.
 * \param f Bitmap file.
 */
void claw::graphic::bitmap::writer::save_data(std::ostream& f) const
{
  unsigned int line;
  unsigned int buffer_size = m_image.width() * 3;

  // lines are 4-bytes aligned, so adjust buffer's size.
  if(buffer_size % 4 != 0)
    buffer_size += 4 - buffer_size % 4;

  char* buffer = new char[buffer_size];

  for(line = m_image.height(); line > 0;)
    {
      --line;
      pixel32_to_pixel24(buffer, m_image[line]);
      f.write(buffer, buffer_size);
    }

  delete[] buffer;
}

/**
 * \brief Converts a pixel32 scanline to a BGR array.
 * \param dest (out) Filled array.
 * \param src Scanline to convert.
 */
void claw::graphic::bitmap::writer::pixel32_to_pixel24(
    char* dest, const scanline& src) const
{
  unsigned int i24 = 0;
  scanline::const_iterator first(src.begin());
  scanline::const_iterator last(src.end());

  for(; first != last; ++first)
    {
      dest[i24++] = first->components.blue;
      dest[i24++] = first->components.green;
      dest[i24++] = first->components.red;
    }
}

/**
 * \brief Initialize header's data, for saving.
 * \param h Header to initialize.
 */
void claw::graphic::bitmap::writer::init_header(header& h) const
{
  unsigned int adjusted_line = m_image.width() * 3;

  if(m_image.width() % 4 != 0)
    adjusted_line += 4 - m_image.width() % 4;

  // for a 24 bpp bitmap.
  h.id[0] = 'B';
  h.id[1] = 'M';
  h.file_size = adjusted_line * m_image.height() + sizeof(h);
  h.nop = 0;

  // there is no color pallet, so data is just after the h
  h.data_offset = sizeof(h);
  // default value for Windows' bitmaps.
  h.header_size = 0x28;

  h.width = m_image.width();
  h.height = m_image.height();
  h.layers = 1;
  h.bpp = 24;
  h.compression = BMP_COMPRESSION_RGB;
  h.image_size = adjusted_line * m_image.height();
  h.ppm_x = 0x2E23; // 11811
  h.ppm_y = 0x2E23;
  h.colors_count = 0;
  h.importants_colors = 0;
}
