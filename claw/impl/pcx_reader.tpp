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
 * \file pcx_reader.tpp
 * \brief Implementation of the template methods of the pcx::reader class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Decompress the scan lines and convert their contents into pixels in
 *        the image.
 * \param h The header read in the file.
 * \param f The file from which we read.
 * \param convert Converter to use to convert the data into pixels.
 */
template<typename Converter>
void claw::graphic::pcx::reader::decompress
( const header& h, std::istream& f, const Converter& convert )
{
  std::vector<color_plane_type> scanline
    ( h.color_planes, color_plane_type(h.bytes_per_line) );

  for ( unsigned int y=0; y!=m_image.height(); ++y)
    {
      for (unsigned int i=0; i!=h.color_planes; ++i)
        decompress_line(f, scanline[i]);

      convert( scanline, m_image, y );
    }
} // pcx::reader::decompress()
