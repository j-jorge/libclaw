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
 * \file bitmap_reader.tpp
 * \brief Template classes of the claw::graphic::bitmap::reader class.
 * \author Julien Jorge
 */
#include <algorithm>
#include <cassert>
#include <claw/exception.hpp>

/**
 * \brief Constructor.
 * \param palette The color palette to convert the pixels.
 * \param img The image we're filling.
 */
template <bool Coded4Bits>
claw::graphic::bitmap::reader::rle_bitmap_output_buffer<
    Coded4Bits>::rle_bitmap_output_buffer(const color_palette_type& palette,
                                          image& img)
  : m_palette(palette)
  , m_image(img)
  , m_x(0)
  , m_y(m_image.height() - 1)
{}

/**
 * \brief Go to the begining of the next line to fill.
 */
template <bool Coded4Bits>
void claw::graphic::bitmap::reader::rle_bitmap_output_buffer<
    Coded4Bits>::next_line()
{
  assert(m_y > 0);

  m_x = 0;
  --m_y;
}

/**
 * \brief Move the cursor horizontally and vertically.
 * \param x Horizontal displacement.
 * \param y Vertical displacement.
 */
template <bool Coded4Bits>
void claw::graphic::bitmap::reader::rle_bitmap_output_buffer<
    Coded4Bits>::delta_move(unsigned char x, unsigned char y)
{
  assert(m_x + x < m_image.width());
  assert(m_y + y < m_image.height());

  m_x += x;
  m_y += y;
}

//******************************************************************************

/**
 * \brief Get the type of the following data in the input buffer, eventually
 *        apply the special codes.
 * \param input The input stream (the bitmap file).
 * \param output The output stream (the bitmap image).
 */
template <typename OutputBuffer>
void claw::graphic::bitmap::reader::rle_bitmap_decoder<
    OutputBuffer>::read_mode(file_input_buffer& input,
                             output_buffer_type& output)
{
  this->m_mode = this->stop;
  bool ok = true;

  if(input.remaining() < 2)
    ok = input.read_more(2);

  if(ok)
    {
      unsigned char key, pattern;

      key = input.get_next();
      pattern = input.get_next();

      // compressed data, next byte is the pattern
      if(key > 0)
        {
          this->m_mode = this->compressed;
          this->m_count = key;
          this->m_pattern = pattern;
        }
      else
        switch(pattern)
          {
            // end of line
          case 0:
            output.next_line();
            read_mode(input, output);
            break;
            // end of file
          case 1:
            this->m_mode = this->stop;
            break;
            // delta move
          case 2:
            {
              if(input.remaining() < 1)
                ok = input.read_more(1);

              if(ok)
                {
                  unsigned char x, y;
                  x = pattern;
                  y = input.get_next();
                  output.delta_move(x, y);
                  read_mode(input, output);
                  break;
                }
            }
            // raw data
          default:
            this->m_mode = this->raw;
            this->m_count = pattern;
            break;
          }
    }
}

/**
 * \brief Load uncompressed data from the file.
 * \param f The file from which we're loading the bitmap.
 * \param buffer_size Number of bytes needed to store one line of pixels.
 * \param palette Color palette.
 * \param pixel_convert A method to convert one line of pixels from the file to
 *        a line of the current bitmap.
 *
 * \remark The Convert type method must take this four parameters in this
 * order: # scanline& destination line, # const char* input buffer (contains
 * one line of the bitmap), # const color_palette_type& palette The color
 * palette of the file,
 */
template <typename Convert>
void claw::graphic::bitmap::reader::load_rgb_data(
    std::istream& f, unsigned int buffer_size,
    const color_palette_type& palette, const Convert& pixel_convert)
{
  unsigned int line;

  // lines are 4-bytes aligned, so adjust buffer's size.
  if(buffer_size % 4 != 0)
    buffer_size += 4 - buffer_size % 4;

  char* buffer = new char[buffer_size];

  for(line = m_image.height(); (line > 0) && !f.eof();)
    {
      --line;
      f.read(buffer, buffer_size);
      pixel_convert(m_image[line], buffer, palette);
    }

  delete[] buffer;

  if(f.rdstate() != std::ios_base::goodbit)
    throw claw::bad_format("bitmap::reader::load_data");
}
