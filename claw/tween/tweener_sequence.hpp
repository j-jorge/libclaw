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
 * \file claw/tween/tweener_sequence.hpp
 * \brief The tweener sequence manages several tweeners in a common timeline.
 * \author Julien Jorge
 */
#ifndef __CLAW_TWEEN_TWEENER_SEQUENCE_HPP__
#define __CLAW_TWEEN_TWEENER_SEQUENCE_HPP__

#include <claw/tween/base_tweener.hpp>
#include <claw/tween/tweener.hpp>

#include <list>

namespace claw
{
  namespace tween
  {
    /**
     * \brief The tweener sequence manages several tweeners in a common
     *        timeline.
     *
     * A tweener in a tweener sequence will start only when all the previous
     * tweeners in the same sequence are over.
     *
     * \author Julien Jorge
     * \sa \ref tweeners
     */
    class tweener_sequence:
      public base_tweener
    {
    public:
      void insert( const tweener& t );

      void clear();
      bool empty() const;

    private:
      tweener_sequence* do_clone() const;
      bool do_is_finished() const;
      double do_update( double dt );

    private:
      /** \brief The tweeners. */
      std::list<tweener> m_tweeners;
      
    }; // class tweener_sequence

  } // namespace tween
} // namespace claw

#endif // __CLAW_TWEEN_TWEENER_SEQUENCE_HPP__
