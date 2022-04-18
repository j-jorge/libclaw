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
 * \file claw/code/tween/single_tweener.cpp
 * \brief Implementation of the claw::tween::single_tweener class.
 * \author Julien Jorge
 */
#include <claw/tween/single_tweener.hpp>

#include <algorithm>

/**
 * \brief Default constructor.
 */
claw::tween::single_tweener::single_tweener()
  : m_date(0)
  , m_easing(easing_none::ease_in_out)
{}

/**
 * \brief Constructor.
 * \param init The initial value.
 * \param end The final value.
 * \param duration The total duration.
 * \param callback The function called when the single_tweener is updated.
 * \param e The function used to compute the new value.
 */
claw::tween::single_tweener::single_tweener(double init, double end,
                                            double duration,
                                            update_function callback,
                                            easing_function e)
  : m_init(init)
  , m_end(end)
  , m_date(0)
  , m_duration(duration)
  , m_callback(callback)
  , m_easing(e)
{}

/**
 * \brief Constructor.
 * \param val The value to tween (and to use as the The initial value).
 * \param end The final value.
 * \param duration The total duration.
 * \param e The function used to compute the new value.
 */
claw::tween::single_tweener::single_tweener(double& val, double end,
                                            double duration, easing_function e)
  : m_init(val)
  , m_end(end)
  , m_date(0)
  , m_duration(duration)
  , m_easing(e)
{
  m_callback = [&val](double v) -> void
  {
    val = v;
  };
}

/**
 * \brief Gets the initial value.
 */
double claw::tween::single_tweener::get_init() const
{
  return m_init;
}

/**
 * \brief Sets the initial value.
 * \param v The value.
 */
void claw::tween::single_tweener::set_init(double v)
{
  m_init = v;
}

/**
 * \brief Gets the final value.
 */
double claw::tween::single_tweener::get_end() const
{
  return m_end;
}

/**
 * \brief Sets the final value.
 * \param v The value.
 */
void claw::tween::single_tweener::set_end(double v)
{
  m_end = v;
}

/**
 * \brief Gets the total duration.
 */
double claw::tween::single_tweener::get_duration() const
{
  return m_duration;
}

/**
 * \brief Sets the total duration.
 * \param v The value.
 */
void claw::tween::single_tweener::set_duration(double v)
{
  m_duration = v;
}

/**
 * \brief The function called when the single_tweener is updated.
 * \param f The function.
 */
void claw::tween::single_tweener::set_callback(update_function f)
{
  m_callback = f;
}

/**
 * \brief The function used to compute the new value.
 * \param f The function.
 */
void claw::tween::single_tweener::set_easing(easing_function f)
{
  m_easing = f;
}

/**
 * \brief Gets the current value of the tweener.
 */
double claw::tween::single_tweener::get_value() const
{
  const double coeff = m_easing(m_date / m_duration);
  return m_init + coeff * (m_end - m_init);
}

/**
 * \brief Create a copy of this allocated with new.
 */
claw::tween::single_tweener* claw::tween::single_tweener::do_clone() const
{
  return new single_tweener(*this);
}

/**
 * \brief Tell if the single_tweener has reached his total duration.
 */
bool claw::tween::single_tweener::do_is_finished() const
{
  return m_date >= m_duration;
}

/**
 * \brief Update the single_tweener of a given amount of time.
 * \param dt The duration of the update in time units since the last call.
 */
double claw::tween::single_tweener::do_update(double dt)
{
  const double t(std::min(m_duration - m_date, dt));
  const double result = dt - t;
  m_date += t;

  const double val(get_value());

  if(m_callback)
    m_callback(val);

  return result;
}
