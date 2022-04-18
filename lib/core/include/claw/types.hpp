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
 * \file types.hpp
 * \brief Some classes for the raw manipulation of the base types.
 * \author Julien Jorge
 */

#include <claw/meta/conditional.hpp>
#include <claw/meta/type_list.hpp>

#ifndef __CLAW_TYPES_HPP__
#define __CLAW_TYPES_HPP__

namespace claw
{
#ifdef CLAW_HAS_LONG_LONG

  typedef meta::type_list<signed long long int, meta::no_type>
      non_standard_signed_types;

  typedef meta::type_list<unsigned long long int, meta::no_type>
      non_standard_unsigned_types;

#else // !def CLAW_HAS_LONG_LONG

  typedef meta::no_type non_standard_signed_types;
  typedef meta::no_type non_standard_unsigned_types;

#endif // CLAW_HAS_LONG_LONG

  /** \brief This is the list of c++ signed integer types. */
  typedef meta::type_list<
      signed char,
      meta::type_list<
          signed short,
          meta::type_list<signed int, non_standard_signed_types> > >
      signed_integers;

  /** \brief This is the list of c++ unsigned integer types. */
  typedef meta::type_list<
      unsigned char,
      meta::type_list<
          unsigned short,
          meta::type_list<unsigned int, non_standard_unsigned_types> > >
      unsigned_integers;

  /**
   * \brief This meta class finds, in a list of types, the first type stored
   *        exactly with a given number of bits.
   *
   * \b Template \b parameters
   *  - \a Size The number of bits in the type to find,
   *  - \a TypeList A list of types (see meta::type_list).
   */
  template <std::size_t Size, typename TypeList>
  struct find_type_by_size
  {
  private:
    typedef typename TypeList::head_type head_type;
    typedef typename TypeList::queue_type queue_type;

  public:
    /** \brief If the size of head_type matches the desired one, then the
       result
        is head_type, otherwise we search the type in the remaining ones . */
    typedef typename meta::if_then_else<
        sizeof(head_type) * 8 == Size, head_type,
        typename find_type_by_size<Size, queue_type>::type>::result type;

  }; // find_type_by_size

  /** \brief End of the recursion of the find_type_by_size class. */
  template <std::size_t Size>
  struct find_type_by_size<Size, meta::no_type>
  {
    /** \brief This is an incomplete type that should make the compiler
        failing. */
    struct type;
  }; // find_type_by_size

  /**
   * \brief Define the type of a signed integer stored with a given number of
   *        bits.
   * \b Template \b parameters
   *  - \a Size The number of bytes in the types.
   */
  template <std::size_t Size>
  struct integer_of_size
  {
    /** \brief The integer type that matches the given size. */
    typedef typename find_type_by_size<Size, signed_integers>::type type;

  }; // struct integer_of_size

  /**
   * \brief Define the type of an unsigned integer stored with a given number
   * of bits. \b Template \b parameters
   *  - \a Size The number of bytes in the types.
   */
  template <std::size_t Size>
  struct unsigned_integer_of_size
  {
    /** \brief The integer type that matches the given size. */
    typedef typename find_type_by_size<Size, unsigned_integers>::type type;

  }; // struct unsigned_integer_of_size

  /** \brief An unsigned integer on 8 bits. */
  typedef unsigned_integer_of_size<8>::type u_int_8;

  /** \brief An unsigned integer on 16 bits. */
  typedef unsigned_integer_of_size<16>::type u_int_16;

  /** \brief An unsigned integer on 32 bits. */
  typedef unsigned_integer_of_size<32>::type u_int_32;

  /** \brief An integer on 8 bits. */
  typedef integer_of_size<8>::type int_8;

  /** \brief An integer on 16 bits. */
  typedef integer_of_size<16>::type int_16;

  /** \brief An integer on 32 bits. */
  typedef integer_of_size<32>::type int_32;

}

#endif // __CLAW_TYPES_HPP__
