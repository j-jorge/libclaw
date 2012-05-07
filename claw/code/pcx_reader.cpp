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
 * \file pcx_reader.cpp
 * \brief Implementation of the pcx::reader class.
 * \author Julien Jorge
 */
#include <claw/pcx.hpp>
#include <claw/exception.hpp>

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Converts a scan line of a monochrome pcx into 32 bpp pixels.
 * \param scanline the scan line to convert.
 * \param img The image in which we write the results.
 * \param y The line of \a img concerned by the pixels.
 */
void claw::graphic::pcx::reader::converter_mono::operator()
( const std::vector<color_plane_type>& scanline, image& img,
  unsigned int y ) const
{
  CLAW_PRECOND( scanline.size() == 1 );

  unsigned int x=0;

  for ( unsigned int code=0; x!=img.width(); ++code )
    {
      u_int_8 c = scanline[0][code]; // only one color plane for monochrome pcx

      for( unsigned int i=0; (i!=8) && (x!=img.width()); ++x, ++i, c<<=1 )
        if ( c & 0x80 )
          img[y][x] = white_pixel;
        else
          img[y][x] = black_pixel;
    }
} // pcx::reader::converter_mono::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param h The header of the pcx file (contains the color palette).
 */
claw::graphic::pcx::reader::converter_16::converter_16( const header& h )
  : m_header(h)
{

} // pcx::reader::converter_16::converter_16()

/*----------------------------------------------------------------------------*/
/**
 * \brief Converts a scan line of a 4 bpp color mapped pcx into 32 bpp pixels.
 * \param scanline the scan line to convert.
 * \param img The image in which we write the results.
 * \param y The line of \a img concerned by the pixels.
 */
void claw::graphic::pcx::reader::converter_16::operator()
( const std::vector<color_plane_type>& scanline, image& img,
  unsigned int y ) const
{
  CLAW_PRECOND( scanline.size() == 4 );

  unsigned int x=0;

  for ( unsigned int code=0; x!=img.width(); ++code )
    {
      u_int_8 c0 = scanline[0][code];
      u_int_8 c1 = scanline[1][code];
      u_int_8 c2 = scanline[2][code];
      u_int_8 c3 = scanline[3][code];

      for( unsigned int i=0; (i!=8) && (x!=img.width()); ++x, ++i )
        {
          unsigned int index =
            ( (c3 & 0x80) >> 4 )
            | ( (c2 & 0x80) >> 5 )
            | ( (c1 & 0x80) >> 6 )
            | ( (c0 & 0x80) >> 7 );

          img[y][x] = m_header.color_map[index];

          c0 <<= 1;
          c1 <<= 1;
          c2 <<= 1;
          c3 <<= 1;
        }
    }
} // pcx::reader::converter_16::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param palette The color palette of the image.
 */
claw::graphic::pcx::reader::converter_256::converter_256
( const color_palette32& palette )
  : m_palette(palette)
{

} // pcx::reader::converter_256::converter_256()

/*----------------------------------------------------------------------------*/
/**
 * \brief Converts a scan line of a 8 bpp color mapped pcx into 32 bpp pixels.
 * \param scanline the scan line to convert.
 * \param img The image in which we write the results.
 * \param y The line of \a img concerned by the pixels.
 */
void claw::graphic::pcx::reader::converter_256::operator()
( const std::vector<color_plane_type>& scanline, image& img,
  unsigned int y ) const
{
  CLAW_PRECOND( scanline.size() == 1 );

  for ( unsigned int x=0; x!=img.width(); ++x )
    img[y][x] = m_palette[ scanline[0][x] ];
} // pcx::reader::converter_256::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Converts a scan line of a 3 planes true color pcx into 32 bpp pixels.
 * \param scanline the scan line to convert.
 * \param img The image in which we write the results.
 * \param y The line of \a img concerned by the pixels.
 */
void claw::graphic::pcx::reader::converter_true_color::operator()
( const std::vector<color_plane_type>& scanline, image& img,
  unsigned int y ) const
{
  CLAW_PRECOND( scanline.size() == 3 );

  for ( unsigned int x=0; x!=img.width(); ++x )
    {
      img[y][x].components.red = scanline[0][x];
      img[y][x].components.green = scanline[1][x];
      img[y][x].components.blue = scanline[2][x];
      img[y][x].components.alpha =
        std::numeric_limits<rgba_pixel_8::component_type>::max();
    }
} // pcx::reader::converter_true_color::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param result The scan line in which we write the uncompressed data.
 * \pre \a result is big enough to store the uncompressed data.
 */
claw::graphic::pcx::reader::rle_pcx_output_buffer::rle_pcx_output_buffer
( color_plane_type& result )
  : m_result(result), m_position(0)
{

} // pcx::reader::rle_pcx_output_buffer::rle_pcx_output_buffer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy a pixel a certain number of times.
 * \param n The number of pixel to write.
 * \param pattern The pixel to copy.
 */
void claw::graphic::pcx::reader::rle_pcx_output_buffer::fill
( unsigned int n, u_int_8 pattern )
{
  CLAW_PRECOND( m_position + n <= m_result.size() );

  for (unsigned int i=0; i!=n; ++i)
    m_result[m_position + i] = pattern;

  m_position += n;
} // pcx::reader::rle_pcx_output_buffer::fill()

/*----------------------------------------------------------------------------*/
/**
 * \brief Direct copy of a certain number of pixels from the file.
 * \param n The number of pixels to write.
 * \param buffer The buffer from which we read.
 */
void claw::graphic::pcx::reader::rle_pcx_output_buffer::copy
( unsigned int n, rle_pcx_input_buffer& buffer )
{
  CLAW_ASSERT( false, "This method should not have been called" );
} // pcx::reader::rle_pcx_output_buffer::copy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we have completely filled the buffer.
 */
bool claw::graphic::pcx::reader::rle_pcx_output_buffer::completed() const
{
  return m_position == m_result.size();
} // pcx::reader::rle_pcx_output_buffer::completed()





/*----------------------------------------------------------------------------*/
/**
 * \brief Get the type of the following data in the input buffer.
 * \param input The input stream (the pcx file).
 * \param output The output stream (the pcx image).
 */
void claw::graphic::pcx::reader::rle_pcx_decoder::read_mode
( input_buffer_type& input, output_buffer_type& output )
{
  this->m_mode = this->stop;
  bool ok = !output.completed();

  if ( ok && (input.remaining() < 1) )
    ok = input.read_more(1);

  if (ok)
    {
      unsigned char key = input.get_next();
      this->m_mode = this->compressed;
      
      if ( (key & 0xC0) == 0xC0 )
        {
          this->m_count = key & 0x3F;

          if ( input.remaining() < 1 )
            input.read_more(1);

          this->m_pattern = input.get_next();
        }
      else
        {
          this->m_count = 1;
          this->m_pattern = key;
        }
    }
} // pcx::reader::rle_pcx_decoder::read_mode()






/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which the data will be stored.
 */
claw::graphic::pcx::reader::reader( image& img )
  : m_image( img )
{

} // pcx::reader::reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which the data will be stored.
 * \param f The file from which we read the data.
 * \post img contains the data from \a f.
 */
claw::graphic::pcx::reader::reader( image& img, std::istream& f )
  : m_image( img )
{
  load(f);
} // pcx::reader::reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an image from a pcx file.
 * \param f Pcx file.
 */
void claw::graphic::pcx::reader::load( std::istream& f )
{
  CLAW_PRECOND( !!f );
  std::istream::pos_type init_pos = f.tellg();

  try
    {
      header h;

      f.read( reinterpret_cast<char*>(&h), sizeof(header) );
      
      if ( f.rdstate() == std::ios_base::goodbit )
        {
          check_if_pcx(h);

          m_image.set_size( h.window.x_max - h.window.x_min + 1,
                            h.window.y_max - h.window.y_min + 1 );

          bool supported_format = true;

          switch(h.color_planes)
            {
            case 1:
              if (h.bpp == 1)
                load_mono(h, f);
              else if (h.bpp == 8)
                load_256_color_mapped(h, f);
              else
                supported_format = false;
              break;
            case 3:
              if (h.bpp == 8)
                load_true_color(h, f);
              else
                supported_format = false;
              break;
            case 4:
              if (h.bpp == 1)
                load_16_color_mapped(h, f);
              else
                supported_format = false;
              break;
            default :
              supported_format = false;
            }

          if ( supported_format == false )
            throw claw::bad_format
              ( "pcx::reader::pcx: unsupported image type" );
        }
      else
        throw claw::bad_format
          ( "claw::pcx::reader::pcx: can't read header" );
    }
  catch(...)
    {
      f.clear();
      f.seekg( init_pos, std::ios_base::beg );
      throw;
    }
} // pcx::reader::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if an header seems valid for a pcx file.
 * \param h The header read in the file.
 */
void claw::graphic::pcx::reader::check_if_pcx( const header& h ) const
{
  if ( h.manufacturer != 0x0A )
    throw CLAW_EXCEPTION( "Not a Pcx file." );
} // pcx::reader::check_if_pcx()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the monochrome pcx content.
 * \param h The header read in the file.
 * \param f The file from which we read.
 */
void claw::graphic::pcx::reader::load_mono( const header& h, std::istream& f )
{
  assert( h.color_planes == 1 );

  converter_mono convert;
  decompress( h, f, convert );
} // pcx::reader::load_mono()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the 16 bpp color mapped pcx content.
 * \param h The header read in the file.
 * \param f The file from which we read.
 */
void claw::graphic::pcx::reader::load_16_color_mapped
( const header& h, std::istream& f )
{
  assert( h.color_planes == 4 );

  converter_16 convert(h);
  decompress( h, f, convert );
} // pcx::reader::load_16_color_mapped()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the true color (24 bpp) pcx content.
 * \param h The header read in the file.
 * \param f The file from which we read.
 */
void
claw::graphic::pcx::reader::load_true_color( const header& h, std::istream& f )
{
  assert( h.color_planes == 3 );

  converter_true_color convert;
  decompress( h, f, convert );
} // pcx::reader::load_true_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the 8 bpp color mapped pcx content.
 * \param h The header read in the file.
 * \param f The file from which we read.
 */
void claw::graphic::pcx::reader::load_256_color_mapped
( const header& h, std::istream& f )
{
  assert( h.color_planes == 1 );

  // 256 RGB triplets
  const unsigned int palette_length = 256 * 3;

  color_palette32 palette(256);
  std::istream::pos_type init_pos = f.tellg();

  // -1 for the check byte
  f.seekg( -(std::istream::off_type)palette_length - 1, std::ios_base::end ); 

  char check;
  f.read(&check, 1);
  
  if ( check != 12 )
    throw CLAW_EXCEPTION( "PCX: The color palette is missing." );

  char buffer[palette_length];
  f.read(buffer, palette_length);

  for (unsigned int i=0, j=0; i!=palette_length; i+=3, ++j)
    {
      palette[j].components.alpha = 255;
      palette[j].components.red   = buffer[i];
      palette[j].components.green = buffer[i+1];
      palette[j].components.blue  = buffer[i+2];
    }

  f.seekg( init_pos );
  converter_256 convert(palette);
  decompress( h, f, convert );
} // pcx::reader::load_256_color_mapped()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the 8 bpp color mapped pcx content.
 * \param f The file from which we read.
 * \param scanline (out) Uncompressed scan line.
 */
void claw::graphic::pcx::reader::decompress_line
( std::istream& f, color_plane_type& scanline ) const
{
  rle_pcx_input_buffer input(f);
  rle_pcx_output_buffer output(scanline);

  rle_pcx_decoder decoder;

  decoder.decode( input, output );
} // pcx::reader::decompress_line()
