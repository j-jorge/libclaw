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
 * \file jpeg_error_manager.cpp
 * \brief Implementation of the claw::graphic::jpeg::error_manager class.
 * \author Julien Jorge
 */
#include <claw/jpeg.hpp>
#include <claw/jpeg_error_manager.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Throw an exception when an error occurs in an internal jpeg
 *        processing.
 * \param cinfo Informations about the process.
 */
void claw::graphic::jpeg__error_manager__error_exit(j_common_ptr cinfo)
{
  claw::graphic::jpeg::error_manager* err =
    (claw::graphic::jpeg::error_manager*)cinfo->err;

  err->error_string = cinfo->err->jpeg_message_table[cinfo->err->msg_code];

  /* Return control to the setjmp point */
  longjmp(err->setjmp_buffer, 1);
} // jpeg__error_manager__error_exit()
