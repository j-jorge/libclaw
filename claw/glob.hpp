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
 * \file glob.hpp
 * \brief Globalization algorithm.
 * \author Julien Jorge
 */
#ifndef __CLAW_GLOB_HPP__
#define __CLAW_GLOB_HPP__

namespace claw
{
    template<typename InputIterator1, typename InputIterator2>
    bool glob_match
    ( InputIterator1 pattern_first, InputIterator1 pattern_last,
      InputIterator2 first, InputIterator2 last,
      typename InputIterator1::value_type any_sequence,
      typename InputIterator1::value_type zero_or_one,
      typename InputIterator1::value_type any );

    template<typename InputIterator1, typename InputIterator2>
    bool glob_potential_match
    ( InputIterator1 pattern_first, InputIterator1 pattern_last,
      InputIterator2 first, InputIterator2 last,
      typename InputIterator1::value_type any_sequence,
      typename InputIterator1::value_type zero_or_one,
      typename InputIterator1::value_type any );

} // namespace claw

#include <claw/impl/glob.tpp>

#endif // __CLAW_GLOB_HPP__
