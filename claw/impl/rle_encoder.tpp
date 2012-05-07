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
 * \file rle_encoder.tpp
 * \brief Implementation of the rle_encoder class.
 * \author Julien Jorge
 */
#include <cassert>

/*----------------------------------------------------------------------------*/
/**
 * \brief Encode a range of datas.
 * \param first Iterator on the first data.
 * \param last Iterator past the last data.
 * \param output The buffer on which we write the compressed data.
 *
 * \pre Iterator::value_type must be castable to pattern_type.
 */
template<typename OutputBuffer >
template<typename Iterator>
void claw::rle_encoder<OutputBuffer>::encode( Iterator first, Iterator last,
                                              output_buffer_type& output ) const
{
  const unsigned int max_encodable = output.max_encodable();
  const unsigned int min_interesting = output.min_interesting();
  raw_buffer_type raw_buffer;

  assert( max_encodable > 0 );

  while (first != last)
    {
      unsigned int count = 1;
      pattern_type pattern = *first;
      Iterator saved_it = first;
      ++first;
      bool ok = true;

      // try to find enough similar data
      while ( ok && (first != last) && (count < max_encodable) )
        if (*first == pattern)
          {
            ++count;
            ++first;
          }
        else
          ok = false;

      // if we have enough data
      if ( count >= min_interesting )
        {
          if ( !raw_buffer.empty() )
            {
              output.raw( raw_buffer.begin(), raw_buffer.end() );
              raw_buffer.clear();
            }

          output.encode( count, pattern );
        }
      else
        raw_buffer.insert( raw_buffer.end(), saved_it, first );
    }

  
  if ( !raw_buffer.empty() )
    output.raw( raw_buffer.begin(), raw_buffer.end() );
} // rle_encoder::encode()
