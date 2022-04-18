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
 * \file claw/tween/single_tweener.hpp
 * \brief A single_tweener makes a value to evolve through time from a initial
 *        value to an end value according to a given function.
 * \author Julien Jorge
 */
#ifndef __CLAW_TWEEN_SINGLE_TWEENER_HPP__
#define __CLAW_TWEEN_SINGLE_TWEENER_HPP__

#include <claw/tween/base_tweener.hpp>
#include <claw/tween/easing/easing_none.hpp>

#include <functional>

namespace claw
{
  namespace tween
  {
    /**
     * \brief A single_tweener makes a value to evolve through time from a
     *        initial value to an end value according to a given function.
     * \author Julien Jorge
     * \sa \ref tweeners
     */
    class single_tweener : public base_tweener
    {
    public:
      /** \brief The type of the function called when the single_tweener is
          updated. */
      typedef std::function<void(double)> update_function;

      /** \brief The type of the function used to compute the new value. */
      typedef std::function<double(double)> easing_function;

    public:
      single_tweener();
      single_tweener(double init, double end, double duration,
                     update_function callback, easing_function e);
      single_tweener(double& val, double end, double duration,
                     easing_function e);

      double get_init() const;
      void set_init(double v);

      double get_end() const;
      void set_end(double v);

      double get_duration() const;
      void set_duration(double v);

      void set_callback(update_function f);
      void set_easing(easing_function f);

      double get_value() const;

    private:
      single_tweener* do_clone() const;
      bool do_is_finished() const;
      double do_update(double dt);

    private:
      /** \brief The initial value. */
      double m_init;

      /** \brief The final value. */
      double m_end;

      /** \brief The current date in the timeline of the single_tweener. */
      double m_date;

      /** \brief The total duration of the single_tweener. */
      double m_duration;

      /** \brief The function called when the single_tweener is updated. */
      update_function m_callback;

      /** \brief The function used to compute the new value. */
      easing_function m_easing;

    }; // class single_tweener

  }
}

#endif // __CLAW_TWEEN_SINGLE_TWEENER_HPP__
