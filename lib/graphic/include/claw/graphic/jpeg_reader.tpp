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
 * \file jpeg_reader.tpp
 * \brief Implementation of the template methods of the
 *        claw::graphic::jpeg::reader class.
 * \author Julien Jorge
 */
#include <claw/graphic/jpeg_error_manager.hpp>

#include <claw/exception.hpp>

/**
 * \brief Decompress a RGB jpeg image.
 * \param cinfo Informations about the decompression process.
 * \param pixel_convert Operator to use to convert pixels from input.
 */
template <class Convert>
void claw::graphic::jpeg::reader::read_data(jpeg_decompress_struct& cinfo,
                                            const Convert& pixel_convert)
{
  const unsigned int pixel_size = cinfo.output_components;
  JSAMPLE* buffer = new JSAMPLE[cinfo.output_width * pixel_size];

  error_manager jerr;
  jpeg_error_mgr* jerr_saved = cinfo.err;

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = jpeg__error_manager__error_exit;

  if(setjmp(jerr.setjmp_buffer))
    {
      delete[] buffer;
      throw CLAW_EXCEPTION(jerr.error_string);
    }

  while(cinfo.output_scanline < cinfo.output_height)
    {
      jpeg_read_scanlines(&cinfo, &buffer, 1);

      scanline::iterator pixel = m_image[cinfo.output_scanline - 1].begin();

      for(unsigned int i = 0; i != pixel_size * m_image.width();
          i += pixel_size, ++pixel)
        *pixel = pixel_convert(&buffer[i]);
    }

  delete[] buffer;
  cinfo.err = jerr_saved;
}
