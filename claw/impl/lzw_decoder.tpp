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
 * \file lzw_decoder.tpp
 * \brief Implementation of the claw::lzw_decoder class.
 * \author Julien Jorge
 */
#include <list>

/*----------------------------------------------------------------------------*/
/**
 * \brief Decode a sequence of LZW compressed datas.
 * \param input Where we read the compressed datas.
 * \param output Where we write uncompressed datas.
 */
template<typename InputBuffer, typename OutputBuffer>
void claw::lzw_decoder<InputBuffer, OutputBuffer>::decode
( input_buffer_type& input, output_buffer_type& output )
{
  const unsigned int symbols_count = input.symbols_count();

  table_type table;
  unsigned int table_size = 0;

  unsigned int prefix = input.get_next();

  if ( !input.end_of_data() )
    {
      while ( !input.end_of_data() )
        {
          unsigned int suffix = input.get_next();

          if (!input.end_of_data() )
            {
              unsigned int new_suffix;

              if ( suffix < table_size + symbols_count )
                new_suffix = get_first_symbol(table, suffix, symbols_count);
              else
                new_suffix = get_first_symbol(table, prefix, symbols_count);

              table.push_back( word_type(prefix, new_suffix) );
              ++table_size;
              input.new_code(table_size + symbols_count);

              decompose( table, prefix, symbols_count, output );
              prefix = suffix;
            }
        }

      decompose( table, prefix, symbols_count, output );
    }
} // lzw_decoder::decode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the first symbol of a string, represented by a code.
 * \param table The table of codes.
 * \param code The code of the string from which we want the first symbol.
 * \param symbols_count The count of atomic codes.
 */
template<typename InputBuffer, typename OutputBuffer>
unsigned int claw::lzw_decoder<InputBuffer, OutputBuffer>::get_first_symbol
( const table_type& table, const unsigned int code,
  const unsigned int symbols_count ) const
{
  unsigned int result = code;

  while ( result >= symbols_count )
    result = table[result - symbols_count].first;

  return result;
} // lzw_decoder::get_first_symbol()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a string, represented by a code, in the ouput buffer.
 * \param table The table of codes.
 * \param code The code of the string to write.
 * \param symbols_count The count of atomic codes.
 * \param output Where we write the uncompressed datas.
 */
template<typename InputBuffer, typename OutputBuffer>
void claw::lzw_decoder<InputBuffer, OutputBuffer>::decompose
( const table_type& table, unsigned int code,
  const unsigned int symbols_count, output_buffer_type& output ) const
{
  std::list<unsigned int> result;

  while ( code >= symbols_count )
    {
      result.push_front( table[code - symbols_count].second );
      code = table[code - symbols_count].first;
    }

  result.push_front(code);

  std::list<unsigned int>::const_iterator it;

  for (it=result.begin(); it!=result.end(); ++it)
    output.write( *it );

} // lzw_decoder::decompose()
