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
 * \file kmp.tpp
 * \brief Implementation of the kmp class.
 * \author Julien Jorge
 */
#include <map>
#include <assert.h>
#include <claw/it_index.hpp>

/*---------------------------------------------------------------------------*/
/**
 * \brief Calculate the length of the longest common prefix between two
 *        patterns.
 * \param begin_1 Iterator on the first item in the first pattern.
 * \param begin_2 Iterator on the first item in the second pattern.
 * \param end_1 Iterator after the last item in the first pattern.
 * \param end_2 Iterator after the last item in the second pattern.
 */
template<class RandomIterator>
unsigned int 
claw::text::kmp<RandomIterator>::
common_prefix_length( const RandomIterator begin_1,
                      const RandomIterator begin_2,
                      const RandomIterator end_1, const RandomIterator end_2 
                      ) const
{
  unsigned int count = 0;
  RandomIterator it_1 = begin_1, it_2 = begin_2;
  bool quit = false;

  while ( (it_1 != end_1) && (it_2 != end_2) && ! quit )
    if ( *it_1 == *it_2 )
      {
        ++it_1;
        ++it_2;
        ++count;
      }
    else
      quit = true;

  return count;
} // common_prefix_length()

/*---------------------------------------------------------------------------*/
/**
 * \brief Preprocessing. Calculate the pattern's z-boxes.
 * \param begin Iterator on the first item in the pattern.
 * \param end Iterator after the last item in the pattern.
 * \param out (out) Calculated z-boxes.
 * \remark out contains only non-zero z-boxes.
 */
template<class RandomIterator>
void claw::text::kmp<RandomIterator>::z_boxes(const RandomIterator begin,
                                              const RandomIterator end,
                                              std::map<unsigned int,unsigned int>& out) const
{
  // right and left bounds of the current item's Z-box.
  claw::it_index<RandomIterator> it_r(begin);
  claw::it_index<RandomIterator> it_l(begin); 
  claw::it_index<RandomIterator> it_k(begin);      // increment on the items
  unsigned int z;                   // le Zi of the current position

  for (++it_k; it_k!=end; ++it_k)
    {
      if (it_k > it_r)
        {
          z = common_prefix_length(begin, it_k, end, end);
                  
          if ( z > 0 ) // set the Z-box
            {
              out[it_k] = z;
              it_l = it_k;
              it_r = it_k.operator+(z) - 1;
            }
        }
      else /* k <= r */
        {
          unsigned int k_bis = it_k - it_l;
	  claw::it_index<RandomIterator> it_b(it_r - it_k);

          if ( out.find(k_bis) == out.end() )
            z = 0;
          else
            z = out[k_bis];

          if ( z <= (unsigned int)it_b )
            {
              if ( z > 0 )
                out[it_k] = z;
            }
          else
            {
              claw::it_index<RandomIterator> it_q = it_r + 1;
                          
              it_q += common_prefix_length(it_q, it_b+1, end, end);

              out[it_k] = it_q - it_k;
              it_r = it_q - 1;
              it_l = it_k;
            }
        }
    }
} // z_boxes()

/*---------------------------------------------------------------------------*/
/**
 * \brief Preprocessing. Calculate the pattern's spi' values.
 * \param begin Iterator on the first item in the pattern.
 * \param end Iterator after the last item in the pattern.
 * \param out (out) Calculated spi'.
 * \remark out contains only non-zero spi'.
 */
template<class RandomIterator>
void claw::text::kmp<RandomIterator>::spi_prime(const RandomIterator begin,
                                                const RandomIterator end,
                                                std::map<unsigned int, unsigned int>& out) const
{
  std::map<unsigned int, unsigned int> z;    // pattern's Z-boxes.
  unsigned int j;                            // loop index.

  // set Z-boxes
  z_boxes(begin, end, z);

  // calculates spi' (from end to begining)
  j=end-begin;

  do
    {
      --j;
      if (z.find(j) != z.end())
        out[j + z[j] - 1] = z[j];
    }
  while (j!=0);
} // spi_prime()

/*---------------------------------------------------------------------------*/
/**
 * \brief Pattern matching with the Knuth-Morris-Pratt's algorithm.
 * \param pattern_begin Iterator on the first item in the pattern.
 * \param pattern_end Iterator after the last item in the pattern.
 * \param text_begin Iterator on the first item in the text.
 * \param text_end Iterator after the last item in the text.
 * \param action Predicate called with the last found position for the pattern.
 * \remark Exits if action return false.
 * \pre pattern_begin != pattern_end
 */
template<class RandomIterator>
template<class UnaryPredicate>
void claw::text::kmp<RandomIterator>::operator()
  (const RandomIterator pattern_begin, const RandomIterator pattern_end,
   const RandomIterator text_begin, const RandomIterator text_end, 
   UnaryPredicate& action ) const
{
  std::map<unsigned int, unsigned int> spi; // pattern's spi'
  claw::it_index<RandomIterator> it_p(pattern_begin,1);
  claw::it_index<RandomIterator> it_t(text_begin,1);
  bool stop = false;    // result of the last call to the predicate action

  const int pattern_length = pattern_end - pattern_begin;
  const int text_length    = text_end    - text_begin;

  assert(pattern_begin != pattern_end);

  spi_prime(pattern_begin, pattern_end, spi);

  unsigned int i = 0;

  while ( ((int)it_t <= text_length - (pattern_length - it_p)) && !stop )
    {
      unsigned int common;

      common = common_prefix_length(it_p, it_t, pattern_end, text_end);
      i += common;
      it_p += common;
      it_t += common;

      if (it_p == 1)
	++it_t;
      else
        {
	  if ( (int)it_p == pattern_length+1 )
	    stop = !action( (int)it_t - pattern_length-1 );

	  i = spi[i-1];
	  it_p.set(pattern_begin+i, i+1);
        }
    }
} // operator()
