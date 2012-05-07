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
 * \file xbm_writer.cpp
 * \brief Implementation of the claw::graphic::xbm::writer class.
 * \author Julien Jorge
 */
#include <claw/xbm.hpp>
#include <iomanip>

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
claw::graphic::xbm::writer::options::options()
  : name("noname"), hot(NULL)
{

} // xbm::writer::options::options()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param n The name of the image structure in the file.
 * \param h The position of the hot spot in the image.
 */
claw::graphic::xbm::writer::options::options
( const std::string& n, const claw::math::coordinate_2d<int>* h )
  : name(n), hot(h)
{

} // xbm::writer::options::options()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which the data will be stored.
 */
claw::graphic::xbm::writer::writer( const image& img )
  : m_image( img )
{

} // xbm::writer::writer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image to save.
 * \param f The file in which we write the data.
 * \param opt Saving options.
 */
claw::graphic::xbm::writer::writer
( const image& img, std::ostream& f, const options& opt )
  : m_image( img )
{
  save(f, opt);
} // xbm::writer::writer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the image in a XBM file.
 * \param f XBM file.
 * \param opt Saving options.
 */
void
claw::graphic::xbm::writer::save( std::ostream& f, const options& opt ) const
{
  CLAW_PRECOND( !!f );

  f << "#define " << opt.name << "_width " << m_image.width() << "\n";
  f << "#define " << opt.name << "_height " << m_image.height() << "\n";

  if ( opt.hot != NULL )
    {
      f << "#define " << opt.name << "_x_hot " << opt.hot->x << "\n";
      f << "#define " << opt.name << "_y_hot " << opt.hot->y << "\n";
    }

  f << "static unsigned char " << opt.name << "_bits[] = {\n ";

  save_bits(f);
} // xbm::writer::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the pixels of the image in a XBM file.
 * \param f XBM file.
 */
void claw::graphic::xbm::writer::save_bits( std::ostream& f ) const
{
  const unsigned int max_per_line = (80 - 1) / 6;
  const unsigned int nb_pxl = m_image.width() * m_image.height();

  unsigned int pxl_count = 0;
  unsigned int per_line = 0;

  for (unsigned int y=0; y!=m_image.height(); ++y)
    {
      unsigned int x=0;

      while ( x!=m_image.width() )
        {
          unsigned int v(0);
          unsigned int bits;

          for ( bits=0; (x!=m_image.width()) && (bits != 8);
                ++bits, ++x, ++pxl_count )
            {
              v >>= 1;
              if ( m_image[y][x].luminosity() <= 127 )
                v |= 0x80;
            }

          v >>= 8 - bits;

          ++per_line;

          f << " 0x" << std::setw(2) << std::setfill('0') << std::hex << v;

          if ( pxl_count != nb_pxl )
            {
              f  << ",";

              if ( per_line == max_per_line )
                {
                  f << "\n ";
                  per_line = 0;
                }
            }
        }
    }

  f << "};" << std::endl;
} // xbm::writer::save()
