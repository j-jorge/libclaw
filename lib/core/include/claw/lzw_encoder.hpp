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
/**
 * \file lzw_encoder.hpp
 * \brief A class to help encoding a stream with Lempel-Ziv-Welch (LZW)
 *        compression algorithm.
 * \author Julien Jorge
 */
#ifndef __CLAW_LZW_ENCODER_HPP__
#define __CLAW_LZW_ENCODER_HPP__

namespace claw
{
  /**
   * \brief A class to help encoding a stream with Lempel-Ziv-Welch (LZW)
   *        compression algorithm.
   *
   * \b Template \b parameters:
   * - \a InputBuffer The type of the input buffer (where we get raw data),
   * - \a OutputBuffer The type of the output buffer (where we write compressed
   *      data).
   *
   * The \a InputBuffer type must have the following methods:
   * - bool end_of_data(), tell if the input buffer is empty,
   * - unsigned int symbols_count(), get the number of different symbols in the
   *   uncompressed data,
   * - unsigned int get_next(), returns an unsigned integer representing the
   *   next symbol and moves to the next symbol.
   *
   * The \a OutputBuffer type must have the following methods:
   * - unsigned int max_code(), get the highest code that the output buffer can
   *   handle,
   * - write( unsigned int ), write a code in the output.
   *
   * \author Julien Jorge
   */
  template <typename InputBuffer, typename OutputBuffer>
  class lzw_encoder
  {
  public:
    /** \brief The type of the input buffer. */
    typedef InputBuffer input_buffer_type;

    /** \brief The type of the output buffer. */
    typedef OutputBuffer output_buffer_type;

  public:
    void encode(input_buffer_type& input, output_buffer_type& output) const;

  }; // class lzw_encoder
}

#include <claw/lzw_encoder.tpp>

#endif // __CLAW_LZW_ENCODER_HPP__
