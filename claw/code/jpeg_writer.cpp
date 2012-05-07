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
 * \file jpeg_writer.cpp
 * \brief Implementation of the claw::graphic::jpeg::writer class.
 * \author Julien Jorge
 */
#include <claw/jpeg.hpp>
#include <claw/jpeg_error_manager.hpp>

#include <claw/exception.hpp>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the output stream.
 * \param cinfo Informations about the compression.
 */
METHODDEF(void)
claw__graphic__jpeg__destination_manager__init_destination(j_compress_ptr cinfo)
{
  // nothing to do
} // claw__graphic__jpeg__destination_manager__init_destination()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write the content of the buffer in the file.
 * \param cinfo Informations about the compression.
 */
METHODDEF(boolean)
claw__graphic__jpeg__destination_manager__empty_output_buffer
(j_compress_ptr cinfo)
{
  claw::graphic::jpeg::writer::destination_manager* self =
    (claw::graphic::jpeg::writer::destination_manager*)cinfo->client_data;

  CLAW_PRECOND( &self->pub == cinfo->dest );

  self->flush();

  return TRUE;
} // claw__graphic__jpeg__destination_manager__empty_output_buffer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write the last pending bytes in the file.
 * \param cinfo Informations about the compression.
 */
METHODDEF(void)
claw__graphic__jpeg__destination_manager__term_destination(j_compress_ptr cinfo)
{
  claw::graphic::jpeg::writer::destination_manager* self =
    (claw::graphic::jpeg::writer::destination_manager*)cinfo->client_data;

  CLAW_PRECOND( &self->pub == cinfo->dest );

  self->term();
} // claw__graphic__jpeg__destination_manager__term_destination()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param os The stream we write in.
 */
claw::graphic::jpeg::writer::destination_manager::destination_manager
( std::ostream& os )
  : m_output(os), m_buffer_size(1024)
{
  m_buffer = new JOCTET[m_buffer_size];
  pub.next_output_byte = m_buffer;
  pub.free_in_buffer = m_buffer_size;
} // jpeg::writer::destination_manager::destination_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
claw::graphic::jpeg::writer::destination_manager::~destination_manager()
{
  delete[] m_buffer;
} // jpeg::writer::destination_manager::~destination_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write the content of the buffer in the file.
 */
void claw::graphic::jpeg::writer::destination_manager::flush()
{
  m_output.write((char*)m_buffer, m_buffer_size);

  pub.next_output_byte = m_buffer;
  pub.free_in_buffer = m_buffer_size;
} // jpeg::writer::destination_manager::fill_output_buffer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write the last pending bytes in the file.
 */
void
claw::graphic::jpeg::writer::destination_manager::term()
{
  m_output.write((char*)m_buffer, m_buffer_size - pub.free_in_buffer);
} // jpeg::writer::destination_manager::term()



/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 * \remark Default options are \a quality = 75 % and \a progressive = false.
 */
claw::graphic::jpeg::writer::options::options()
  : quality(75), progressive(false)
{

} // jpeg::writer::options::options()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param quality_ Quality of the saved image.
 * \param progressive_ Tell if we save a progressive jpeg.
 * \remark If \a quality_ > 100, then it will be adjusted to 100 when saving.
 */
claw::graphic::jpeg::writer::options::options
( unsigned char quality_, bool progressive_ )
  : quality(quality_), progressive(progressive_)
{

} // jpeg::writer::options::options()




/*----------------------------------------------------------------------------*/
const unsigned int claw::graphic::jpeg::writer::s_rgb_pixel_size = 3;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which the data will be stored.
 */
claw::graphic::jpeg::writer::writer( const image& img )
  : m_image( img )
{

} // jpeg::writer::writer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor. Save an image in a jpeg file.
 * \param img The image from which we read the data.
 * \param f The file in which we write the data.
 * \param opt Options about the saved file.
 */
claw::graphic::jpeg::writer::writer
( const image& img, std::ostream& f, const options& opt )
  : m_image( img )
{
  save(f, opt);
} // jpeg::writer::writer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save an image in a jpeg file.
 * \param f Jpeg file.
 * \param opt Options about the saved file.
 */
void
claw::graphic::jpeg::writer::save( std::ostream& f, const options& opt ) const
{
  CLAW_PRECOND( !!f );

  destination_manager outfile(f);
  jpeg_compress_struct cinfo;
  error_manager jerr;

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = jpeg__error_manager__error_exit;
  
  if ( setjmp(jerr.setjmp_buffer) )
    throw CLAW_EXCEPTION(jerr.error_string);
  
  create_compress_info( cinfo, outfile );
  
  try
    {
      set_options( cinfo, opt );
      save_image( cinfo );
      jpeg_destroy_compress(&cinfo);
    }
  catch(...)
    {
      jpeg_abort_compress(&cinfo);
      jpeg_destroy_compress(&cinfo);
      throw;
    }
} // jpeg::writer::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the parameters of the JPEG saving structures.
 * \param cinfo JPEG file description.
 * \param opt Options about the saved file.
 */
void claw::graphic::jpeg::writer::set_options
( jpeg_compress_struct& cinfo, const options& opt ) const
{
  cinfo.image_width = m_image.width();       /* image width, in pixels */
  cinfo.image_height = m_image.height();     /* image height, in pixels */
  cinfo.input_components = s_rgb_pixel_size; /* # of components per pixel */
  cinfo.in_color_space = JCS_RGB;            /* colorspace of input image */

  jpeg_set_defaults(&cinfo);

  if (opt.quality > 100)
    jpeg_set_quality(&cinfo, 100, TRUE);
  else
    jpeg_set_quality(&cinfo, opt.quality, TRUE);

  if (opt.progressive)
    jpeg_simple_progression(&cinfo);
} // jpeg::writer::set_options()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the image in the configured stream.
 * \param cinfo The structure to initialize.
 */
void
claw::graphic::jpeg::writer::save_image( jpeg_compress_struct& cinfo ) const
{
  JSAMPLE* data = new JSAMPLE[ m_image.width() * s_rgb_pixel_size ];

  error_manager jerr;
  jpeg_error_mgr* jerr_saved = cinfo.err;

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = jpeg__error_manager__error_exit;

  if ( setjmp(jerr.setjmp_buffer) )
    {
      delete[] data;
      jpeg_abort_compress(&cinfo);
      throw CLAW_EXCEPTION(jerr.error_string);
    }

  jpeg_start_compress( &cinfo, TRUE );

  while (cinfo.next_scanline < cinfo.image_height)
    {
      copy_pixel_line( data, cinfo.next_scanline );
      jpeg_write_scanlines( &cinfo, &data, 1 );
    }

  delete[] data;
  jpeg_finish_compress(&cinfo);

  cinfo.err = jerr_saved;
} // jpeg::writer::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the pixels from the image to an array of bytes.
 * \param data (out) The pixels for the JPEG image.
 * \param y Index of the line of the image from which we read the pixels.
 * \pre The memory pointed by data is long enough to store the pixels.
 */
void claw::graphic::jpeg::writer::copy_pixel_line
( JSAMPLE* data, unsigned int y ) const
{
  CLAW_PRECOND( data );
  CLAW_PRECOND( y < m_image.height() );

  // three bytes for each pixel in the line
  for (unsigned int x=0; x!=m_image.width(); ++x, data+=s_rgb_pixel_size)
    {
      data[0] = m_image[y][x].components.red;
      data[1] = m_image[y][x].components.green;
      data[2] = m_image[y][x].components.blue;
    }
} // jpeg::writer::copy_pixel_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the jpeg compression structure.
 * \param cinfo The structure to initialize.
 * \param outfile The destination manager.
 */
void claw::graphic::jpeg::writer::create_compress_info
( jpeg_compress_struct& cinfo, destination_manager& outfile ) const
{
  jpeg_create_compress(&cinfo);

  cinfo.dest = &outfile.pub;
  cinfo.client_data = &outfile;

  outfile.pub.init_destination =
    claw__graphic__jpeg__destination_manager__init_destination;
  outfile.pub.empty_output_buffer =
    claw__graphic__jpeg__destination_manager__empty_output_buffer;
  outfile.pub.term_destination =
    claw__graphic__jpeg__destination_manager__term_destination;
} // jpeg::writer::create_compress_info()
