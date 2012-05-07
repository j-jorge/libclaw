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
 * \file rle_decoder.hpp
 * \brief A class to help decoding run-length encoded (RLE) streams.
 * \author Julien Jorge
 */
#ifndef __CLAW_RLE_DECODER_HPP__
#define __CLAW_RLE_DECODER_HPP__

namespace claw
{
  /**
   * \brief A class to help decoding run-length encoded (RLE) streams.
   *
   *\b Template \b parameters :
   * - \a Pattern The type of the patterns in the coded stream,
   * - \a InputBuffer The type of the input buffer,
   * - \a OutputBuffer The type of the output buffer.
   *
   * The \a Pattern and \a InputBuffer parameters don't have any type
   * requirement.
   *
   * The \a OutputBuffer type must have the following methods :
   * - fill( unsigned int n, Pattern pattern ), copy n times the pattern,
   * - copy( unsigned int n, InputBuffer input ), copy n patterns directly from
   *         the input buffer.
   *
   * \author Julien Jorge
   */
  template< typename Pattern, typename InputBuffer, typename OutputBuffer >
  class rle_decoder
  {
  public:
    /** \brief The type of the patterns we will read in the input buffer. */
    typedef Pattern pattern_type;

    /** \brief The type of the input buffer. */
    typedef InputBuffer input_buffer_type;

    /** \brief The type of the output buffer. */
    typedef OutputBuffer output_buffer_type;

  protected:
    /**
     * \brief State of the decompression.
     */
    enum mode
      {
        /** \brief Stop the decoding. */
        stop,

        /** \brief Next bytes represent raw data. */
        raw,

        /** \brief Next bytes represent compressed data. */
        compressed
      }; // enum mode

  public:
    rle_decoder();
    virtual ~rle_decoder();

    void decode( input_buffer_type& input, output_buffer_type& output );
  
  protected:
    virtual void read_mode( input_buffer_type& input,
                            output_buffer_type& output ) = 0;

  protected:
    /** \brief Current mode of the decompression. */
    mode m_mode;

    /**
     * \brief Case of m_mode :
     * - m_mode == raw : The number of the next raw patterns,
     * - m_mode == compressed : How many times the pattern is repeated.
     */
    unsigned int m_count;

    /** \brief The pattern to repeat. */
    pattern_type m_pattern;
  }; // class rle_decoder
} // namespace claw

#include <claw/impl/rle_decoder.tpp>

#endif // __CLAW_RLE_DECODER_HPP__
