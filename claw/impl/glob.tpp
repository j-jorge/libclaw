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
 * \file string_algorithm.tpp
 * \brief Implementation of the globalization algorithms.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if a sequence matches a given pattern.
 * \param pattern_first Iterator on the beginning of the pattern.
 * \param pattern_last Iterator just past the end of the pattern.
 * \param first Iterator on the beginning of the sequence.
 * \param last Iterator just last the end of the sequence.
 * \param any_sequence A value representing any sequence of values, empty or
 *        not.
 * \param zero_or_one A value representing any value or no value.
 * \param any A value representing any value.
 */
template<typename InputIterator1, typename InputIterator2>
bool claw::glob_match
( InputIterator1 pattern_first, InputIterator1 pattern_last,
  InputIterator2 first, InputIterator2 last,
  typename InputIterator1::value_type any_sequence,
  typename InputIterator1::value_type zero_or_one,
  typename InputIterator1::value_type any )
{
  bool result(false);

  if ( (pattern_first == pattern_last) || (first == last) )
    {
      result = (first == last);

      for ( ; result && (pattern_first != pattern_last); ++ pattern_first )
	result =
	  (*pattern_first == any_sequence) || (*pattern_first == zero_or_one);
    }
  else if ( *pattern_first == any_sequence )
    result =
      glob_match
      ( pattern_first + 1, pattern_last, first, last, any_sequence, zero_or_one,
	any)
      || glob_match
      ( pattern_first, pattern_last, first + 1, last, any_sequence, zero_or_one,
	any );
  else if ( *pattern_first == zero_or_one )
    result =
      glob_match
      ( pattern_first + 1, pattern_last, first, last, any_sequence, zero_or_one,
	any)
      || glob_match
      ( pattern_first + 1, pattern_last, first + 1, last, any_sequence,
	zero_or_one, any );
  else if ( (*pattern_first == zero_or_one) || (*pattern_first == *first) )
    result =
      glob_match
      ( pattern_first + 1, pattern_last, first + 1, last, any_sequence,
	zero_or_one, any );
  else
    result = false;

  return result;
} // glob_match()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if a sequence may match a given pattern.
 * \param pattern_first Iterator on the beginning of the pattern.
 * \param pattern_last Iterator just past the end of the pattern.
 * \param first Iterator on the beginning of the sequence.
 * \param last Iterator just last the end of the sequence.
 * \param any_sequence A value representing any sequence of values, empty or
 *        not.
 * \param zero_or_one A value representing any value or no value.
 * \param any A value representing any value.
 */
template<typename InputIterator1, typename InputIterator2>
bool claw::glob_potential_match
( InputIterator1 pattern_first, InputIterator1 pattern_last,
  InputIterator2 first, InputIterator2 last,
  typename InputIterator1::value_type any_sequence,
  typename InputIterator1::value_type zero_or_one,
  typename InputIterator1::value_type any )
{
  bool result(true);
  bool stop(false);

  while ( !stop && (pattern_first != pattern_last) && (first != last) )
    if ( (*pattern_first == any_sequence) || (*pattern_first == zero_or_one) )
      stop = true;
    else if ( *pattern_first == any )
      {
	++pattern_first;
	++first;
      }
    else if ( *pattern_first == *first )
      {
	++pattern_first;
	++first;
      }
    else
      {
	result = false;
	stop = true;
      }

  return result;
} // glob_potential_match()
