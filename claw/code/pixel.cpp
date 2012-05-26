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
 * \file pixel.cpp
 * \brief Implementation of the pixel classes.
 * \author Julien Jorge
 */
#include <claw/pixel.hpp>

#include <claw/types.hpp>

#include <stdexcept>
#include <limits>
#include <climits>
#include <sstream>

namespace claw
{
  namespace graphic
  {
    /** \ingroup Colors.
     *  \{
     */
    rgba_pixel transparent_pixel( 0, 0, 0, 0 );

    rgba_pixel black_pixel
    ( 0, 0, 0, std::numeric_limits<rgba_pixel::component_type>::max() );
    rgba_pixel white_pixel
    ( std::numeric_limits<rgba_pixel::component_type>::max(),
      std::numeric_limits<rgba_pixel::component_type>::max(),
      std::numeric_limits<rgba_pixel::component_type>::max(),
      std::numeric_limits<rgba_pixel::component_type>::max() );

    rgba_pixel blue_pixel
    ( 0, 0, std::numeric_limits<rgba_pixel::component_type>::max(),
      std::numeric_limits<rgba_pixel::component_type>::max() );
    rgba_pixel green_pixel
    ( 0, std::numeric_limits<rgba_pixel::component_type>::max(), 0,
      std::numeric_limits<rgba_pixel::component_type>::max() );
    rgba_pixel red_pixel
    ( std::numeric_limits<rgba_pixel::component_type>::max(), 0, 0,
      std::numeric_limits<rgba_pixel::component_type>::max() );

    rgba_pixel yellow_pixel
    ( std::numeric_limits<rgba_pixel::component_type>::max(),
      std::numeric_limits<rgba_pixel::component_type>::max(), 0,
      std::numeric_limits<rgba_pixel::component_type>::max() );
    rgba_pixel magenta_pixel
    ( std::numeric_limits<rgba_pixel::component_type>::max(), 0,
      std::numeric_limits<rgba_pixel::component_type>::max(),
      std::numeric_limits<rgba_pixel::component_type>::max() );
    rgba_pixel cyan_pixel
    ( 0, std::numeric_limits<rgba_pixel::component_type>::max(),
      std::numeric_limits<rgba_pixel::component_type>::max(),
      std::numeric_limits<rgba_pixel::component_type>::max() );

    /** \} */

  } // namespace graphic
} // namespace claw

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
claw::graphic::rgb_pixel::rgb_pixel()
{

} // rgb_pixel::rgb_pixel()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param r The value of the red field.
 * \param g The value of the green field.
 * \param b The value of the blue field.
 */
claw::graphic::rgb_pixel::rgb_pixel
( component_type r, component_type g, component_type b )
{ 
  components.red = r;
  components.green = g;
  components.blue = b;
} // rgb_pixel::rgb_pixel()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p An RGBA pixel.
 */
claw::graphic::rgb_pixel::rgb_pixel( const rgba_pixel& p )
{ 
  components.red = p.components.red;
  components.green = p.components.green;
  components.blue = p.components.blue;
} // rgb_pixel::rgb_pixel()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor from a color name or code.
 * \param c The color name or code.
 */
claw::graphic::rgb_pixel::rgb_pixel( const std::string& c )
{
  std::istringstream iss(c);
  u_int_32 color;

  if ( c[0] == '#' )
    iss.ignore(1);

  if ( !(iss >> std::hex >> color) )
    throw std::invalid_argument(c);

  components.red = (color & 0xFF0000) >> (CHAR_BIT * 2);
  components.green = (color & 0x00FF00) >> CHAR_BIT;
  components.blue = color & 0x0000FF;
} // rgb_pixel::rgb_pixel()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare to a pixel.
 * \param that The pixel to compare to.
 */
bool claw::graphic::rgb_pixel::operator==(const rgb_pixel& that) const
{ 
  return (components.red == that.components.red)
    && (components.green == that.components.green)
    && (components.blue == that.components.blue);
} // rgb_pixel::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare to a pixel.
 * \param that The pixel to compare to.
 */
bool claw::graphic::rgb_pixel::operator==(const rgba_pixel& that) const
{ 
  return *this == rgb_pixel(that);
} // rgb_pixel::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare to a pixel.
 * \param that The pixel to compare to.
 */
bool claw::graphic::rgb_pixel::operator!=(const rgb_pixel& that) const
{ 
  return !(*this == that);
} // rgb_pixel::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare to a pixel.
 * \param that The pixel to compare to.
 */
bool claw::graphic::rgb_pixel::operator!=(const rgba_pixel& that) const
{ 
  return !(*this == that);
} // rgb_pixel::operator!=()




/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
claw::graphic::rgba_pixel::rgba_pixel()
{

} // rgba_pixel::rgba_pixel()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param that The other pixel.
 * \remark The alpha component of \a this is set to 255.
 */
claw::graphic::rgba_pixel::rgba_pixel( const rgb_pixel& that )
{
  components.red = that.components.red;
  components.green = that.components.green;
  components.blue = that.components.blue;
  components.alpha = 255;
} // rgba_pixel::rgba_pixel()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param r The value of the red field.
 * \param g The value of the green field.
 * \param b The value of the blue field.
 * \param a The value of the alpha field.
 */
claw::graphic::rgba_pixel::rgba_pixel
( component_type r, component_type g, component_type b, component_type a )
{ 
  components.red = r;
  components.green = g;
  components.blue = b;
  components.alpha = a;
} // rgba_pixel::rgba_pixel()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor from a color name or code.
 * \param c The color name or code.
 */
claw::graphic::rgba_pixel::rgba_pixel( const std::string& c )
{
  std::istringstream iss(c);
  u_int_32 color;
  bool has_alpha;

  if ( c[0] == '#' )
    {
      iss.ignore(1);
      has_alpha = c.length() > 7;
    }
  else
    has_alpha = c.length() > 6;

  if ( !((iss >> std::hex >> color) && (iss.rdbuf()->in_avail() == 0)) )
    throw std::invalid_argument(c);

  if ( has_alpha )
    components.alpha = (color & 0xFF000000) >> (CHAR_BIT * 3);
  else
    components.alpha = std::numeric_limits<component_type>::max();

  components.red = (color & 0xFF0000) >> (CHAR_BIT * 2);
  components.green = (color & 0x00FF00) >> CHAR_BIT;
  components.blue = color & 0x0000FF;
} // rgba_pixel::rgba_pixel()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignement operator.
 * \param that The other pixel.
 * \remark The alpha component of \a this is set to 255.
 */
claw::graphic::rgba_pixel&
claw::graphic::rgba_pixel::operator=( const rgb_pixel& that )
{
  components.red = that.components.red;
  components.green = that.components.green;
  components.blue = that.components.blue;
  components.alpha = 255;

  return *this;
} // rgba_pixel::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two pixels are equal.
 * \param that The other pixel.
 */
bool claw::graphic::rgba_pixel::operator==( const rgba_pixel& that ) const
{
  return pixel == that.pixel;
} // rgba_pixel::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two pixels are different.
 * \param that The other pixel.
 */
bool claw::graphic::rgba_pixel::operator!=( const rgba_pixel& that ) const
{
  return pixel != that.pixel;
} // rgba_pixel::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the luminosity of the pixel.
 * \return The luminosity between 0 (dark) and 255 (light).
 *
 * The luminosity is approximated with integers:
 * \f$0,715160 \simeq 183 \div 256 = 0,71484375 \f$
 * \f$0,212671 \simeq 54 \div 256 = 0,2109375 \f$
 * \f$0,072169 \simeq 18 \div 256 = 0,0703125 \f$
 */
claw::graphic::rgba_pixel::component_type
claw::graphic::rgba_pixel::luminosity() const
{
  return ((unsigned int)components.red * 183 
          + (unsigned int)components.green * 54 
          + (unsigned int)components.blue * 18
          ) / 256;
} // rgba_pixel::luminosity()
