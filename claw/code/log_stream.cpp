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
 * \file log_stream.cpp
 * \brief Implementation of the log_stream classes.
 * \author Julien Jorge
 */
#include <claw/log_stream.hpp>
#include <iostream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
claw::log_stream::~log_stream()
{
  // nothing to do
} // log_stream::~log_stream()

/*----------------------------------------------------------------------------*/
/**
 * \brief Flush the stream
 */
void claw::log_stream::flush()
{
  // nothing to do
} // log_stream::flush()




/*----------------------------------------------------------------------------*/
/**
 * \brief Write a string in the stream.
 */
void claw::console_logger::write( const std::string& str )
{
  std::clog << str;
} // console_logger::write()

/*----------------------------------------------------------------------------*/
/**
 * \brief Flush the stream
 */
void claw::console_logger::flush()
{
  std::clog.flush();
} // console_logger::flush()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
claw::file_logger::file_logger( const std::string& filename )
{
  m_file = new std::ofstream( filename.c_str() );
} // file_logger::file_logger()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
claw::file_logger::~file_logger()
{
  delete m_file;
} // file_logger::~file_logger()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a string in the file.
 */
void claw::file_logger::write( const std::string& str )
{
  (*m_file) << str;
} // file_logger::write()

/*----------------------------------------------------------------------------*/
/**
 * \brief Flush the stream
 */
void claw::file_logger::flush()
{
  m_file->flush();
} // file_logger::flush()
