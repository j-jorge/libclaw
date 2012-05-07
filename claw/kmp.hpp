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
 * \file kmp.hpp
 * \brief kmp class interface. Use this class for exact pattern matching.
 *        This class uses the Knuth-Morris-Pratt's algorithm.
 * \author Julien Jorge
 */
#ifndef __CLAW_KMP_HPP__
#define __CLAW_KMP_HPP__

#include <map>

namespace claw
{
  namespace text
  {
    /**
     * \brief Exact pattern finding with the Knuth-Morris-Pratt's algorithm.
     * \author Julien Jorge
     */
    template<class RandomIterator> class kmp
    {
    public:
      template <class UnaryPredicate>
      void operator()(const RandomIterator pattern_begin, 
                      const RandomIterator pattern_end,
                      const RandomIterator text_begin,
                      const RandomIterator text_end, 
                      UnaryPredicate& action) const;

    private:
      unsigned int common_prefix_length( const RandomIterator begin_1,
                                         const RandomIterator begin_2,
                                         const RandomIterator end_1,
                                         const RandomIterator end_2 ) const;

      void z_boxes(const RandomIterator begin, const RandomIterator end,
                   std::map<unsigned int, unsigned int>& out) const;

      void spi_prime(const RandomIterator begin, const RandomIterator end,
                     std::map<unsigned int, unsigned int>& out) const;
    }; // class kmp

  } // namespace text
} // namespace claw

#include <claw/impl/kmp.tpp>

#endif // __CLAW_KMP_HPP__
