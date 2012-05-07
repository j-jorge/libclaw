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
 * \file pixel.hpp
 * \brief Representation of a pixel in image processing.
 * \author Julien Jorge
 */
#ifndef __CLAW_PIXEL_HPP_
#define __CLAW_PIXEL_HPP_

#include <string>

namespace claw
{
  namespace graphic
  {
    struct rgba_pixel;

    /**
     * \brief RGB pixel.
     */
    struct rgb_pixel
    {
      typedef unsigned char component_type;

      /** \brief Component by component representation. */
      struct
      {
        /** \brief Red component. */
        component_type red;

        /** \brief Green component. */
        component_type green;

        /** \brief Blue component. */
        component_type blue;

      } components; 

    public:
      rgb_pixel();
      rgb_pixel( component_type r, component_type g, component_type b );
      rgb_pixel( const rgba_pixel& p );
      explicit rgb_pixel( const std::string& c );

      bool operator==(const rgb_pixel& that) const;
      bool operator==(const rgba_pixel& that) const;
      bool operator!=(const rgb_pixel& that) const;
      bool operator!=(const rgba_pixel& that) const;

    }; // struct rgb_pixel

    /**
     * \brief RGBA pixel.
     */
    struct rgba_pixel
    {
      typedef unsigned char component_type;

      union
      {
	/** \brief Compressed representation. */
	unsigned int pixel;

	/** \brief Component by component representation. */
	struct
	{
	  /** \brief Red component. */
	  component_type red;

	  /** \brief Green component. */
	  component_type green;

	  /** \brief Blue component. */
	  component_type blue;

	  /** \brief Translucy. */
	  component_type alpha;

	} components;
      };

    public:
      rgba_pixel();
      rgba_pixel( const rgb_pixel& that );
      rgba_pixel( component_type r, component_type g, component_type b,
                  component_type a );
      explicit rgba_pixel( const std::string& c );

      rgba_pixel& operator=( const rgb_pixel& that );
      bool operator==( const rgba_pixel& that ) const;
      bool operator!=( const rgba_pixel& that ) const;

      component_type luminosity() const;

    }; // struct rgba_pixel

    typedef rgb_pixel rgb_pixel_8;
    typedef rgba_pixel rgba_pixel_8;

    extern rgba_pixel transparent_pixel;

    extern rgba_pixel black_pixel;
    extern rgba_pixel white_pixel;

    extern rgba_pixel blue_pixel;
    extern rgba_pixel green_pixel;
    extern rgba_pixel red_pixel;

    extern rgba_pixel yellow_pixel;
    extern rgba_pixel magenta_pixel;
    extern rgba_pixel cyan_pixel;

  } // namespace graphic
} // namespace claw

#endif // __CLAW_PIXEL_HPP__
