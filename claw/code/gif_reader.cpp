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
 * \file gif_reader.cpp
 * \brief Implementation of the claw::graphic::gif::reader class.
 * \author Julien Jorge
 */
#include <claw/gif.hpp>

#include <algorithm>
#include <climits>
#include <limits>
#include <claw/functional.hpp>
#include <claw/exception.hpp>
#include <claw/it_index.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param is The stream from which we read the data.
 * \param code_size The initial size of the codes.
 */
claw::graphic::gif::reader::input_buffer::input_buffer
( std::istream& is, u_int_8 code_size )
  : m_val(0), m_input(is), m_pending(0), m_pending_bits(0), m_pending_end(0),
    m_initial_code_size(code_size), m_code_size(m_initial_code_size+1),
    m_code_limit(1 << m_code_size)
{
  m_input.read
    ( reinterpret_cast<char*>(&m_next_data_length),
      sizeof(m_next_data_length) );
} // gif::reader::input_buffer::input_buffer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we reached the end of the data and we must restart the
 *        decompression procedure.
 */
bool claw::graphic::gif::reader::input_buffer::end_of_data() const
{
  return (m_val == (unsigned int)(1 << m_initial_code_size))
    || end_of_information();
} // gif::reader::input_buffer::end_of_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we reached the end of the compressed stream.
 */
bool claw::graphic::gif::reader::input_buffer::end_of_information() const
{
  return !m_input || (m_val == (unsigned int)(1 << m_initial_code_size)+1)
    || ( (m_next_data_length == 0)             // no more data in the stream
         && (m_pending == m_pending_end)       // no more data in the buffer
         && (m_pending_bits < m_code_size) );
} // gif::reader::input_buffer::end_of_information()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get The count of symbols in the alphabet.
 */
unsigned int claw::graphic::gif::reader::input_buffer::symbols_count() const
{
  return (1 << m_initial_code_size) + 2;
} // gif::reader::input_buffer::symbols_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the next code in the input.
 */
unsigned int claw::graphic::gif::reader::input_buffer::get_next()
{
  if ( m_pending == m_pending_end )
    fill_buffer();
  else if ( m_pending_bits + (m_pending_end - m_pending - 1) * CHAR_BIT
            < m_code_size )
    fill_buffer();

  m_val = 0;

  std::size_t n(m_code_size);
  unsigned int cur_size = 0;
  char* buf = reinterpret_cast<char*>(&m_val);

  while ( (n != 0) && m_input )
    {
      while( (m_pending_bits != 0) && (n!=0) && m_input )
        {
          unsigned int bits = std::min((std::size_t)m_pending_bits, n);

          if ( CHAR_BIT - cur_size < bits )
            bits = CHAR_BIT - cur_size;

          unsigned int mask = (1 << bits) - 1;

          *buf |= (m_buffer[m_pending] & mask) << cur_size;
          cur_size += bits;
          m_pending_bits -= bits;
          m_buffer[m_pending] >>= bits;
          n -= bits;

          if ( cur_size == CHAR_BIT )
            {
              ++buf;
              cur_size = 0;
            }
        }

      if ( m_pending_bits == 0 )
        {
          ++m_pending;

          if ( (m_pending == m_pending_end) && (n!=0) )
            fill_buffer();
          
          if ( m_pending == m_pending_end )
            n = 0;
          else
            m_pending_bits = CHAR_BIT;
        }
    }

  return m_val;
} // gif::reader::input_buffer::get_next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reset the code size and the code limit.
 */
void claw::graphic::gif::reader::input_buffer::reset()
{
  m_val = 0;
  m_code_size = m_initial_code_size+1;
  m_code_limit = 1 << m_code_size;
} // gif::reader::input_buffer::reset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the input buffer that a new code has been added in the
 *        dictionary.
 * \param code The new code.
 */
void claw::graphic::gif::reader::input_buffer::new_code( unsigned int code )
{
  if ( (code == m_code_limit) && (m_code_size != 12) )
    {
      ++m_code_size;
      m_code_limit = 1 << m_code_size;
    }
} // gif::reader::input_buffer::new_code()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the input buffer.
 */
void claw::graphic::gif::reader::input_buffer::fill_buffer()
{
  // move available data at the begining of the buffer
  std::copy( m_buffer + m_pending, m_buffer + m_pending_end, m_buffer );
  m_pending_end = m_pending_end - m_pending;
  m_pending = 0;

  if (m_next_data_length != 0)
    {
      assert( m_pending_end + m_next_data_length <= sizeof(m_buffer) );

      m_input.read( m_buffer + m_pending_end, m_next_data_length );
      m_pending_end += m_next_data_length;

      if ( (m_pending_bits == 0) && (m_pending != m_pending_end) )
        m_pending_bits = CHAR_BIT;

      m_input.read
        ( reinterpret_cast<char*>(&m_next_data_length),
          sizeof(m_next_data_length) );
    }
} // gif::reader::input_buffer::fill_buffer()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The palette from which we take the color.
 * \param id The descriptor of the frame.
 * \param transparent_color_index The index of the transparent color in \a p.
 * \param output The image in which we store the data.
 */
claw::graphic::gif::reader::output_buffer::output_buffer
( const palette_type& p, const image_descriptor& id,
  int transparent_color_index, image& output )
  : m_palette(p), m_id(id), m_transparent_color_index(transparent_color_index),
    m_output(output), m_x(0), m_y(0), m_interlace_pass(0),
    m_interlace_step(8)
{

} // gif::reader::output_buffer::output_buffer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a data.
 * \param code The data to write.
 */
void claw::graphic::gif::reader::output_buffer::write( unsigned int code )
{
  assert(code < m_palette.size());
  assert(m_x < m_id.width);
  assert(m_y < m_id.height);

  m_output[m_y + m_id.top][m_x + m_id.left] = m_palette[code];

  if ( m_transparent_color_index != -1 )
    if ( code == (unsigned int)m_transparent_color_index )
      m_output[m_y + m_id.top][m_x + m_id.left].components.alpha = 0;

  ++m_x;

  if (m_x == m_id.width)
    {
      m_x = 0;

      if ( !m_id.is_interlaced() )
        ++m_y;
      else
        {
          m_y += m_interlace_step;

          while ( (m_y >= m_id.height) && (m_interlace_pass!=3) )
            {
              ++m_interlace_pass;
              switch (m_interlace_pass)
                {
                case 1: m_y = 4; m_interlace_step = 8; break;
                case 2: m_y = 2; m_interlace_step = 4; break;
                case 3: m_y = 1; m_interlace_step = 2; break;
                }
            }
        }
    }
} // gif::reader::output_buffer::write()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which we store the data.
 */
claw::graphic::gif::reader::reader( image& img )
  : m_image(&img)
{

} // gif::reader::reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which we store the data.
 * \param f The stream from which we read the data (gif file format).
 *
 * The first frame of the gif will be saved in the image passed to the
 * constructor.
 */
claw::graphic::gif::reader::reader( image& img, std::istream& f )
  : m_image(&img)
{
  load( f );
} // gif::reader::reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param frames The frames read from the file.
 * \param f The stream from which we read the data (gif file format).
 */
claw::graphic::gif::reader::reader
( frame_list& frames, std::istream& f )
  : m_image(NULL) 
{
  load( f );
  frames = m_frame;
  m_frame.clear();
} // gif::reader::reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in which we store the data.
 * \param frames The frames read from the file.
 * \param f The stream from which we read the data (gif file format).
 *
 * The first frame of the gif will be saved in the image passed to the
 * constructor.
 */
claw::graphic::gif::reader::reader
( image& img, frame_list& frames, std::istream& f )
  : m_image(&img) 
{
  load( f );
  frames = m_frame;
  m_frame.clear();
} // gif::reader::reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
claw::graphic::gif::reader::~reader()
{
  clear();
} // gif::reader::~reader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the image data from a stream.
 * \param f The stream from which we read the data (gif file format).
 */
void claw::graphic::gif::reader::load( std::istream& f )
{
  clear();

  inside_load(f);

  if ( !m_frame.empty() && (m_image!=NULL) )
    *m_image = *m_frame.front();
} // gif::reader::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear the data read from a file.
 */
void claw::graphic::gif::reader::clear()
{
  std::for_each
    ( m_frame.begin(), m_frame.end(), claw::delete_function<frame*>() );
  m_frame.clear();
} // gif::reader::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the image data from a stream.
 * \param f The stream from which we read the data (gif file format).
 */
void claw::graphic::gif::reader::inside_load( std::istream& f )
{
  std::istream::pos_type init_pos = f.tellg();
  reader_info info;
  info.palette = NULL;

  try
    {
      check_if_gif(f);

      read_screen_descriptor(f, info);
      read_data(f, info);
      make_frames(info);

      delete info.palette;
    }
  catch(...)
    {
      delete info.palette;

      f.seekg( init_pos, std::ios_base::beg );
      throw;
    }
} // gif::reader::inside_load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the frames of the final animation.
 * \param info The informations on the gif file.
 */
void claw::graphic::gif::reader::make_frames( const reader_info& info )
{
  it_index<frame_list::const_iterator> it(m_frame.begin());

  frame_list result;
  std::size_t cumul_count(0);
  frame cumul(info.sd.screen_width, info.sd.screen_height);
  frame prev;

  if ( !info.disposal_method.empty() )
    {
      if ( info.disposal_method[0]
           == graphic_control_extension::dispose_background )
        fill_background(cumul, info);
      else
        std::fill(cumul.begin(), cumul.end(), transparent_pixel);
    }

  for ( ; it!=m_frame.end(); ++it )
    {
      if ( info.disposal_method[it]
           == graphic_control_extension::dispose_previous )
        prev = cumul;

      cumul.merge(**it);
      cumul.set_delay( (*it)->get_delay() );
      ++cumul_count;

      if ( cumul.get_delay() > 0 )
        {
          result.push_back( new frame(cumul) );
          cumul_count = 0;
        }

      switch( info.disposal_method[it] )
        {
        case graphic_control_extension::dispose_background:
          fill_background(cumul, info);
          break;
        case graphic_control_extension::dispose_previous:
          cumul = prev;
          break;
        default:
          { /* nothing to do */ }
        }
    }

  if ( cumul_count != 0 )
     result.push_back( new frame(cumul) );

  clear();
  std::swap( m_frame, result );
} // gif::reader::make_frames()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill an image with the correct background color.
 * \param img The image to fill.
 * \param info The informations on the gif file.
 */
void claw::graphic::gif::reader::fill_background
( image& img, const reader_info& info ) const
{
  rgba_pixel clr(transparent_pixel);

  if ( info.sd.has_global_color_table() && (info.palette != NULL) )
    if (info.sd.background_color < info.palette->size() )
      clr = (*info.palette)[info.sd.background_color];

  std::fill( img.begin(), img.end(), clr );
} // gif::reader::fill_background()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if a stream contains gif data.
 * \param f The stream to check.
 */
void claw::graphic::gif::reader::check_if_gif( std::istream& f ) const
{
  CLAW_PRECOND( !!f );

  header h;
  f.read( reinterpret_cast<char*>(&h), sizeof(header) );

  bool valid = false;

  if ( f.rdstate() == std::ios_base::goodbit )
    if ( (h.signature[0] == 'G')
         && (h.signature[1] == 'I')
         && (h.signature[2] == 'F')
         && (h.version[0] == '8')
         && ( (h.version[1] == '7') || (h.version[1] == '9') )
         && (h.version[2] == 'a') )
        valid = true;

  if ( !valid )
    throw claw::bad_format( "Not a GIF file." );
} // gif::reader::check_if_gif()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the screen descriptor and initialize the screen.
 * \param f The stream from which we read the data (gif file format).
 * \param info (out) Some global data needed when reading the file.
 */
void claw::graphic::gif::reader::read_screen_descriptor
( std::istream& f, reader_info& info )
{
  f.read( reinterpret_cast<char*>(&info.sd), sizeof(screen_descriptor) );

  if ( info.sd.has_global_color_table() )
    {
      info.palette = new palette_type(info.sd.color_palette_size());
      read_palette(f, *info.palette);
    }
} // gif::reader::read_screen_descriptor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a palette.
 * \param f The stream from which we read the palette.
 * \param p (out) The palette.
 */
void claw::graphic::gif::reader::read_palette
( std::istream& f, palette_type& p ) const
{
  u_int_8 red, green, blue;

  for (std::size_t i=0; i!=p.size(); ++i)
    {
      f.read( reinterpret_cast<char*>(&red), sizeof(u_int_8) );
      f.read( reinterpret_cast<char*>(&green), sizeof(u_int_8) );
      f.read( reinterpret_cast<char*>(&blue), sizeof(u_int_8) );
      
      p[i].components.red = red;
      p[i].components.green = green;
      p[i].components.blue = blue;
    }
} // gif::reader::read_palette()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the data of the gif stream.
 * \param f The stream to read.
 * \param info Some global data needed when reading the file.
 */
void claw::graphic::gif::reader::read_data
( std::istream& f, reader_info& info )
{
  u_int_8 code;

  do
    {
      code = 0;
      f.read( reinterpret_cast<char*>(&code), sizeof(code) );

      if (f)
        switch(code)
          {
          case extension::block_id:
            f.read( reinterpret_cast<char*>(&code), sizeof(code) );

            if (code == graphic_control_extension::block_label)
              read_frame_with_gce(f, info);
            else
              skip_extension(f);

            break;
          case image_descriptor::block_id:
            read_frame(f, info);
            break;
          case trailer::block_id:
            break;
          default:
            throw claw::bad_format( "gif::reader: invalid code" );
          }
    }
  while ( f && (code != trailer::block_id) );
} // gif::reader::read_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a frame without graphic control extension.
 * \param f The stream to read.
 * \param info Some global data needed when reading the file.
 */
void claw::graphic::gif::reader::read_frame
( std::istream& f, reader_info& info )
{
  frame* new_frame(NULL);

  try
    {
      new_frame = new frame;
      read_frame_data(f, info, *new_frame);

      info.disposal_method.push_back(graphic_control_extension::dispose_none);
      m_frame.push_back(new_frame);
    }
  catch(...)
    {
      delete new_frame;
      throw;
    }
} // gif::reader::read_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the data of the gif stream.
 * \param f The stream to read.
 * \param info Some global data needed when reading the file.
 */
void claw::graphic::gif::reader::read_frame_with_gce
( std::istream& f, reader_info& info )
{
  graphic_control_extension gce;
  u_int_8 code;

  f.read( reinterpret_cast<char*>(&gce), sizeof(gce) );
  f.read( reinterpret_cast<char*>(&code), sizeof(code) );

  while ( (code == extension::block_id) && f )
    {
      f.read( reinterpret_cast<char*>(&code), sizeof(code) );

      if (code == graphic_control_extension::block_label)
        f.read( reinterpret_cast<char*>(&gce), sizeof(gce) );
      else // unknown extension
        skip_extension(f);

      // read the code of the following block
      f.read( reinterpret_cast<char*>(&code), sizeof(code) );
    }

  if (code == image_descriptor::block_id)
    {
      frame* new_frame = new frame;
      new_frame->set_delay(gce.delay);

      info.disposal_method.push_back(gce.get_disposal_method());

      if ( gce.has_transparent_color() )
        info.transparent_color_index = gce.transparent_color;
      else
        info.transparent_color_index = -1;

      read_frame_data(f, info, *new_frame);
      m_frame.push_back(new_frame);
    }
} // gif::reader::read_frame_with_gce()

/*----------------------------------------------------------------------------*/
/**
 * \brief Skip an extension block.
 * \param f The stream to read.
 * \pre The label of the extension is already read.
 */
void claw::graphic::gif::reader::skip_extension( std::istream& f ) const
{
  u_int_8 block_size(0);

  f.read( reinterpret_cast<char*>(&block_size), sizeof(block_size) );

  while ( f && (block_size!=0) )
    {
      f.seekg( block_size, std::ios_base::cur );
      f.read( reinterpret_cast<char*>(&block_size), sizeof(block_size) );
    }
} // gif::reader::skip_extension()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the data of a frame.
 * \param f The stream to read.
 * \param info Some global data needed when reading the file.
 * \param the_frame (in/out) The frame in which we save the pixels.
 * \pre The cursor in the stream \a f is positioned at the beginning of an
 *      image_descriptor block (just after the separator code).
 */
void claw::graphic::gif::reader::read_frame_data
( std::istream& f, const reader_info& info, frame& the_frame ) const
{
  image_descriptor id;

  f.read( reinterpret_cast<char*>(&id), sizeof(id) );

  the_frame.set_size(info.sd.screen_width, info.sd.screen_height);

  std::fill( the_frame.begin(), the_frame.end(), transparent_pixel );

  palette_type* palette(info.palette);

  if ( id.has_color_table() )
    {
      palette = new palette_type(id.color_palette_size());
      read_palette(f, *palette);
    }

  decode_data(f, *palette, id, info.transparent_color_index, the_frame);

  if ( id.has_color_table() )
    delete palette;
} // gif::reader::read_frame_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Decode the pixels of a frame.
 * \param f The stream to read.
 * \param palette The color palette to use.
 * \param id The descriptor of the current frame.
 * \param transparent_color_index The index of the transparent color in \a p.
 * \param the_frame (in/out) The frame in which we save the pixels.
 * \pre The cursor in the stream \a f is positioned at the beginning of an
 *      LZW encoded block (just on the code size byte).
 */
void claw::graphic::gif::reader::decode_data
( std::istream& f, const palette_type& palette, const image_descriptor& id,
  int transparent_color_index, frame& the_frame ) const
{
  u_int_8 code_size;

  f.read( reinterpret_cast<char*>(&code_size), sizeof(code_size) );
  input_buffer input(f, code_size);
  output_buffer output(palette, id, transparent_color_index, the_frame);

  do
    {
      gif_lzw_decoder decoder;
      input.reset();
      decoder.decode(input, output);
    }
  while ( !input.end_of_information() );
} // gif::reader::decode_data()
