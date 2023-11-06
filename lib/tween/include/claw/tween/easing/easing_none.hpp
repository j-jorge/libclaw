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
 * \file claw/tween/easing/easing_none.hpp
 * \brief Easing functions for the tweener. Those functions do nothing.
 * \author Julien Jorge
 */
#ifndef __CLAW_TWEEN_EASING_NONE_HPP__
#define __CLAW_TWEEN_EASING_NONE_HPP__

namespace claw
{
  namespace tween
  {
    /**
     * \brief Easing functions for the tweener. Those functions do nothing.
     * \author Julien Jorge
     */
    class easing_none
    {
    public:
      static double ease_in(double t);
      static double ease_out(double t);
      static double ease_in_out(double t);

    }; // class easing_none

  }
}

#endif // __CLAW_TWEEN_EASING_NONE_HPP__
