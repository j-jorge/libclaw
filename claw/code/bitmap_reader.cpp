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
 * \file bitmap_reader.cpp
 * \brief Implementation of the claw::graphic::bitmap::reader class.
 * \author Julien Jorge
 */
#include <claw/bitmap.hpp>
#include <algorithm>

namespace claw
{
  namespace graphic
  {
    /*------------------------------------------------------------------------*/
    /**
     * \brief Copy a color a certain number of times.
     * \param n Number of copies.
     * \param pattern The index of the color to copy.
     * \remark This method is for 8bpp bitmaps.
     */
    template<>
    void bitmap::reader::rle_bitmap_output_buffer<false>::fill
    ( unsigned int n, unsigned char pattern )
    {
      assert( m_x + n <= m_image.width() );
      
      std::fill(&m_image[m_y][m_x], &m_image[m_y][m_x] + n, m_palette[pattern]);
      
      m_x += n;
    } // bitmap::reader::rle_bitmap_output_buffer<false>::fill()
  } // namespace graphic
} // namespace claw

namespace claw
{
  namespace graphic
  {
    /*------------------------------------------------------------------------*/
    /**
     * \brief Copy a pair of colors a certain number of times.
     * \param n Number of pixels to fill.
     * \param pattern The index of the two colors to copy.
     * \remark This method is for 4bpp bitmaps.
     */
    template<>
    void bitmap::reader::rle_bitmap_output_buffer<true>::fill
    ( unsigned int n, unsigned char pattern )
    {
      assert( m_x + n <= m_image.width() );
      
      for (unsigned int i = 0; i != n / 2; ++i, m_x += 2)
        {
          m_image[m_y][m_x] = m_palette[ (pattern & 0xF0) >> 4 ];
          m_image[m_y][m_x+1] = m_palette[ pattern & 0x0F ];
        }
      
      if ( n % 2 )
        {
          m_image[m_y][m_x] = m_palette[ (pattern & 0xF0) >> 4 ];
          ++m_x;
        }
    } // bitmap::reader::rle_bitmap_output_buffer<false>::fill()
  } // namespace graphic
} // namespace claw

namespace claw
{
  namespace graphic
  {
    /*------------------------------------------------------------------------*/
    /**
     * \brief Copy some pixels directly.
     * \param n The number of pixels to copy.
     * \param buffer The input buffer, to copy from.
     * \remark This method is for 8bpp bitmaps.
     */
    template<>
    void bitmap::reader::rle_bitmap_output_buffer<false>::copy
    ( unsigned int n, file_input_buffer& buffer )
    {
      assert( m_x + n <= m_image.width() );
      
      // RLE bitmap data is 2-bytes aligned
      const unsigned int bytes_needed = n + n % 2;
      
      if ( buffer.remaining() < bytes_needed )
        buffer.read_more(bytes_needed);
      
      assert( buffer.remaining() >= bytes_needed );
      
      const unsigned char* p =
        reinterpret_cast<const unsigned char*>(buffer.get_buffer());
      
      std::transform( p, p + n, &m_image[m_y][m_x], m_palette );
      
      m_x += n;

      buffer.move(bytes_needed);
    } // bitmap::reader::rle_bitmap_output_buffer<false>::copy()
  } // namespace graphic
} // namespace claw

namespace claw
{
  namespace graphic
  {
    /*------------------------------------------------------------------------*/
    /**
     * \brief Copy some pixels directly.
     * \param n The number of pixels to copy.
     * \param buffer The input buffer, to copy from.
     * \remark This method is for 4bpp bitmaps.
     */
    template<>
    void bitmap::reader::rle_bitmap_output_buffer<true>::copy
    ( unsigned int n, file_input_buffer& buffer )
    {
      assert( m_x + n <= m_image.width() );

      // RLE bitmap data is 2-bytes aligned
      unsigned int bytes_needed = n / 2 + n % 2;

      if ( bytes_needed % 2 )
        ++bytes_needed;

      if ( buffer.remaining() < bytes_needed )
        buffer.read_more( bytes_needed );

      assert( buffer.remaining() >= bytes_needed );

      const unsigned char* p =
        reinterpret_cast<const unsigned char*>(buffer.get_buffer());
      const unsigned char* last = p + n / 2;

      for ( ; p != last; ++p, m_x += 2)
        {
          m_image[m_y][m_x] = m_palette[ (*p & 0xF0) >> 4 ];
          m_image[m_y][m_x+1] = m_palette[ *p & 0x0F ];
        }

      if ( n % 2 )
        {
          m_image[m_y][m_x] = m_palette[ (*p & 0xF0) >> 4 ];
          ++m_x;
        }

      buffer.move( bytes_needed );
    } // bitmap::reader::rle_bitmap_output_buffer<true>::copy()
  } // namespace graphic
} // namespace claw



/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a bitset array to a pixel32 scanline.
 * \param dest (out) Filled scanline.
 * \param src Pixel array to convert.
 * \param palette Color palette.
 * \pre palette.size() == 2
 */
void claw::graphic::bitmap::reader::pixel1_to_pixel32::operator()
  ( scanline& dest, const char* src, const color_palette_type& palette ) const
{
  assert(palette.size() == 2);

  scanline::iterator it( dest.begin() );
  const unsigned int n = dest.size();
  const unsigned int byte_size = 8; // 8 bits per byte
  const unsigned int upper_bound = n / byte_size;

  for (unsigned int i=0; i!=upper_bound; ++i)
    for (unsigned int j=0; j!=byte_size; ++j, ++it)
      if ( src[i] & (0x80 >> j) )
        *it = palette[1];
      else
        *it = palette[0];

  for (unsigned int j = 0; j != (n % byte_size); ++j, ++it)
    if ( src[upper_bound] & (0x80 >> j) )
      *it = palette[1];
    else
      *it = palette[0];
} // bitmap::reader::pixel1_to_pixel32()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a 4bpp array to a pixel32 scanline.
 * \param dest (out) Filled scanline.
 * \param src Pixel array to convert.
 * \param palette Color palette.
 * \pre palette.size() == 16
 */
void claw::graphic::bitmap::reader::pixel4_to_pixel32::operator()
  ( scanline& dest, const char* src, const color_palette_type& palette ) const
{
  assert(palette.size() == 16);

  scanline::iterator it( dest.begin() );
  const unsigned int upper_bound = dest.size() / 2;

  for (unsigned int i=0; i!=upper_bound; ++i, ++src)
    {
      *it = palette[ (*src & 0xF0) >> 4 ];
      ++it;
      *it = palette[ *src & 0x0F ];
      ++it;
    }

  if (dest.size() % 2)
    *it = palette[ (*src & 0xF0) >> 4 ];
} // bitmap::reader::pixel4_to_pixel32()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a 8bpp array to a pixel32 scanline.
 * \param dest (out) Filled scanline.
 * \param src Pixel array to convert.
 * \param palette Color palette.
 * \pre palette.size() == 256
 */
void claw::graphic::bitmap::reader::pixel8_to_pixel32::operator()
  ( scanline& dest, const char* src, const color_palette_type& palette ) const
{
  assert(palette.size() == 256);

  const unsigned char* s = reinterpret_cast<const unsigned char*>(src);

  std::transform(s, s + dest.size(), dest.begin(), palette);
} // bitmap::reader::pixel8_to_pixel32()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a BGR array to a pixel32 scanline.
 * \param dest (out) Filled scanline.
 * \param src Pixel array to convert.
 * \param palette (not used) Color palette.
 */
void claw::graphic::bitmap::reader::pixel24_to_pixel32::operator()
  ( scanline& dest, const char* src, const color_palette_type& palette ) const
{
  scanline::iterator it( dest.begin() );
  const unsigned int upper_bound = 3 * dest.size();

  for (unsigned int i=0; i!=upper_bound; i+=3)
    {
      it->components.alpha = 255;
      it->components.blue  = src[i];
      it->components.green = src[i+1];
      it->components.red   = src[i+2];

      ++it;
    }
} // bitmap::reader::pixel24_to_pixel32()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which the data will be stored.
 */
claw::graphic::bitmap::reader::reader( image& img )
  : m_image( img )
{

} // bitmap::reader::reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which the data will be stored.
 * \param f The file from which we read the data.
 * \post img contains the data from \a f.
 */
claw::graphic::bitmap::reader::reader( image& img, std::istream& f )
  : m_image( img )
{
  load(f);
} // bitmap::reader::reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the image data from a stream.
 * \param f The file from which we read the data.
 * \post The image passed to the constructor contains the data from \a f.
 */
void claw::graphic::bitmap::reader::load( std::istream& f )
{
  CLAW_PRECOND( !!f );
  std::istream::pos_type init_pos = f.tellg();

  try
    {
      header h;

      f.read( reinterpret_cast<char*>(&h), sizeof(header) );

      if ( (h.id[0] == 'B') && (h.id[1] == 'M')
           && (f.rdstate() == std::ios_base::goodbit) )
        {
          m_image.set_size(h.width, h.height);
      
          switch(h.bpp)
            {
            case 1 : load_1bpp(h, f); break;
            case 4 : load_4bpp(h, f); break;
            case 8 : load_8bpp(h, f); break;
              //case 16 : load_16bpp(h, f); break;
            case 24 : load_24bpp(h, f); break;
            default : 
              throw claw::bad_format
                ("bitmap::bitmap: unsupported color depth.");
            }
        }
      else
        throw claw::bad_format( "bitmap::bitmap: invalid header." );
    }
  catch(...)
    {
      f.clear();
      f.seekg( init_pos, std::ios_base::beg );
      throw;
    }
} // bitmap::reader::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the palette of the image.
 * \param h File's header, must have been read before call.
 * \param f Bitmap file.
 * \param palette The loaded palette.
 * \pre h.bpp <= 8
 */
void claw::graphic::bitmap::reader::load_palette
( const header& h, std::istream& f, color_palette_type& palette ) const
{
  assert(h.bpp <= 8);

  switch(h.bpp)
    {
    case 1 : assert( palette.size() == 2 ); break;
    case 4 : assert( palette.size() == 16 ); break;
    case 8 : assert( palette.size() == 256 ); break;
    }

  const unsigned int sizeof_color = sizeof(color_palette_type::color_type);
  const unsigned int buffer_size = sizeof_color * palette.size();
  char* buffer = new char[buffer_size];

  f.read(buffer, buffer_size);

  for (unsigned int i=0, j=0; i!=buffer_size; i+=sizeof_color, ++j)
    {
      palette[j].components.alpha = 255;
      palette[j].components.blue  = buffer[i];
      palette[j].components.green = buffer[i+1];
      palette[j].components.red   = buffer[i+2];
    }

  delete[] buffer;
} // bitmap::reader::load_palette()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a monochrome bitmap file.
 * \param h File's header, must have been read before call.
 * \param f Bitmap file.
 * \pre h.bpp == 1
 */
void
claw::graphic::bitmap::reader::load_1bpp( const header& h, std::istream& f )
{
  assert(h.bpp == 1);
  //assert(h.compression == BMP_COMPRESSION_BITFIELDS);

  color_palette_type palette(2);
  unsigned int buffer_size = m_image.width() / (sizeof(char) * 8);
        
  if ( m_image.width() % (sizeof(char) * 8) )
    ++buffer_size;
    
  load_palette(h, f, palette);
  f.seekg(h.data_offset);

  load_rgb_data(f, buffer_size, palette, pixel1_to_pixel32());
} // bitmap::reader::load_1bpp()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads a 4 bpp bitmap file.
 * \param h File's header, must have been read before call.
 * \param f Bitmap file.
 * \pre (h.bpp == 4)
 */
void
claw::graphic::bitmap::reader::load_4bpp( const header& h, std::istream& f )
{
  assert(h.bpp == 4);
  assert( (h.compression == BMP_COMPRESSION_RGB)
          || (h.compression == BMP_COMPRESSION_RLE4) );

  color_palette_type palette(16);
  load_palette(h, f, palette);

  if (h.compression == BMP_COMPRESSION_RLE4)
    load_4bpp_rle(h, f, palette);
  else
    load_4bpp_rgb(h, f, palette);
} // bitmap::reader::load_4bpp()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads a 8 bpp bitmap file.
 * \param h File's header, must have been read before call.
 * \param f Bitmap file.
 * \pre (h.bpp == 8)
 */
void
claw::graphic::bitmap::reader::load_8bpp( const header& h, std::istream& f )
{
  assert(h.bpp == 8);
  assert( (h.compression == BMP_COMPRESSION_RGB)
          || (h.compression == BMP_COMPRESSION_RLE8) );

  color_palette_type palette(256);
  load_palette(h, f, palette);

  if (h.compression == BMP_COMPRESSION_RLE8)
    load_8bpp_rle(h, f, palette);
  else
    load_8bpp_rgb(h, f, palette);
} // bitmap::reader::load_8bpp()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads a 24 bpp bitmap file.
 * \param h File's header, must have been read before call.
 * \param f Bitmap file.
 * \pre (h.bpp == 24)
 */
void
claw::graphic::bitmap::reader::load_24bpp( const header& h, std::istream& f )
{
  assert(h.bpp == 24);

  unsigned int buffer_size = m_image.width() * 3;
  color_palette_type palette(0);
        
  f.seekg(h.data_offset);

  load_rgb_data(f, buffer_size, palette, pixel24_to_pixel32());
} // bitmap::reader::load_24bpp()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads a 4 bpp RLE encoded bitmap file.
 * \param h File's header and palette, must have been read before call.
 * \param f Bitmap file.
 * \param palette The color palette to use for converting colors.
 * \pre (h.bpp == 4) && (h.compression = BMP_COMPRESSION_RLE4)
 *      && (palette.size() == 16)
 */
void claw::graphic::bitmap::reader::load_4bpp_rle
( const header& h, std::istream& f, const color_palette_type& palette )
{
  assert(h.bpp == 4);
  assert(h.compression == BMP_COMPRESSION_RLE4);
  assert(palette.size() == 16);

  f.seekg(h.data_offset);

  rle4_decoder decoder;
  rle4_decoder::output_buffer_type output_buffer( palette, m_image );
  file_input_buffer input_buffer(f);

  decoder.decode( input_buffer, output_buffer );
} // bitmap::reader::load_4bpp_rle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads a 4 bpp RGB encoded bitmap file.
 * \param h File's header and palette, must have been read before call.
 * \param f Bitmap file.
 * \param palette The color palette to use for converting colors.
 * \pre (h.bpp == 4) && (h.compression = BMP_COMPRESSION_RGB)
 *      && (palette.size() == 16)
 */
void claw::graphic::bitmap::reader::load_4bpp_rgb
( const header& h, std::istream& f, const color_palette_type& palette )
{
  assert(h.bpp == 4);
  assert(h.compression == BMP_COMPRESSION_RGB);
  assert(palette.size() == 16);

  unsigned int buffer_size = m_image.width() / 2 + m_image.width() % 2;
    
  f.seekg(h.data_offset);

  load_rgb_data(f, buffer_size, palette, pixel4_to_pixel32());
} // bitmap::reader::load_4bpp_rgb()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads a 8 bpp bitmap file.
 * \param h File's header and palette, must have been read before call.
 * \param f Bitmap file.
 * \param palette The color palette to use for converting colors.
 * \pre (h.bpp == 8) && (h.compression = BMP_COMPRESSION_RLE8)
 *      && (palette.size() == 256)
 */
void claw::graphic::bitmap::reader::load_8bpp_rle
( const header& h, std::istream& f, const color_palette_type& palette )
{
  assert(h.bpp == 8);
  assert(h.compression == BMP_COMPRESSION_RLE8);
  assert(palette.size() == 256);

  f.seekg(h.data_offset);

  rle8_decoder decoder;
  rle8_decoder::output_buffer_type output_buffer( palette, m_image );
  file_input_buffer input_buffer(f);

  decoder.decode( input_buffer, output_buffer );
} // bitmap::reader::load_8bpp_rle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads a 8 bpp RGB encoded bitmap file.
 * \param h File's header and palette, must have been read before call.
 * \param f Bitmap file.
 * \param palette The color palette to use for converting colors.
 * \pre (h.bpp == 8) && (h.compression = BMP_COMPRESSION_RGB)
 *      && (palette.size() == 256)
 */
void claw::graphic::bitmap::reader::load_8bpp_rgb
( const header& h, std::istream& f, const color_palette_type& palette )
{
  assert(h.bpp == 8);
  assert(h.compression == BMP_COMPRESSION_RGB);
  assert(palette.size() == 256);

  unsigned int buffer_size = m_image.width();
    
  f.seekg(h.data_offset);

  load_rgb_data(f, buffer_size, palette, pixel8_to_pixel32());
} // bitmap::reader::load_8bpp_rgb()
