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
 * \file conditional.hpp
 * \brief Structures for meta-programming.
 * \author Julien Jorge
 */
#ifndef __CLAW_CONDITIONAL_HPP__
#define __CLAW_CONDITIONAL_HPP__

namespace claw
{
  namespace meta
  {
    /**
     * \brief This structure allows to do conditional typing.
     *
     * \b Template \b parameters
     * - \a Cond The result of the predicate to check.
     * - \a ThenPart The type defined when the predicate is true.
     * - \a ElsePart The type defined when the predicate is false.
     *
     * The result can be retrieved via the if_then_else::result type.
     *
     * \author Julien Jorge
     */
    template<bool Cond, typename ThenPart, typename ElsePart>
    struct if_then_else;

    /**
     * \brief Specialization for the case where the condition evaluates to
     *        true.
     */
    template<typename ThenPart, typename ElsePart>
    struct if_then_else<true, ThenPart, ElsePart>
    {
      /** \brief When the condition evaluates to true, the result is the "else"
          part. */
      typedef ThenPart result;

    }; // struct if_then_else [true]

    /**
     * \brief Specialization for the case where the condition evaluates to
     *        false.
     */
    template<typename ThenPart, typename ElsePart>
    struct if_then_else<false, ThenPart, ElsePart>
    {
      /** \brief When the condition evaluates to false, the result is the "else"
          part. */
      typedef ElsePart result;

    }; // struct if_then_else [false]

  } // namespace meta
} // namespace claw

#endif // __CLAW_CONDITIONAL_HPP__
