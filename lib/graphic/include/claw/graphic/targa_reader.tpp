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
 * \file targa_reader.tpp
 * \brief Implementation of the template methods of the targa::reader class and
 *        subclasses.
 * \author Julien Jorge
 */
#include <iterator>
#include <limits>

/**
 * \brief Constructor.
 * \param f The file to read.
 */
template <typename Pixel>
claw::graphic::targa::reader::file_input_buffer<Pixel>::file_input_buffer(
    std::istream& f)
  : buffered_istream<std::istream>(f)
{}

//*****************************************************************************/

/**
 * \brief Constructor.
 * \param f The file to read.
 * \param p The color palette.
 */
template <typename Pixel>
claw::graphic::targa::reader::mapped_file_input_buffer<
    Pixel>::mapped_file_input_buffer(std::istream& f, const color_palette32& p)
  : buffered_istream<std::istream>(f)
  , m_palette(p)
{}

//*****************************************************************************/

/**
 * \brief Constructor.
 * \param img The targa image we're loading.
 * \param up_down Tell if the image is stored from top to bottom.
 * \param left_right Tell if the image is stored from left to right.
 */
template <typename InputBuffer>
claw::graphic::targa::reader::rle_targa_output_buffer<
    InputBuffer>::rle_targa_output_buffer(image& img, bool up_down,
                                          bool left_right)
  : m_image(img)
  , m_x_inc(left_right ? 1 : -1)
  , m_y_inc(up_down ? 1 : -1)
{
  if(up_down)
    m_y = 0;
  else
    m_y = m_image.height() - 1;

  if(left_right)
    m_x = 0;
  else
    m_x = m_image.width() - 1;
}

/**
 * \brief Copy a pixel a certain number of times.
 * \param n The number of pixel to write.
 * \param pattern The pixel to copy.
 */
template <typename InputBuffer>
void claw::graphic::targa::reader::rle_targa_output_buffer<InputBuffer>::fill(
    unsigned int n, rgba_pixel_8 pattern)
{
  assert((int)(m_x + m_x_inc * n) >= -1);
  assert(m_x + m_x_inc * n <= m_image.width());

  const int bound = (int)m_x + m_x_inc * n;
  int x = m_x;

  for(; x != bound; x += m_x_inc)
    m_image[m_y][x] = pattern;

  adjust_position(x);
}

/**
 * \brief Direct copy of a certain number of pixels from the file.
 * \param n The number of pixels to write.
 * \param buffer The buffer from which we read.
 */
template <typename InputBuffer>
void claw::graphic::targa::reader::rle_targa_output_buffer<InputBuffer>::copy(
    unsigned int n, input_buffer_type& buffer)
{
  assert((int)(m_x + m_x_inc * n) >= -1);
  assert(m_x + m_x_inc * n <= m_image.width());

  const int bound = (int)m_x + m_x_inc * n;
  int x = m_x;

  for(; x != bound; x += m_x_inc)
    m_image[m_y][x] = buffer.get_pixel();

  adjust_position(x);
}

/**
 * \brief Tell if we have completely filled the image.
 */
template <typename InputBuffer>
bool claw::graphic::targa::reader::rle_targa_output_buffer<
    InputBuffer>::completed() const
{
  return ((int)m_y == -1) || (m_y == m_image.height());
}

/**
 * \brief Recalculate the position in the file.
 * \param x The x-coordinate where we stopped.
 *
 * If \a x is lower tha zero, the position is set at the end of the previous
 * line ; if \a is greater or equal to the width of the image, the position is
 * set at the begining of the next line ; otherwise the position is set to \a
 * x.
 */
template <typename InputBuffer>
void claw::graphic::targa::reader::rle_targa_output_buffer<
    InputBuffer>::adjust_position(int x)
{
  if(x < 0)
    {
      m_x = m_image.width() - 1;
      m_y += m_y_inc;
    }
  else if(x >= (int)m_image.width())
    {
      m_x = 0;
      m_y += m_y_inc;
    }
  else
    m_x = x;
}

//*****************************************************************************/

/**
 * \brief Get the type of the following data in the input buffer.
 * \param input The input stream (the targa file).
 * \param output The output stream (the targa image).
 */
template <typename InputBuffer, typename OutputBuffer>
void claw::graphic::targa::reader::rle_targa_decoder<
    InputBuffer, OutputBuffer>::read_mode(input_buffer_type& input,
                                          output_buffer_type& output)
{
  this->m_mode = this->stop;
  bool ok = !output.completed();

  if(ok && (input.remaining() < 1))
    ok = input.read_more(1);

  if(ok)
    {
      char key = input.get_next();

      this->m_count = (key & 0x7F) + 1;

      if(key & 0x80) // compressed
        {
          this->m_mode = this->compressed;
          this->m_pattern = input.get_pixel();
        }
      else
        this->m_mode = this->raw;
    }
}

//*****************************************************************************/

/**
 * \brief Load an uncompressed true color targa file.
 * \param h File's header, must have been read before call.
 * \param f Targa file.
 * \param palette The color palette of the image.
 * \pre f.is_open()
 */
template <typename Pixel>
void claw::graphic::targa::reader::load_color_mapped_raw(
    const header& h, std::istream& f, const color_palette32& palette)
{
  /* We use a part of the rle framework but there isn't any compressed data
     here. We only use the direct copy of the rle algorithm. */

  typedef mapped_file_input_buffer<Pixel> input_buffer_type;

  rle_targa_output_buffer<input_buffer_type> output(
      m_image, h.image_specification.up_down_oriented(),
      h.image_specification.left_right_oriented());
  input_buffer_type input(f, palette);

  for(unsigned int i = 0; i != m_image.height(); ++i)
    output.copy(m_image.width(), input);
}

/**
 * \brief Load a RLE color mapped targa file.
 * \param h File's header, must have been read before call.
 * \param f Targa file.
 * \param palette The color palette of the image.
 * \pre f.is_open()
 */
template <typename Decoder>
void claw::graphic::targa::reader::decompress_rle_color_mapped(
    const header& h, std::istream& f, const color_palette32& palette)
{
  Decoder decoder;
  typename Decoder::output_buffer_type output_buffer(
      m_image, h.image_specification.up_down_oriented(),
      h.image_specification.left_right_oriented());
  typename Decoder::input_buffer_type input_buffer(f, palette);

  decoder.decode(input_buffer, output_buffer);
}

/**
 * \brief Load an uncompressed true color targa file.
 * \param h File's header, must have been read before call.
 * \param f Targa file.
 * \pre f.is_open() && !h.color_map
 */
template <typename Pixel>
void claw::graphic::targa::reader::load_true_color_raw(const header& h,
                                                       std::istream& f)
{
  assert(!h.color_map);

  /* We use a part of the rle framework but there isn't any compressed data
     here. We only use the direct copy of the rle algorithm. */

  typedef file_input_buffer<Pixel> input_buffer_type;

  rle_targa_output_buffer<input_buffer_type> output(
      m_image, h.image_specification.up_down_oriented(),
      h.image_specification.left_right_oriented());
  input_buffer_type input(f);

  for(unsigned int i = 0; i != m_image.height(); ++i)
    output.copy(m_image.width(), input);
}

/**
 * \brief Load a true color RLE targa file.
 * \param h File's header, must have been read before call.
 * \param f Targa file.
 * \pre f.is_open() && !h.color_map
 */
template <typename Decoder>
void claw::graphic::targa::reader::decompress_rle_true_color(const header& h,
                                                             std::istream& f)
{
  assert(!h.color_map);

  Decoder decoder;
  typename Decoder::output_buffer_type output_buffer(
      m_image, h.image_specification.up_down_oriented(),
      h.image_specification.left_right_oriented());
  typename Decoder::input_buffer_type input_buffer(f);

  decoder.decode(input_buffer, output_buffer);
}

/**
 * \brief Load the content of the color palette.
 * \param f Targa file.
 * \param palette (out) The color palette.
 */
template <typename Pixel>
void claw::graphic::targa::reader::load_palette_content(
    std::istream& f, color_palette32& palette) const
{
  file_input_buffer<Pixel> input(f);

  for(unsigned int i = 0; i != palette.size(); ++i)
    palette[i] = input.get_pixel();
}
