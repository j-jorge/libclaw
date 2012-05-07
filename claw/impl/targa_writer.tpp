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
 * \file targa_writer.tpp
 * \brief Implementation of the template methods of the targa::writer class and
 *        subclasses.
 * \author Julien Jorge
 */
#include <limits>
#include <iterator>


//********************* targa::writer::file_output_buffer **********************




  /*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param os The 
 */
template<typename Pixel>
claw::graphic::targa::writer::file_output_buffer<Pixel>::file_output_buffer
( std::ostream& os )
  : m_stream(os)
{

} // targa::writer::file_output_buffer::file_output_buffer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Code a pixel.
 * \param n The number of time the pixel appears.
 * \param pattern The value of the pixel.
 */
template<typename Pixel>
void claw::graphic::targa::writer::file_output_buffer<Pixel>::encode
( unsigned int n, pattern_type pattern )
{
  assert( n <= max_encodable() );
  assert( n >= min_interesting() );

  unsigned char key = (n-1) | 0x80;

  m_stream << key;
  order_pixel_bytes( pattern );
} // targa::writer::file_output_buffer::encode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write raw data int the stream.
 * \param first Iterator on the first data.
 * \param last Iterator past the last data.
 */
template<typename Pixel>
template<typename Iterator>
void claw::graphic::targa::writer::file_output_buffer<Pixel>::raw
( Iterator first, Iterator last )
{
  unsigned int n = std::distance(first, last);

  unsigned int full = n / max_encodable();
  unsigned int remaining = n % max_encodable();

  unsigned char key = max_encodable() - 1;

  for (unsigned int i=0; i!=full; ++i)
    {
      m_stream << key;

      for (unsigned int j=0; j!=max_encodable(); ++j, ++first)
        order_pixel_bytes( *first );
    }

  if (remaining)
    {
      key = remaining - 1;
      m_stream << key;
      
      for (unsigned int j=0; j!=remaining; ++j, ++first)
        order_pixel_bytes( *first );
    }

} // targa::writer::file_output_buffer::raw()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the minimum number of pixels needed for encoding.
 */
template<typename Pixel>
unsigned int
claw::graphic::targa::writer::file_output_buffer<Pixel>::min_interesting() const
{
  return 2;
} // targa::writer::file_output_buffer::min_interesting()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the maximum number of pixel a code can encode.
 */
template<typename Pixel>
unsigned int
claw::graphic::targa::writer::file_output_buffer<Pixel>::max_encodable() const
{
  return 0x80;
} // targa::writer::file_output_buffer::max_encodable()
