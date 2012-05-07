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
 * \file algorithm.tpp
 * \brief Generic algorithms on sequences.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply an unary function to all members of a sequence.
 *
 * This function works like std::for_each() but allows the function to apply
 * non-const methods to its argument.
 *
 * \param first Iterator on the first element of the sequence.
 * \param last Iterator just past the end of the sequence.
 * \param f Function to apply to the elements of the sequence.
 *
 * \remark The return value of the unary function is ignored.
 */
template<typename InputIterator, typename UnaryFunction>
UnaryFunction claw::inplace_for_each
( InputIterator first, InputIterator last, UnaryFunction f )
{
  for (; first!=last; ++first)
    f(*first);

  return f;
} // inplace_for_each()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the position in a range of the first element not in the
 *        elements of a given range.
 *
 * \param first1 Iterator on the first element of the sequence in which we
 *        search.
 * \param last1 Iterator just past the end of the sequence in which we search.
 * \param first2 Iterator on the first element of the range of the elements to
 *        skip.
 * \param last2 Iterator just past the end of the range of the elements to
 *        skip.
 */
template<typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator1 claw::find_first_not_of
( ForwardIterator1 first1, ForwardIterator1 last1,
  ForwardIterator2 first2, ForwardIterator2 last2 )
{
  for ( ; first1!=last1; ++first1 )
    {
      bool found(false);
      for ( ForwardIterator2 it(first2); !found && (it!=last2); ++it )
        found = *first1 == *it;

      if (!found)
        return first1;
    }

  return last1;
} // find_first_not_of()

/*----------------------------------------------------------------------------*/
/**
 * \brief Replace a set of elements in a range by other elements.
 * \param first Iterator on the first element in the range to modify.
 * \param last Iterator just past the end of the range to modify.
 * \param e1_first Iterator on the first element to replace.
 * \param e1_last Iterator just past the last element to replace.
 * \param e2_first Iterator on the first element to replace with.
 * \param e2_last Iterator just past the last element to replace with.
 *
 * \return The number of replaced elements.
 *
 * Each element *(e1_first + i) will be replaced with *(e2_first + i).
 * If the range (\a e1_first, \a e1_last) is smaller than
 * (\a e2_first, \a e2_last), the latter will be completed by repeating its last
 * element.
 *
 * \b Example :
 * <tt>
 * int* r = { 1, 0, 1, 1, 0, 2, 3, 0, 5, 1 };
 * unsigned int* e1 = { 0, 1, 2 };
 * unsigned int* e2 = { 1, 0 };
 * claw::replace( r, r+10, e1, e1+3, e2, e2+2 );
 * // r is now { 0, 1, 0, 0, 1, 0, 3, 1, 5, 0 }
 * </tt>
 */
template<typename ForwardIterator1, typename ForwardIterator2,
         typename ForwardIterator3>
std::size_t claw::replace
( ForwardIterator1 first, ForwardIterator1 last,
  ForwardIterator2 e1_first, ForwardIterator2 e1_last,
  ForwardIterator3 e2_first, ForwardIterator3 e2_last )
{
  if ( (e1_first == e1_last) || (e2_first == e2_last) )
    return 0;

  std::size_t count(0);

  for ( ; first != last; ++first )
    {
      bool stop(false);
      ForwardIterator3 r(e2_first);

      for (ForwardIterator2 it=e1_first; !stop && (it!=e1_last); ++it)
        {
          if ( *first == *it )
            {
              *first = *r;
              ++count;
              stop = true;
            }
          else
            {
              ForwardIterator3 n=r;
              ++n;
              if (n!=e2_last)
                r = n;
            }
        }
    }

  return count;
} // replace()
