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
 * \file pcx_writer.cpp
 * \brief Implementation of the pcx::writer class.
 * \author Julien Jorge
 */
#include <claw/graphic/pcx.hpp>

/**
 * \brief Constructor.
 * \param os The stream in which we write the encoded data.
 */
claw::graphic::pcx::writer::file_output_buffer::file_output_buffer(
    std::ostream& os)
  : m_stream(os)
{}

/**
 * \brief Encode a pixel data.
 * \param n The number of time the pixel appears.
 * \param pattern The data of the pixel.
 */
void claw::graphic::pcx::writer::file_output_buffer::encode(
    unsigned int n, pattern_type pattern)
{
  if((pattern > 63) || (n > 1))
    {
      u_int_8 cnt = 0xC0 | (u_int_8)n;
      m_stream.write(reinterpret_cast<char*>(&cnt), sizeof(u_int_8));
    }

  m_stream.write(reinterpret_cast<char*>(&pattern), sizeof(u_int_8));
}

/**
 * \brief Get the minimum number of pixels needed for encoding.
 */
unsigned int
claw::graphic::pcx::writer::file_output_buffer::min_interesting() const
{
  return 1;
}

/**
 * \brief Get the maximum number of pixel a code can encode.
 */
unsigned int
claw::graphic::pcx::writer::file_output_buffer::max_encodable() const
{
  return 63;
}

/**
 * \brief Constructor.
 * \param img The image to save.
 */
claw::graphic::pcx::writer::writer(const image& img)
  : m_image(img)
{}

/**
 * \brief Constructor.
 * \param img The image to save.
 * \param f The file in which we save the data.
 */
claw::graphic::pcx::writer::writer(const image& img, std::ostream& f)
  : m_image(img)
{
  save(f);
}

/**
 * \brief Save the content of the image in a stream.
 * \param os The stream in which we write.
 */
void claw::graphic::pcx::writer::save(std::ostream& os) const
{
  const unsigned int bytes_per_line = m_image.width() + m_image.width() % 2;

  write_header(os, bytes_per_line);
  save_rle_true_color(os, bytes_per_line);
}

/**
 * \brief Save the header of the image in a stream.
 * \param os The stream in which we write.
 * \param bytes_per_line Number of bytes per decoded line.
 */
void claw::graphic::pcx::writer::write_header(
    std::ostream& os, unsigned int bytes_per_line) const
{
  header h;

  h.manufacturer = 10;
  h.version = 5;
  h.encoded = 1;
  h.bpp = 8;
  h.window.x_min = 0;
  h.window.y_min = 0;
  h.window.x_max = m_image.width() - 1;
  h.window.y_max = m_image.height() - 1;
  h.horizontal_dpi = 72; // arbitrary value
  h.vertical_dpi = 72;
  std::fill(h.color_map, h.color_map + 16, rgb_pixel_8(0, 0, 0));
  h.reserved = 0;
  h.color_planes = 3; // RGB
  h.bytes_per_line = bytes_per_line;
  h.palette_info = 0;
  h.screen_size.horizontal = 0;
  h.screen_size.vertical = 0;
  std::fill(h.filler, h.filler + 54, 0);

  os.write(reinterpret_cast<char*>(&h), sizeof(header));
}

/**
 * \brief Save the content of the image in a stream.
 * \param os The stream in which we write.
 * \param bytes_per_line Number of bytes per decoded line.
 */
void claw::graphic::pcx::writer::save_rle_true_color(
    std::ostream& os, unsigned int bytes_per_line) const
{
  std::vector<u_int_8> data(bytes_per_line, 0);

  rle_pcx_encoder encoder;
  file_output_buffer output(os);

  for(unsigned int y = 0; y != m_image.height(); ++y)
    {
      // red
      for(unsigned int x = 0; x != m_image.width(); ++x)
        data[x] = m_image[y][x].components.red;

      encoder.encode(data.begin(), data.end(), output);

      // green
      for(unsigned int x = 0; x != m_image.width(); ++x)
        data[x] = m_image[y][x].components.green;

      encoder.encode(data.begin(), data.end(), output);

      // blue
      for(unsigned int x = 0; x != m_image.width(); ++x)
        data[x] = m_image[y][x].components.blue;

      encoder.encode(data.begin(), data.end(), output);
    }
}
