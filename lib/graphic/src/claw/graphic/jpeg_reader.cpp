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
 * \file jpeg_reader.cpp
 * \brief Implementation of the claw::graphic::jpeg::reader class.
 * \author Julien Jorge
 */
#include <claw/graphic/jpeg.hpp>

#include <claw/graphic/jpeg_error_manager.hpp>

#include <claw/assert.hpp>
#include <claw/exception.hpp>

/**
 * \brief Initialize the input stream.
 * \param cinfo Informations about the decompression.
 */
METHODDEF(void)
claw__graphic__jpeg__source_manager__init_source(j_decompress_ptr cinfo)
{
  // nothing to do
}

/**
 * \brief Fill the input buffer with new data.
 * \param cinfo Informations about the decompression.
 */
METHODDEF(boolean)
claw__graphic__jpeg__source_manager__fill_input_buffer(j_decompress_ptr cinfo)
{
  claw::graphic::jpeg::reader::source_manager* self
      = (claw::graphic::jpeg::reader::source_manager*)cinfo->client_data;

  CLAW_PRECOND(&self->pub == cinfo->src);

  return self->fill_input_buffer();
}

/**
 * \brief Skip some bytes in the input buffer.
 * \param cinfo Informations about the decompression.
 * \param num_bytes The number of bytes to skip.
 */
METHODDEF(void)
claw__graphic__jpeg__source_manager__skip_input_data(j_decompress_ptr cinfo,
                                                     long num_bytes)
{
  claw::graphic::jpeg::reader::source_manager* self
      = (claw::graphic::jpeg::reader::source_manager*)cinfo->client_data;

  CLAW_PRECOND(&self->pub == cinfo->src);

  return self->skip_input_data(num_bytes);
}

/**
 * \brief Close the input stream.
 * \param cinfo Informations about the decompression.
 */
METHODDEF(void)
claw__graphic__jpeg__source_manager__term_source(j_decompress_ptr cinfo)
{
  // nothing to do
}

/**
 * \brief Constructor.
 * \param is The stream we read from.
 */
claw::graphic::jpeg::reader::source_manager::source_manager(std::istream& is)
  : m_input(is)
  , m_buffer_size(1024)
  , m_stream_position(0)
{
  std::istream::pos_type pos = is.tellg();

  is.seekg(0, std::ios_base::end);
  m_stream_size = is.tellg();

  is.seekg(pos, std::ios_base::beg);

  m_buffer = new JOCTET[m_buffer_size];
  pub.bytes_in_buffer = 0;
}

/**
 * \brief Destructor.
 */
claw::graphic::jpeg::reader::source_manager::~source_manager()
{
  delete[] m_buffer;
}

/**
 * \brief Fill the input buffer with new data.
 */
boolean claw::graphic::jpeg::reader::source_manager::fill_input_buffer()
{
  unsigned int n = std::min(m_buffer_size, m_stream_size - m_stream_position);
  m_input.read((char*)m_buffer, n);

  pub.next_input_byte = m_buffer;
  pub.bytes_in_buffer = n;

  m_stream_position += n;

  if(m_input)
    return TRUE;
  else
    return FALSE;
}

/**
 * \brief Skip some bytes in the input buffer.
 * \param num_bytes The number of bytes to skip.
 */
void claw::graphic::jpeg::reader::source_manager::skip_input_data(
    long num_bytes)
{
  CLAW_PRECOND(num_bytes >= 0);

  if((size_t)num_bytes <= pub.bytes_in_buffer)
    {
      pub.next_input_byte += num_bytes;
      pub.bytes_in_buffer -= num_bytes;
    }
  else
    {
      num_bytes -= pub.bytes_in_buffer;

      long div = num_bytes / m_buffer_size;
      long rest = num_bytes % m_buffer_size;

      for(long i = 0; i != (div + 1); ++i)
        fill_input_buffer();

      pub.next_input_byte += rest;
      pub.bytes_in_buffer -= rest;
    }
}

/**
 * \brief Convert a RGB pixel to a ARGB pixel.
 */
claw::graphic::rgba_pixel_8
claw::graphic::jpeg::reader::RGB_to_pixel32::operator()(
    const JSAMPLE* pixel) const
{
  rgba_pixel_8 result;

  result.components.alpha = 255;
  result.components.red = pixel[0];
  result.components.green = pixel[1];
  result.components.blue = pixel[2];

  return result;
}

/**
 * \brief Convert a grey level pixel to a ARGB pixel.
 */
claw::graphic::rgba_pixel_8
claw::graphic::jpeg::reader::grayscale_to_pixel32::operator()(
    const JSAMPLE* pixel) const
{
  rgba_pixel_8 result;

  result.components.alpha = 255;
  result.components.red = pixel[0];
  result.components.green = pixel[0];
  result.components.blue = pixel[0];

  return result;
}

/**
 * \brief Constructor.
 * \param img The image in which the data will be stored.
 */
claw::graphic::jpeg::reader::reader(image& img)
  : m_image(img)
{}

/**
 * \brief Constructor. Load an image from a jpeg file.
 * \param img The image in which the data will be stored.
 * \param f The file from which we read the data.
 * \post img contains the data from \a f.
 */
claw::graphic::jpeg::reader::reader(image& img, std::istream& f)
  : m_image(img)
{
  load(f);
}

/**
 * \brief Load an image from a jpeg file.
 * \param f JPEG file.
 */
void claw::graphic::jpeg::reader::load(std::istream& f)
{
  CLAW_PRECOND(!!f);

  std::istream::pos_type init_pos = f.tellg();

  try
    {
      read_from_file(f);
    }
  catch(...)
    {
      f.clear();
      f.seekg(init_pos, std::ios_base::beg);
      throw;
    }
}

/**
 * \brief Load an image from a jpeg file.
 * \param f Jpeg file.
 */
void claw::graphic::jpeg::reader::read_from_file(std::istream& f)
{
  source_manager infile(f);
  jpeg_decompress_struct cinfo;
  error_manager jerr;

  cinfo.err = jpeg_std_error(&jerr.pub);

  if(setjmp(jerr.setjmp_buffer))
    throw CLAW_EXCEPTION(jerr.error_string);

  create_decompress_info(cinfo, infile);
  jerr.pub.error_exit = jpeg__error_manager__error_exit;

  try
    {
      decompress(f, cinfo);
      jpeg_destroy_decompress(&cinfo);
    }
  catch(...)
    {
      jpeg_destroy_decompress(&cinfo);
      throw;
    }
}

/**
 * \brief Decompress the data of a Jpeg file.
 * \param f Jpeg file.
 * \param cinfo Info on the decompression process (libjpeg concerned).
 */
void claw::graphic::jpeg::reader::decompress(std::istream& f,
                                             jpeg_decompress_struct& cinfo)
{
  error_manager jerr;
  jpeg_error_mgr* jerr_saved = cinfo.err;

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = jpeg__error_manager__error_exit;

  if(setjmp(jerr.setjmp_buffer))
    {
      jpeg_abort_decompress(&cinfo);
      throw CLAW_EXCEPTION(jerr.error_string);
    }

  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  try
    {
      m_image.set_size(cinfo.image_width, cinfo.image_height);

      if(cinfo.out_color_components == 3)
        read_data(cinfo, RGB_to_pixel32());
      else if(cinfo.out_color_components == 1)
        read_data(cinfo, grayscale_to_pixel32());
      else
        throw CLAW_EXCEPTION("invalid number of colors per channel");

      jpeg_finish_decompress(&cinfo);
    }
  catch(...)
    {
      jpeg_abort_decompress(&cinfo);
      throw;
    }

  cinfo.err = jerr_saved;
}

/**
 * \brief Initialize the jpeg decompression structure.
 * \param cinfo The structure to initialize.
 * \param infile The source manager.
 */
void claw::graphic::jpeg::reader::create_decompress_info(
    jpeg_decompress_struct& cinfo, source_manager& infile) const
{
  jpeg_create_decompress(&cinfo);

  cinfo.src = &infile.pub;
  cinfo.client_data = &infile;

  infile.pub.fill_input_buffer
      = claw__graphic__jpeg__source_manager__fill_input_buffer;
  infile.pub.skip_input_data
      = claw__graphic__jpeg__source_manager__skip_input_data;
  infile.pub.init_source = claw__graphic__jpeg__source_manager__init_source;
  infile.pub.resync_to_restart = jpeg_resync_to_restart;
  infile.pub.term_source = claw__graphic__jpeg__source_manager__term_source;
}
