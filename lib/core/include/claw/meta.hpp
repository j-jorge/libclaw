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
 * \file meta.hpp
 * \brief Structures for meta-programming.
 * \author Julien Jorge
 */

#ifdef __GNUC__
#warning                                                                      \
    "This header file is deprecated and will be removed in next release. Use the adequate file from claw/meta/ folder instead."
#elif def _MSVC_VER
#pragma message(                                                              \
    "This header file is deprecated and will be removed in next release. Use the adequate file from claw/meta/ folder instead.")
#endif

#include <claw/meta/conditional.hpp>
#include <claw/meta/same_type.hpp>
#include <claw/meta/type_list.hpp>
