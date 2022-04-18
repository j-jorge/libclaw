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
 * \file rle_decoder.tpp
 * \brief Implementation of the claw::rle_decoder class and subclasses.
 * \author Julien Jorge
 */

/**
 * \brief Constructor.
 */
template <typename Pattern, typename InputBuffer, typename OutputBuffer>
claw::rle_decoder<Pattern, InputBuffer, OutputBuffer>::rle_decoder()
  : m_mode(stop)
  , m_count(0)
{}

/**
 * \brief Destructor.
 */
template <typename Pattern, typename InputBuffer, typename OutputBuffer>
claw::rle_decoder<Pattern, InputBuffer, OutputBuffer>::~rle_decoder()
{}

/**
 * \brief Decode a RLE stream.
 * \param input The RLE stream.
 * \param output The raw stream.
 */
template <typename Pattern, typename InputBuffer, typename OutputBuffer>
void claw::rle_decoder<Pattern, InputBuffer, OutputBuffer>::decode(
    input_buffer_type& input, output_buffer_type& output)
{
  m_mode = stop;
  read_mode(input, output);

  while(m_mode != stop)
    {
      if(m_mode == compressed)
        output.fill(m_count, m_pattern);
      else
        output.copy(m_count, input);

      read_mode(input, output);
    }
}
