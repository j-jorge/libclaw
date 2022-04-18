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
 * \file assert.hpp
 * \brief Some assert macros to strengthen you code.
 * \author Julien Jorge
 */
#ifndef __CLAW_ASSERT_HPP__
#define __CLAW_ASSERT_HPP__

#include <cstdlib>
#include <iostream>
#include <sstream>

/**
 * \fn CLAW_ASSERT( b, s )
 * \brief Print a message on std::cerr and stop the program if a condition is
 *        not true.
 * \param b Condition to verify.
 * \param s An error message.
 */
/**
 * \fn CLAW_FAIL( s )
 * \brief Print a message on std::cerr and stop the program.
 * \param s An error message.
 */
/**
 * \fn CLAW_PRECOND( b )
 * \brief Abort the program if a precondition is not true.
 * \param b Condition to verify.
 */
/**
 * \fn CLAW_POSTCOND( b )
 * \brief Abort the program if a postcondition is not true.
 * \param b Condition to verify.
 */

#ifndef NDEBUG

namespace claw
{
  /**
   * \brief The method used by CLAW_ASSERT macro. Souldn't be used elsewhere.
   * \param file The file where the error is produced.
   * \param func The name of the current function.
   * \param line The line where the error is produced.
   * \param b A boolean condition to verify.
   * \param s Message printed if b is not verified.
   */
  inline void debug_assert(const char* file, const char* func,
                           unsigned int line, bool b, const std::string& s)
  {
    if(!(b))
      {
        std::cerr << file << ":" << line << "\n\t" << func
                  << " : assertion failed\n\t" << s << std::endl;

#ifndef CLAW_SOFT_ASSERT
        abort();
#endif
      }
  }

}

#define CLAW_ASSERT(b, s)                                                     \
  claw::debug_assert(__FILE__, __FUNCTION__, __LINE__, (b), (s))

#define CLAW_FAIL(s)                                                          \
  claw::debug_assert(__FILE__, __FUNCTION__, __LINE__, false, (s))

#define CLAW_PRECOND(b) CLAW_ASSERT(b, "precondition failed: " #b)

#define CLAW_POSTCOND(b) CLAW_ASSERT(b, "postcondition failed: " #b)

#else // def NDEBUG

#define CLAW_ASSERT(b, s)
#define CLAW_FAIL(s)
#define CLAW_PRECOND(b)
#define CLAW_POSTCOND(b)

#endif // NDEBUG

#endif // __CLAW_ASSERT_HPP__
