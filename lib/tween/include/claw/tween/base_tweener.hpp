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
 * \file claw/tween/base_tweener.hpp
 * \brief Common interface for all tweeners.
 * \author Julien Jorge
 */
#ifndef __CLAW_TWEEN_BASE_TWEENER_HPP__
#define __CLAW_TWEEN_BASE_TWEENER_HPP__

#include <functional>
#include <list>

namespace claw
{
  namespace tween
  {
    /**
     * \brief Common interface for all tweeners.
     * \author Julien Jorge
     * \sa \ref tweeners
     */
    class base_tweener
    {
    public:
      /**
       * \brief The type of the function called to notify the end of the
       *        tweener.
       */
      typedef std::function<void()> finish_callback;

    public:
      virtual ~base_tweener();

      base_tweener* clone() const;

      bool is_finished() const;
      double update(double dt);

      void on_finished(finish_callback f);

    private:
      void notify_finished() const;

      /**
       * \brief Create a copy of this allocated with new.
       */
      virtual base_tweener* do_clone() const = 0;

      /**
       * \brief Tell if this tweener is over.
       */
      virtual bool do_is_finished() const = 0;

      /**
       * \brief Update the tweener for a given amount of time.
       * \param dt The duration of the update.
       * \return The amount of extra time in dt after the end of the update.
       */
      virtual double do_update(double dt) = 0;

    private:
      /** \brief The callback executed when the tweener is finished. */
      std::list<finish_callback> m_on_finished;

    }; // class base_tweener

  }
}

#endif // __CLAW_TWEEN_BASE_TWEENER_HPP__
