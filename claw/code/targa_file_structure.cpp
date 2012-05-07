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
 * \file targa_file_structure.cpp
 * \brief Implementation of the targa::file_structure class.
 * \author Julien Jorge
 */
#include <claw/targa.hpp>


//************************ targa::file_structure::header ***********************



/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
claw::graphic::targa::file_structure::header::header()
{

} // targa::file_structure::header::header()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param w The width of the image.
 * \param h The height of the image.
 */
claw::graphic::targa::file_structure::header::header
( unsigned int w, unsigned int h )
{
  id_length = 0;
  color_map = 0;

  image_type = true_color;

  color_map_specification.first_entry_index = 0;
  color_map_specification.length = 0;
  color_map_specification.entry_size = 0;

  image_specification.x_origin = 0;
  image_specification.y_origin = 0;
  image_specification.width = w;
  image_specification.height = h;

  image_specification.bpp = 32; // pixel32

  image_specification.descriptor = 8; // unsigned char
  image_specification.descriptor |= 0x20; // origin is top-left
} // targa::file_structure::header::header()




//************** targa::file_structure::header::specification ******************




/*----------------------------------------------------------------------------*/
/**
 * \brief Is image stored up to down ?
 */
bool
claw::graphic::targa::file_structure::header::specification::
up_down_oriented() const
{
  return descriptor & 0x20;
} // targa::file_structure::header::specification::up_down_oriented()

/*----------------------------------------------------------------------------*/
/**
 * \brief Is image stored left to right ?
 */
bool
claw::graphic::targa::file_structure::header::specification::
left_right_oriented() const
{
  return !(descriptor & 0x10);
} // targa::file_structure::header::specification::left_right_oriented()

/*----------------------------------------------------------------------------*/
/** 
 * \brief Number of bits per pixel assigned to alpha chanel.
 */
unsigned char
claw::graphic::targa::file_structure::header::specification::alpha() const
{
  return descriptor & 0x0F;
} // targa::file_structure::header::specification::alpha()




//********************* targa::file_structure::footer **************************




/*----------------------------------------------------------------------------*/
const std::string
claw::graphic::targa::file_structure::footer::s_signature("TRUEVISION-XFILE.");

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
claw::graphic::targa::file_structure::footer::footer()
  : extension_offset(0), developer_offset(0)
{
  std::copy( s_signature.begin(), s_signature.end(), signature );
  signature[s_signature.length()] = '\0';
} // targa::file_structure::footer::footer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the data of this footer is valid.
 */
bool claw::graphic::targa::file_structure::footer::is_valid() const
{
  return std::equal( s_signature.begin(), s_signature.end(), signature )
    && signature[s_signature.length()] == '\0';
} // targa::file_structure::footer::is_valid()
