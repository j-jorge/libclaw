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
 * \file rle_encoder.hpp
 * \brief A class to help run-length encoding (RLE) streams.
 * \author Julien Jorge
 */
#ifndef __CLAW_RLE_ENCODER_HPP__
#define __CLAW_RLE_ENCODER_HPP__

#include <list>

namespace claw
{
  /**
   * \brief A class to help run-length encoding (RLE) streams.
   *
   *\b Template \b parameters :
   * - \a OutputBuffer The type of the output buffer.
   *
   * The \a OutputBuffer type must have the following typedefs :
   * - pattern_type, the type of the stored data.
   *
   * The \a OutputBuffer type must have the following methods :
   * - encode( unsigned int n, pattern_type pattern ), code n times the
   *   pattern ;
   * - template<typename Iterator> raw( Iterator first, Iterator last ) ;
   * - unsigned int min_interesting() returns the minimum number of time we
   *must have the same value before compressing it ;
   * - unsigned int max_encodable() return the maximum number of time we can
   *   have the same value before compressing it.
   *
   * \author Julien Jorge
   */
  template <typename OutputBuffer>
  class rle_encoder
  {
  public:
    /** \brief The type of the output buffer. */
    typedef OutputBuffer output_buffer_type;

    /** \brief The type of the stored data. */
    typedef typename output_buffer_type::pattern_type pattern_type;

  private:
    /** \brief The type of the buffer on which we store the raw data. */
    typedef std::list<pattern_type> raw_buffer_type;

  public:
    template <typename Iterator>
    void encode(Iterator first, Iterator last,
                output_buffer_type& output) const;

  }; // class rle_encoder
}

#include <claw/rle_encoder.tpp>

#endif // __CLAW_RLE_ENCODER_HPP__
