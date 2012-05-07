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
 * \file png_reader.cpp
 * \brief Implementation of the claw::graphic::png::reader class.
 * \author Julien Jorge
 */
#include <claw/png.hpp>

#include <claw/exception.hpp>
#include <claw/assert.hpp>

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read data from the input stream.
 * \param png_ptr Informations about the PNG we are reading.
 * \param data (out) Array of the bytes we have read.
 * \param length Number of bytes to read.
 */
void claw__graphic__png__source_manager__read
( png_structp png_ptr, png_bytep data, png_size_t length )
{
  claw::graphic::png::reader::source_manager* self =
    (claw::graphic::png::reader::source_manager*)png_get_io_ptr(png_ptr);

  self->read(data, length);
} // claw__graphic__png__source_manager__read()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param is The stream we read from.
 */
claw::graphic::png::reader::source_manager::source_manager( std::istream& is )
  : m_input(is)
{
  CLAW_PRECOND( !!is );
} // png::reader::source_manager::source_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read data from the input stream.
 * \param data (out) Array of the bytes we have read.
 * \param length Number of bytes to read.
 */
void claw::graphic::png::reader::source_manager::read
( png_bytep data, png_size_t length )
{
  m_input.read( (char*)data, length * sizeof(png_byte) );
} // png::reader::source_manager::read()




/*----------------------------------------------------------------------------*/
const unsigned int claw::graphic::png::reader::s_rgba_pixel_size = 4;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which the data will be stored.
 */
claw::graphic::png::reader::reader( image& img )
  : m_image( img )
{

} // png::reader::reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which the data will be stored.
 * \param f The file from which we read the data.
 * \post img contains the data from \a f.
 */
claw::graphic::png::reader::reader( image& img, std::istream& f )
  : m_image( img )
{
  load(f);
} // png::reader::reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an image from a png file.
 * \param f PNG file.
 */
void claw::graphic::png::reader::load( std::istream& f )
{
  CLAW_PRECOND( !!f );

  std::istream::pos_type init_pos = f.tellg();

  try
    {
      read_from_file(f);
    }
  catch(...)
    {
      f.clear();
      f.seekg( init_pos, std::ios_base::beg );
      throw;
    }
} // png::reader::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an image from a png file.
 * \param f PNG file.
 */
void claw::graphic::png::reader::read_from_file( std::istream& f )
{
  source_manager infile(f);
  png_structp png_ptr;
  png_infop info_ptr;

  create_read_structures(png_ptr, info_ptr);

  if (setjmp(png_jmpbuf(png_ptr)))
    {
      /* If we get here, we had a problem reading the file */
      /* Free all of the memory associated with the png_ptr and info_ptr */
      png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
      throw CLAW_EXCEPTION("Invalid PNG file.");
    }
      
  check_if_png( png_ptr, f );

  png_set_read_fn( png_ptr, (void *)&infile,
                   claw__graphic__png__source_manager__read );

  png_set_strip_16(png_ptr);
  png_set_expand_gray_1_2_4_to_8(png_ptr);
  png_set_packing(png_ptr);

  // transform palette index into RGB value
  png_set_palette_to_rgb(png_ptr);

  // add an alpha value if none
  png_set_filler( png_ptr,
                  std::numeric_limits<rgba_pixel_8::component_type>::max(),
                  PNG_FILLER_AFTER );

  png_read_info(png_ptr, info_ptr);
  read_image( png_ptr, info_ptr );
      
  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
} // png::reader::read_from_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check that the stream contains a PNG file.
 * \param png_ptr PNG file description.
 * \param f The stream to read from.
 */
void claw::graphic::png::reader::check_if_png
( png_structp png_ptr, std::istream& f ) const
{
  CLAW_PRECOND( !!f );

  const unsigned int bytes_to_check = 8;
  png_byte buffer[bytes_to_check];

  /* Read in some of the signature bytes */
  f.read( (char*)buffer, bytes_to_check * sizeof(png_byte) );

  if ( (png_sig_cmp(buffer, (png_size_t)0, bytes_to_check) != 0) || !f )
    throw CLAW_EXCEPTION( "Not a PNG file." );

  png_set_sig_bytes(png_ptr, bytes_to_check);
} // png::reader::check_if_png()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the image data of the PNG.
 * \param png_ptr PNG file description.
 * \param info_ptr PNG file informations.
 */
void claw::graphic::png::reader::read_image
( png_structp png_ptr, png_infop info_ptr )
{
  CLAW_PRECOND( png_ptr );
  CLAW_PRECOND( info_ptr );

  m_image.set_size( png_get_image_width(png_ptr, info_ptr),
                    png_get_image_height(png_ptr, info_ptr) );

  if ( png_get_interlace_type(png_ptr, info_ptr) == PNG_INTERLACE_NONE )
    read_sequential_image(png_ptr, info_ptr);
  else
    read_interlaced_image( png_ptr, info_ptr,
                           png_set_interlace_handling(png_ptr) );
} // png::reader::read_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the image data of a non interlaced PNG.
 * \param png_ptr PNG file description.
 * \param info_ptr PNG file informations.
 */
void claw::graphic::png::reader::read_sequential_image
( png_structp png_ptr, png_infop info_ptr )
{
  CLAW_PRECOND( png_ptr );
  CLAW_PRECOND( info_ptr );

  png_bytep data =
    (png_bytep)png_malloc( png_ptr, s_rgba_pixel_size * m_image.width() );

  try
    {
      for (unsigned int y=0; y!=m_image.height(); ++y)
        {
          png_read_row(png_ptr, data, NULL);
          copy_pixel_line( data, y );
        }
    }
  catch(...)
    {
      png_free(png_ptr, data);
      throw;
    }

  png_free(png_ptr, data);
} // png::reader::read_sequential_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the image data of an interlaced PNG.
 * \param png_ptr PNG file description.
 * \param info_ptr PNG file informations.
 * \param passes Number of passes (for interlaced images).
 */
void claw::graphic::png::reader::read_interlaced_image
( png_structp png_ptr, png_infop info_ptr, unsigned int passes )
{
  CLAW_PRECOND( passes > 1 );
  CLAW_PRECOND( png_ptr );
  CLAW_PRECOND( info_ptr );

  const unsigned int row_length = s_rgba_pixel_size * m_image.width();
  png_bytepp data =
    (png_bytepp)png_malloc( png_ptr, sizeof(png_bytep) * m_image.height() );
  unsigned int i=0;

  try
    {
      for (i=0; i!=m_image.height(); ++i)
        {
          data[i] = (png_bytep)png_malloc( png_ptr, row_length );

          if (!data[i])
            throw std::bad_alloc();

          copy_pixel_line( data[i], i );
        }

      for (unsigned int p=0; p!=passes; ++p)
        png_read_rows( png_ptr, data, NULL, m_image.height() );

      for (unsigned int y=0; y!=m_image.height(); ++y)
        copy_pixel_line( data[y], y );
    }
  catch(...)
    {
      for(unsigned int j=0; j!=i; ++j)
        png_free(png_ptr, data[j]);

      png_free(png_ptr, data);
      throw;
    }

  for(i=0; i!=m_image.height(); ++i)
    png_free(png_ptr, data[i]);

  png_free(png_ptr, data);
} // png::reader::read_interlaced_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the pixels taken from the PNG to the image.
 * \param data the pixels from the PNG image.
 * \param y Index of the line of the image in which we copy the pixels.
 */
void
claw::graphic::png::reader::copy_pixel_line( png_bytep data, unsigned int y )
{
  CLAW_PRECOND( data );
  CLAW_PRECOND( y < m_image.height() );

  // four bytes for each pixel in the line
  for (unsigned int x=0; x!=m_image.width(); ++x, data+=s_rgba_pixel_size)
    {
      m_image[y][x].components.red   = data[0];
      m_image[y][x].components.green = data[1];
      m_image[y][x].components.blue  = data[2];
      m_image[y][x].components.alpha = data[3];
    }
} // png::reader::copy_pixel_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the png read structures.
 * \param png_ptr PNG file description.
 * \param info_ptr PNG file informations.
 */
void claw::graphic::png::reader::create_read_structures
( png_structp& png_ptr, png_infop& info_ptr ) const
{
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (png_ptr)
    {
      info_ptr = png_create_info_struct(png_ptr);

      if (!info_ptr)
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    }

  if (!png_ptr || !info_ptr)
    throw CLAW_EXCEPTION("Can't create PNG read structures.");
} // png::reader::create_read_structures()
