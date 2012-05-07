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
 * \file targa_reader.cpp
 * \brief Implementation of the targa::reader class.
 * \author Julien Jorge
 */
#include <claw/targa.hpp>
#include <claw/exception.hpp>



//********************* targa::reader::file_input_buffer ***********************



namespace claw
{
  namespace graphic
  {
    /*------------------------------------------------------------------------*/
    /**
     * \brief Read a pixel from the file.
     *
     * \remark This method is specialized for the pixels of type
     *         claw::graphic::rgba_pixel_8.
     */
    template< >
    rgba_pixel_8 targa::reader::file_input_buffer<rgba_pixel_8>::get_pixel()
    {
      rgba_pixel_8 result;
      
      if ( this->remaining() < 4 )
        this->read_more(4);
      
      assert( this->remaining() >= 4 );
      
      result.components.blue  = this->get_next();
      result.components.green = this->get_next();
      result.components.red   = this->get_next();
      result.components.alpha = this->get_next();
      
      return result;
    } // targa::reader::file_input_buffer::get_pixel()
  } // namespace graphic
} // namespace claw

namespace claw
{
  namespace graphic
  {
    /*------------------------------------------------------------------------*/
    /**
     * \brief Read a pixel from the file.
     *
     * \remark This method is specialized for the pixels of type
     *         claw::graphic::targa::reader::rgb_pixel_8.
     */
    template< >
    rgba_pixel_8 targa::reader::file_input_buffer<rgb_pixel_8>::get_pixel()
    {
      rgba_pixel_8 result;
      
      if ( this->remaining() < 3 )
        this->read_more(3);
      
      assert( this->remaining() >= 3 );
      
      result.components.blue  = this->get_next();
      result.components.green = this->get_next();
      result.components.red   = this->get_next();
      result.components.alpha =
        std::numeric_limits<claw::graphic::rgba_pixel_8::component_type>::max();
      
      return result;
    } // targa::reader::file_input_buffer::get_pixel()
  } // namespace graphic
} // namespace claw

namespace claw
{
  namespace graphic
  {
    /*------------------------------------------------------------------------*/
    /**
     * \brief Read a pixel from the file.
     *
     * \remark This method is specialized for the pixels of type
     *         claw::graphic::targa::reader::pixel16.
     */
    template< >
    rgba_pixel_8 targa::reader::file_input_buffer<targa::pixel16>::get_pixel()
    {
      rgba_pixel_8 result;

      if ( this->remaining() < 2 )
        this->read_more(2);

      assert( this->remaining() >= 2 );

      unsigned char second_byte  = this->get_next();
      unsigned char first_byte = this->get_next();

      unsigned char r = (first_byte & 0x7C) >> 2;
      unsigned char g =
        ((first_byte & 0x03) << 3) | ((second_byte & 0xE0) >> 5);
      unsigned char b = second_byte & 0x1F;

      result.components.blue  = b * 8;
      result.components.green = g * 8;
      result.components.red   = r * 8;
      result.components.alpha =
        std::numeric_limits<claw::graphic::rgba_pixel_8::component_type>::max();

      return result;
    } // targa::reader::file_input_buffer::get_pixel()
  } // namespace graphic
} // namespace claw







//****************** targa::reader::mapped_file_input_buffer *******************



namespace claw
{
  namespace graphic
  {
    /*------------------------------------------------------------------------*/
    /**
     * \brief Read a pixel from the file.
     *
     * \remark This method is specialized for the pixels of type
     *         claw::graphic::pixel8.
     */
    template< >
    rgba_pixel_8
    targa::reader::mapped_file_input_buffer<targa::pixel8>::get_pixel()
    {
      if ( this->remaining() < 1 )
        this->read_more(1);
      
      assert( this->remaining() >= 1 );
      
      unsigned char index = this->get_next();

      return m_palette[index];
    } // targa::reader::mapped_file_input_buffer::get_pixel()
  } // namespace graphic
} // namespace claw





//****************************** targa::reader *********************************




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which the data will be stored.
 */
claw::graphic::targa::reader::reader( image& img )
  : m_image( img )
{

} // targa::reader::reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which the data will be stored.
 * \param f The file from which we read the data.
 * \post img contains the data from \a f.
 */
claw::graphic::targa::reader::reader( image& img, std::istream& f )
  : m_image( img )
{
  load(f);
} // targa::reader::reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an image from a targa file.
 * \param f Targa file.
 */
void claw::graphic::targa::reader::load( std::istream& f )
{
  CLAW_PRECOND( !!f );
  std::istream::pos_type init_pos = f.tellg();

  try
    {
      check_if_targa(f);

      header h;

      f.read( reinterpret_cast<char*>(&h), sizeof(header) );
      
      if ( f.rdstate() == std::ios_base::goodbit )
        {
          m_image.set_size( h.image_specification.width,
                            h.image_specification.height );
          
          switch(h.image_type)
            {
            case color_mapped: load_color_mapped(h, f); break;
            case rle_color_mapped: load_rle_color_mapped(h, f); break;
            case true_color: load_true_color(h, f); break;
            case rle_true_color: load_rle_true_color(h, f); break;
            default :
              throw claw::bad_format
                ( "targa::reader::targa: unsupported image type" );
            }
        }
      else
        throw claw::bad_format
          ( "claw::targa::reader::targa: can't read header" );
    }
  catch(...)
    {
      f.clear();
      f.seekg( init_pos, std::ios_base::beg );
      throw;
    }
} // targa::reader::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if a stream contains targa data.
 * \param f The stream to check.
 */
void claw::graphic::targa::reader::check_if_targa( std::istream& f ) const
{
  CLAW_PRECOND( !!f );

  std::istream::pos_type init_pos = f.tellg();

  footer foot;

  f.seekg( -(std::istream::off_type)sizeof(footer), std::ios::end );
  f.read( reinterpret_cast<char*>(&foot), sizeof(footer) );
  f.seekg( init_pos , std::ios::beg );
  
  if ( !foot.is_valid() )
    throw CLAW_EXCEPTION( "Not a Targa file." );
} // targa::reader::check_if_targa()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the color palette.
 * \param h File's header, must have been read before call.
 * \param f Targa file.
 * \param palette (out) The color palette.
 * \pre (h.image_type == color_mapped) || (h.image_type == rle_color_mapped)
 */
void claw::graphic::targa::reader::load_palette
( const header& h, std::istream& f, color_palette32& palette ) const
{
  assert((h.image_type == color_mapped) || (h.image_type == rle_color_mapped));

  switch( h.color_map_specification.entry_size )
    {
    case 16: load_palette_content<pixel16>(f, palette); break;
    case 24: load_palette_content<rgb_pixel_8>(f, palette); break;
    case 32: load_palette_content<rgba_pixel_8>(f, palette); break;
    default: 
      throw claw::bad_format
        ( "targa::reader::load_palette: unsupported entry size" );
    }
} // targa::reader::load_palette()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a not compressed color mapped targa file.
 * \param h File's header, must have been read before call.
 * \param f Targa file.
 * \pre h.image_type == color_mapped
 */
void claw::graphic::targa::reader::load_color_mapped
( const header& h, std::istream& f )
{
  assert(h.image_type == color_mapped);

  f.seekg( h.id_length, std::ios_base::cur );

  color_palette32 palette( h.color_map_specification.length );
  load_palette( h, f, palette );

  switch(h.image_specification.bpp)
    {
    case 8: load_color_mapped_raw<pixel8>(h, f, palette); break;
    default: 
      throw claw::bad_format
        ( "targa::reader::load_color_mapped: unsupported color depth" );
    }
} // targa::reader::load_color_mapped()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a RLE color mapped targa file.
 * \param h File's header, must have been read before call.
 * \param f Targa file.
 * \pre h.image_type == color_mapped
 */
void claw::graphic::targa::reader::load_rle_color_mapped
( const header& h, std::istream& f )
{
  assert(h.image_type == rle_color_mapped);

  f.seekg( h.id_length, std::ios_base::cur );

  color_palette32 palette( h.color_map_specification.length );
  load_palette( h, f, palette );

  switch(h.image_specification.bpp)
    {
    case 8: decompress_rle_color_mapped<rle8_decoder>(h, f, palette); break;
    default: 
      throw claw::bad_format
        ( "targa::reader::load_rle_color_mapped: unsupported color depth" );
    }
} // targa::reader::load_color_mapped()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a not compressed true color targa file.
 * \param h File's header, must have been read before call.
 * \param f Targa file.
 * \pre h.image_type == true_color
 */
void claw::graphic::targa::reader::load_true_color
( const header& h, std::istream& f )
{
  assert(h.image_type == true_color);

  f.seekg( h.id_length, std::ios_base::cur );

  switch(h.image_specification.bpp)
    {
    case 16 : load_true_color_raw<pixel16>(h, f); break;
    case 24 : load_true_color_raw<rgb_pixel_8>(h, f); break;
    case 32 : load_true_color_raw<rgba_pixel_8>(h, f); break;
    default : 
      throw claw::bad_format
        ( "targa::reader::load_true_color: unsupported color depth" );
    }
} // targa::reader::load_true_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a RLE true color targa file.
 * \param h File's header, must have been read before call.
 * \param f Targa file.
 * \pre h.image_type == rle_true_color
 */
void claw::graphic::targa::reader::load_rle_true_color
( const header& h, std::istream& f )
{
  assert(h.image_type == rle_true_color);

  f.seekg( h.id_length, std::ios_base::cur );

  switch(h.image_specification.bpp)
    {
    case 16 : decompress_rle_true_color<rle16_decoder>(h, f); break;
    case 24 : decompress_rle_true_color<rle24_decoder>(h, f); break;
    case 32 : decompress_rle_true_color<rle32_decoder>(h, f); break;
    default : 
      throw claw::bad_format
        ( "targa::reader::load_rle_true_color: unsupported color depth" );
    }
} // targa::reader::load_rle_true_color()

