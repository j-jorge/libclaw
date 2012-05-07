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
 * \file string_algorithm.hpp
 * \brief Generic algorithms on strings.
 * \author Julien Jorge
 */
#ifndef __CLAW_STRING_ALGORITHM_HPP__
#define __CLAW_STRING_ALGORITHM_HPP__

#include <cstddef>

namespace claw
{
  namespace text
  {
    template<typename StreamType, typename StringType>
    StreamType& getline( StreamType& is, StringType& str );

    template<typename StringType>
    void trim_left( StringType& str,
                    const typename StringType::value_type* const s = " " );

    template<typename StringType>
    void trim_right( StringType& str,
                     const typename StringType::value_type* const s = " " );

    template<typename StringType>
    void trim( StringType& str,
               const typename StringType::value_type* const s = " " );

    template<typename StringType>
    void squeeze( StringType& str,
                  const typename StringType::value_type* const s );

    template<typename StringType>
    std::size_t
    replace( StringType& str, const StringType& e1, const StringType& e2 );

    template<typename T, typename StringType>
    bool is_of_type( const StringType& str );

    template<typename Sequence>
    void split
    ( Sequence& sequence, const typename Sequence::value_type& str,
      const typename Sequence::value_type::value_type sep );

    template<typename Sequence>
    void split
    ( Sequence& sequence, typename Sequence::value_type::const_iterator first,
      typename Sequence::value_type::const_iterator last,
      const typename Sequence::value_type::value_type sep );

    template<typename InputIterator, typename OutputIterator>
    void c_escape
    ( InputIterator first, InputIterator last, OutputIterator out );

    template<typename StringType>
    bool glob_match( const StringType& pattern, const StringType& text,
		     const typename StringType::value_type any_sequence = '*',
		     const typename StringType::value_type zero_or_one = '?',
		     const typename StringType::value_type any = '.' );

    template<typename StringType>
    bool glob_potential_match
    ( const StringType& pattern, const StringType& text,
      const typename StringType::value_type any_sequence = '*',
      const typename StringType::value_type zero_or_one = '?',
      const typename StringType::value_type any = '.' );

  } // namespace text
} // namespace claw

#include <claw/impl/string_algorithm.tpp>

#endif // __CLAW_STRING_ALGORITHM_HPP__
