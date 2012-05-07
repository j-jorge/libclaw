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
 * \file xbm_reader.cpp
 * \brief Implementation of the claw::graphic::xbm::reader class.
 * \author Julien Jorge
 */
#include <claw/xbm.hpp>

#include <claw/exception.hpp>
#include <claw/assert.hpp>
#include <claw/string_algorithm.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which the data will be stored.
 */
claw::graphic::xbm::reader::reader( image& img )
  : m_image( img ), m_hot(NULL)
{

} // xbm::reader::reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which the data will be stored.
 * \param f The file from which we read the data.
 * \post img contains the data from \a f.
 */
claw::graphic::xbm::reader::reader( image& img, std::istream& f )
  : m_image( img ), m_hot(NULL)
{
  load(f);
} // xbm::reader::reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which the data will be stored.
 * \param f The file from which we read the data.
 * \post img contains the data from \a f.
 */
claw::graphic::xbm::reader::reader( xbm& img, std::istream& f )
  : m_image( img ), m_hot(NULL)
{
  load(f);
  img.set_name( m_name );

  if (m_hot != NULL)
    img.set_hot( *m_hot );
} // xbm::reader::reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
claw::graphic::xbm::reader::~reader()
{
  if (m_hot != NULL)
    {
      delete m_hot;
      m_hot = NULL;
    }
} // xbm::reader::~reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an image from a xbm file.
 * \param f XBM file.
 */
void claw::graphic::xbm::reader::load( std::istream& f )
{
  CLAW_PRECOND( !!f );

  std::istream::pos_type init_pos = f.tellg();

  if (m_hot != NULL)
    {
      delete m_hot;
      m_hot = NULL;
    }

  try
    {
      read_from_file(f);
    }
  catch(...)
    {
      if (m_hot != NULL)
        delete m_hot;

      f.clear();
      f.seekg( init_pos, std::ios_base::beg );
      throw;
    }
} // xbm::reader::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an image from a xbm file.
 * \param f XBM file.
 */
void claw::graphic::xbm::reader::read_from_file( std::istream& f )
{
  std::string line;
  bool valid_format = false;
  unsigned int bpe;

  read_size(f);
  bpe = read_bits_per_entry(f);
  read_name(f);
  read_line( f, line, '{' );

  if ( !line.empty() )
    {
      read_pixels(f, bpe);
      read_line(f, line, ';');

      valid_format = true;
    }

  if ( !valid_format )
    throw claw::exception( "Not an XBM file." );
} // xbm::reader::read_from_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the size of the image.
 * \param f The stream in which we read the field.
 */
void claw::graphic::xbm::reader::read_size( std::istream& f )
{
  unsigned int w(0), h(0);
  bool valid = true;
  bool stop = false;
  std::string line;

  while ( valid && !stop )
    {
      std::ios::pos_type pos = f.tellg();

      read_line( f, line, '\n' );

      if ( !line.empty() )
        {
          if ( line.find("width") != std::string::npos )
            w = read_dim(line);
          else if ( line.find("height") != std::string::npos )
            h = read_dim(line);
          else if ( line.find("x_hot") != std::string::npos )
            {
              if ( m_hot == NULL )
                m_hot = new claw::math::coordinate_2d<int>;

              m_hot->x = read_dim(line);
            }
          else if ( line.find("y_hot") != std::string::npos )
            {
              if ( m_hot == NULL )
                m_hot = new claw::math::coordinate_2d<int>;

              m_hot->y = read_dim(line);
            }
          else if ( line.find("static") != std::string::npos )
            {
              stop = true;
              f.seekg( pos );
            }
        }
      else
        valid = false;
    }
  
  if ( valid )
    m_image.set_size(w, h);
  else
    throw claw::exception( "Not an XBM file." );
} // xbm::reader::read_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the width or height of the image.
 * \param line The line in which we read the field.
 */
unsigned int
claw::graphic::xbm::reader::read_dim( const std::string& line ) const
{
  unsigned int result;
  std::istringstream iss(line);
  std::string token;
  bool valid = false;

  if (iss >> token)
    if ( token == "#define" )
      if ( iss >> token )
        if ( iss >> result )
          valid = true;

  if ( !valid )
    throw claw::exception( "Not an XBM file." );

  return result;
} // xbm::reader::read_dim()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the number of bits per entry.
 * \param f The stream in which we read the field.
 */
unsigned int
claw::graphic::xbm::reader::read_bits_per_entry( std::istream& f ) const
{
  std::string line;
  unsigned int result(0);

  std::string token;

  if ( f >> token )
    if ( token == "static" )
      if ( f >> token )
        {
          if ( token == "unsigned" )
            f >> token;
          else if ( token == "signed" )
            f >> token;

          if ( token == "char" )
            result = sizeof(char) * 8;
          else if ( token == "short" )
            result = sizeof(short) * 8;
          else if ( token == "int" )
            result = sizeof(int) * 8;
          else if ( token == "long" )
            result = sizeof(long) * 8;
        }

  if ( result == 0 )
    throw claw::exception( "Not an XBM file." );

  return result;
} // xbm::reader::read_bits_per_entry()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the name of the image.
 * \param f The stream in which we read the field.
 */
void claw::graphic::xbm::reader::read_name( std::istream& f )
{
  bool valid = false;
  std::string line;

  read_line(f, line, '[');

  if ( !line.empty() )
    {
      std::string::size_type end = line.find_last_of('_');
                  
      if ( end != std::string::npos )
        {
          std::string::size_type begin = line.find_last_of(" \t", end);

          if ( begin == std::string::npos )
            begin = 0;

          m_name = line.substr(begin, end - begin);
          valid = true;
        }
    }

  if ( !valid )
    throw claw::exception( "Not an XBM file." );
} // xbm::reader::read_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the pixels of the image.
 * \param f The stream in which we search the next line.
 * \param bpe Number of bits per entry.
 */
void claw::graphic::xbm::reader::read_pixels
( std::istream& f, unsigned int bpe ) const
{
  image::iterator first = m_image.begin();
  const image::iterator last = m_image.end();

  bool valid = true;

  unsigned int x = 0;

  while ( (first!=last) && valid )
    {
      std::string s_val;
      read_line( f, s_val, ',' );

      std::istringstream iss(s_val);
      long int val;

      if ( iss >> std::hex >> val )
        {
          for( unsigned int i=0;
               (i!=bpe) && (first!=last) && (x!=m_image.width());
               ++i, ++first, ++x, val >>= 1 )
            if ( val & 1 )
              *first = black_pixel;
            else
              *first = white_pixel;

          if ( x==m_image.width() )
            x = 0;
        }
      else
        valid = false;
    }

  if ( !valid )
    throw claw::exception( "Not an XBM file." );
} // xbm::reader::read_pixels()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the next not commented line, and remove any comments from it.
 * \param f The stream in which we search the next line.
 * \param line The line in which we read the field.
 * \param endchar The character used to end the line.
 */
void claw::graphic::xbm::reader::read_line
( std::istream& f, std::string& line, char endchar ) const
{
  bool stop = false;

  line.clear();

  while ( !stop )
    if ( std::getline( f, line, endchar ) )
      {
        text::trim(line);


        remove_comments(f, line, endchar);
        stop = !line.empty();
      }
    else
      stop = true;
} // xbm::reader::read_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the comments from a line.
 * \param f If the line contains the begining of a multi line comment, we search
 *        the next line from this stream.
 * \param line The line in which we read the field.
 * \param endchar The character used to end the line.
 */
void claw::graphic::xbm::reader::remove_comments
( std::istream& f, std::string& line, char endchar ) const
{
  std::string working(line);
  std::string::size_type beg = working.find( "/*" );

  if ( beg != std::string::npos )
    {
      line = working.substr(0, beg);

      std::string::size_type end = working.rfind( "*/" );
      bool stop = false;

      while ( (end == std::string::npos) && !stop )
        if ( std::getline(f, working, endchar) )
          end = working.find( "*/" );
        else
          stop = true;

      if ( !stop )
        {
          line += working.substr(end+2, line.length() - end - 2);
          text::trim(line);
        }

      if ( !line.empty() )
        remove_comments(f, line, endchar);
    }
} // xbm::reader::remove_comments()
