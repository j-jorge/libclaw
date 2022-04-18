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
 * \file targa_writer.cpp
 * \brief Implementation of the targa::writer class.
 * \author Julien Jorge
 */
#include <claw/graphic/targa.hpp>

#include <claw/exception.hpp>

//********************** targa::writer::file_output_buffer
//*********************

namespace claw
{
  namespace graphic
  {
    /**
     * \brief Write a pixel in the stream and set its value in the good order.
     * \param p The pixel to write.
     *
     * \remark This method is specialized for the pixels of type
     *         claw::graphic::rgba_pixel_32.
     */
    template <>
    void targa::writer::file_output_buffer<
        claw::graphic::rgba_pixel_8>::order_pixel_bytes(const pixel_type& p)
    {
      m_stream << p.components.blue << p.components.green << p.components.red
               << p.components.alpha;
    }
  }
}

//************************** targa::writer::writer
//*****************************

/**
 * \brief Constructor.
 * \param img The image to save.
 */
claw::graphic::targa::writer::writer(const image& img)
  : m_image(img)
{}

/**
 * \brief Constructor.
 * \param img The image to save.
 * \param f The file in which we save the data.
 * \param rle Tell if we must encode the data.
 */
claw::graphic::targa::writer::writer(const image& img, std::ostream& f,
                                     bool rle)
  : m_image(img)
{
  save(f, rle);
}

/**
 * \brief Save the content of the image in a stream.
 * \param os The stream in which we write.
 * \param rle Tell if we must encode the data.
 */
void claw::graphic::targa::writer::save(std::ostream& os, bool rle) const
{
  header h(m_image.width(), m_image.height());

  if(rle)
    h.image_type = rle_true_color;
  else
    h.image_type = true_color;

  os.write(reinterpret_cast<char*>(&h), sizeof(header));

  if(rle)
    save_rle_true_color(os);
  else
    save_true_color(os);

  footer f;
  os.write(reinterpret_cast<char*>(&f), sizeof(footer));
}

/**
 * \brief Save the content of the image, without compression.
 * \param os The stream in which we write.
 */
void claw::graphic::targa::writer::save_true_color(std::ostream& os) const
{
  file_output_buffer<rgba_pixel_8> output_buffer(os);

  for(const_iterator it = m_image.begin(); it != m_image.end(); ++it)
    output_buffer.order_pixel_bytes(*it);
}

/**
 * \brief Save the content of the image, with RLE compression.
 * \param os The stream in which we write.
 */
void claw::graphic::targa::writer::save_rle_true_color(std::ostream& os) const
{
  rle32_encoder encoder;
  rle32_encoder::output_buffer_type output_buffer(os);

  for(unsigned int y = 0; y != m_image.height(); ++y)
    encoder.encode(m_image[y].begin(), m_image[y].end(), output_buffer);
}
