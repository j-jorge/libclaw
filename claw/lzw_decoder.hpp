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
 * \file lzw_decoder.hpp
 * \brief A class to help decoding a stream encoded with Lempel-Ziv-Welch (LZW)
 *        compression algorithm.
 * \author Julien Jorge
 */
#ifndef __CLAW_LZW_DECODER_HPP__
#define __CLAW_LZW_DECODER_HPP__

#include <vector>
#include <utility>

namespace claw
{
  /**
   * \brief A class to help decoding a stream encoded with Lempel-Ziv-Welch
   *        (LZW) compression algorithm.
   *
   *\b Template \b parameters:
   * - \a InputBuffer The type of the buffer where we read the compressed datas,
   * - \a OutputBuffer The type of the buffer where we write the uncompressed
   *   datas.
   *
   * The \a InputBuffer type must have the following methods:
   * - bool end_of_data(), tell if the input buffer is empty,
   * - unsigned int symbols_count(), get the number of different symbols in the
   *   uncompressed data,
   * - unsigned int get_next(), returns the next code and moves to the next
   *   symbol.
   *
   * The \a OutputBuffer type must have the following methods:
   * - write( unsigned int ), write a symbol in the output.
   *
   * \author Julien Jorge
   */
  template<typename InputBuffer, typename OutputBuffer>
  class lzw_decoder
  {
  public:
    /** \brief The type of the input buffer. */
    typedef InputBuffer input_buffer_type;

    /** \brief The type of the output buffer. */
    typedef OutputBuffer output_buffer_type;

  private:
    typedef std::pair<unsigned int, unsigned int> word_type;
    typedef std::vector<word_type> table_type;

  public:
    void decode( input_buffer_type& input, output_buffer_type& output );
  
  private:
    unsigned int get_first_symbol
    ( const table_type& table, const unsigned int code,
      const unsigned int symbols_count ) const;

    void decompose
    ( const table_type& table, unsigned int code,
      const unsigned int symbols_count, output_buffer_type& output ) const;

  }; // class lzw_decoder
} // namespace claw

#include <claw/impl/lzw_decoder.tpp>

#endif // __CLAW_LZW_DECODER_HPP__
