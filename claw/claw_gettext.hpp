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
 * \file claw_gettext.hpp
 * \brief Macros to call gettext on the libclaw textdomain.
 * \author Julien Jorge
 */
#ifndef __CLAW_CLAW_GETTEXT_HPP__
#define __CLAW_CLAW_GETTEXT_HPP__

#include <libintl.h>

/**
 * \brief Call gettext on the default text domain used by Claw.
 * \param s The string to translate.
 */
#define claw_gettext(s) dgettext( "libclaw", (s) )

/**
 * \brief Call ngettext on the default text domain used by Claw.
 * \param s The string to translate by default.
 * \param p The plural version of s.
 * \param n The number thar triggers the choice of \a s or \a p.
 */
#define claw_ngettext(s, p, n) dngettext( "libclaw", (s), (p), (n) )

#endif // __CLAW_CLAW_GETTEXT_HPP__
