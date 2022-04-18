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

  contact: julien.jorge@stuff-o-matic.com
*/
#include <cassert>
#include <claw/bit_ostream.hpp>
#include <claw/buffered_istream.hpp>
#include <claw/buffered_ostream.hpp>
#include <claw/lzw_encoder.hpp>
#include <fstream>
#include <iostream>
#include <limits>

/**
 * \brief The input_buffer is the stream where raw data is read by the LZW
 *        encoder.
 */
class input_buffer
{
public:
  input_buffer(std::istream& is)
    : stream(is)
  {
    stream.read_more(1024);
  }

  bool end_of_data() const
  {
    return !stream;
  }
  unsigned int symbols_count() const
  {
    return (unsigned int)std::numeric_limits<unsigned char>::max() + 2;
  }

  unsigned int get_next()
  {
    assert(stream.remaining() > 0);

    unsigned int result = (unsigned char)stream.get_next();

    if(stream.remaining() == 0)
      stream.read_more(1024);

    return result;
  }

private:
  claw::buffered_istream<std::istream> stream;
};

/**
 * \brief The output_buffer is an interface between the LZW encoder and the
 *        stream where the encoded data is written.
 */
class output_buffer
{
public:
  output_buffer(std::ostream& os, const unsigned int max)
    : buffer(os)
    , stream(buffer)
    , m_max_code(max)
    , m_code_size(9)
    , m_code_limit(1 << m_code_size)
  {}

  unsigned int max_code() const
  {
    return m_max_code;
  }

  void reset()
  {
    m_code_size = 9;
    m_code_limit = 1 << m_code_size;
  }

  void new_code(unsigned int code)
  {
    if(code == m_code_limit)
      {
        ++m_code_size;
        m_code_limit = 1 << m_code_size;
      }
  }

  void write(unsigned int code)
  {
    stream.write((char*)&code, m_code_size);
  }

private:
  claw::buffered_ostream<std::ostream> buffer;
  claw::bit_ostream<claw::buffered_ostream<std::ostream> > stream;

  const unsigned int m_max_code;
  unsigned int m_code_size;
  unsigned int m_code_limit;
};

int main(int argc, char* argv[])
{
  if(argc != 1)
    {
      std::cerr
          << "This program encodes a stream using LZW algorithm. The data"
          << " to encode is read on standard input, and the result is"
          << " written in standard output." << std::endl;
    }
  else
    {
      claw::lzw_encoder<input_buffer, output_buffer> encoder;
      input_buffer input(std::cin);
      output_buffer output(std::cout, 4096);

      while(!input.end_of_data())
        {
          output.reset();
          encoder.encode(input, output);
          output.write(256);
        }
    }

  return 0;
}
