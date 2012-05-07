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
 * \file png_writer.cpp
 * \brief Implementation of the claw::graphic::png::writer class.
 * \author Julien Jorge
 */
#include <claw/png.hpp>

#include <claw/exception.hpp>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Write data in the ouput stream.
 * \param png_ptr Informations about the PNG we are writing.
 * \param data (out) Array of the bytes to write.
 * \param length Number of bytes to write.
 */
void claw__graphic__png__target_manager__write
( png_structp png_ptr, png_bytep data, png_size_t length )
{
  claw::graphic::png::writer::target_manager* self =
    (claw::graphic::png::writer::target_manager*)png_get_io_ptr(png_ptr);

  self->write(data, length);
} // claw__graphic__png__target_manager__write()

/*----------------------------------------------------------------------------*/
/**
 * \brief Flush the output stream.
 * \param png_ptr Informations about the PNG we are writing.
 */
void claw__graphic__png__target_manager__flush( png_structp png_ptr )
{
  claw::graphic::png::writer::target_manager* self =
    (claw::graphic::png::writer::target_manager*)png_get_io_ptr(png_ptr);

  self->flush();
} // claw__graphic__png__target_manager__write()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param os The stream in which we write.
 */
claw::graphic::png::writer::target_manager::target_manager( std::ostream& os )
  : m_output(os)
{
  CLAW_PRECOND( !!os );
} // png::writer::target_manager::target_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write data in the ouput stream.
 * \param data (out) Array of the bytes to write.
 * \param length Number of bytes to write.
 */
void claw::graphic::png::writer::target_manager::write
( png_bytep data, png_size_t length )
{
  m_output.write( (char*)data, length * sizeof(png_byte) );
} // png::writer::target_manager::write()

/*----------------------------------------------------------------------------*/
/**
 * \brief Flush the output stream.
 */
void claw::graphic::png::writer::target_manager::flush()
{
  m_output.flush();
} // png::writer::target_manager::flush()




/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
claw::graphic::png::writer::options::options()
  : compression(default_compression), interlace(none)
{

} // png::writer::options::options()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param compression_level_ Compression level to use in the saved stream.
 * \param interlace_ Interlace method to apply to the saved image.
 */
claw::graphic::png::writer::options::options
( compression_level compression_level_, interlace_type interlace_ )
  : compression(compression_level_), interlace(interlace_)
{

} // png::writer::options::options()




/*----------------------------------------------------------------------------*/
const unsigned int claw::graphic::png::writer::s_rgba_pixel_size = 4;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which the data will be stored.
 */
claw::graphic::png::writer::writer( const image& img )
  : m_image( img )
{

} // png::writer::writer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image to save.
 * \param f The file in which we write the data.
 * \param opt Saving options.
 */
claw::graphic::png::writer::writer
( const image& img, std::ostream& f, const options& opt )
  : m_image( img )
{
  save(f, opt);
} // png::writer::writer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the image in a PNG file.
 * \param f PNG file.
 * \param opt Saving options.
 */
void
claw::graphic::png::writer::save( std::ostream& f, const options& opt ) const
{
  CLAW_PRECOND( !!f );

  target_manager outfile(f);
  png_structp png_ptr;
  png_infop info_ptr;

  create_write_structures(png_ptr, info_ptr);

  if (setjmp(png_jmpbuf(png_ptr)))
    {
      /* If we get here, we had a problem reading the file */
      /* Free all of the memory associated with the png_ptr and info_ptr */
      png_destroy_write_struct(&png_ptr, &info_ptr);
      throw CLAW_EXCEPTION("Invalid PNG file.");
    }
      
  png_set_write_fn( png_ptr, (void *)&outfile,
                    claw__graphic__png__target_manager__write,
                    claw__graphic__png__target_manager__flush );

  set_options( png_ptr, info_ptr, opt );
  save_image( png_ptr, info_ptr );
      
  png_destroy_write_struct(&png_ptr, &info_ptr);
} // png::writer::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the parameters of the PNG saving structures.
 * \param png_ptr PNG file description.
 * \param info_ptr PNG file informations.
 * \param opt Compression options.
 */
void claw::graphic::png::writer::set_options
( png_structp png_ptr, png_infop info_ptr, const options& opt ) const
{
  CLAW_PRECOND( png_ptr );
  CLAW_PRECOND( info_ptr );

  png_set_compression_level( png_ptr, opt.compression );

  png_set_IHDR( png_ptr, info_ptr, m_image.width(), m_image.height(),
                sizeof(pixel_type::component_type) * 8, /* 8 bits per byte */
                PNG_COLOR_TYPE_RGB_ALPHA,
                opt.interlace, PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT );
} // png::writer::set_options()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the image in a configured PNG file.
 * \param png_ptr PNG file description.
 * \param info_ptr PNG file informations.
 */
void claw::graphic::png::writer::save_image
( png_structp png_ptr, png_infop info_ptr ) const
{
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

      png_set_rows(png_ptr, info_ptr, data);
      png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
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
} // png::writer::save_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the pixels from the image to an array of bytes.
 * \param data (out) The pixels for the PNG image.
 * \param y Index of the line of the image from which we read the pixels.
 * \pre The memory pointed by data is long enough to store the pixels.
 */
void claw::graphic::png::writer::copy_pixel_line
( png_bytep data, unsigned int y ) const
{
  CLAW_PRECOND( data );
  CLAW_PRECOND( y < m_image.height() );

  // four bytes for each pixel in the line
  for (unsigned int x=0; x!=m_image.width(); ++x, data+=s_rgba_pixel_size)
    {
      data[0] = m_image[y][x].components.red;
      data[1] = m_image[y][x].components.green;
      data[2] = m_image[y][x].components.blue;
      data[3] = m_image[y][x].components.alpha;
    }
} // png::writer::copy_pixel_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the png write structures.
 * \param png_ptr PNG file description.
 * \param info_ptr PNG file informations.
 */
void claw::graphic::png::writer::create_write_structures
( png_structp& png_ptr, png_infop& info_ptr ) const
{
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (png_ptr)
    {
      info_ptr = png_create_info_struct(png_ptr);

      if (!info_ptr)
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    }

  if (!png_ptr || !info_ptr)
    throw CLAW_EXCEPTION("Can't create PNG write structures.");
} // png::writer::create_write_structures()
