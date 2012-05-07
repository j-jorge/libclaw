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
 * \file claw/tween/tweener.hpp
 * \brief A tweener makes a value to evolve through time from a initial value to
 *        an end value according to a given function.
 * \author Julien Jorge
 */
#ifndef __CLAW_TWEEN_TWEENER_HPP__
#define __CLAW_TWEEN_TWEENER_HPP__

#include <algorithm>
#include <boost/function.hpp>

namespace claw
{
  namespace tween
  {
    class base_tweener;

    /**
     * \brief A tweener makes a value to evolve through time from a initial
     *        value to an end value according to a given function.
     * \author Julien Jorge
     * \sa \ref tweeners
     */
    class tweener
    {
    public:
      /**
       * \brief The type of the function called to notify the end of the
       *        tweener.
       */
      typedef boost::function<void ()> finish_callback;

    public:
      tweener();
      tweener( const tweener& that );
      tweener( const base_tweener& that );
      ~tweener();

      tweener& operator=( const tweener& that );

      void swap( tweener& that ) throw();

      bool is_finished() const;
      double update( double dt );

      void on_finished( finish_callback f );
      
    private:
      /** \brief The implementation of this tweener. */
      base_tweener* m_impl;
      
    }; // class tweener

  } // namespace tween
} // namespace claw

namespace std
{
  template<>
  void swap( claw::tween::tweener& a, claw::tween::tweener& b );
} // namespace std

#endif // __CLAW_TWEEN_TWEENER_HPP__
