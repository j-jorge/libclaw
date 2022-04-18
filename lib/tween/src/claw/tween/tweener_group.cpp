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
 * \file claw/code/tween/tweener_group.cpp
 * \brief Implementation of the claw::tween::tweener_group class.
 * \author Julien Jorge
 */
#include "claw/tween/tweener_group.hpp"

/**
 * \brief Add a tweener in the group.
 * \param t The tweener.
 */
void claw::tween::tweener_group::insert(const tweener& t)
{
  m_tweeners.push_back(t);
}

/**
 * \brief Remove all the tweeners from the group.
 */
void claw::tween::tweener_group::clear()
{
  m_tweeners.clear();
}

/**
 * \brief Create a copy of this, allocated with new.
 */
claw::tween::tweener_group* claw::tween::tweener_group::do_clone() const
{
  return new tweener_group(*this);
}

/**
 * \brief Tell if the tweener has reached his total duration.
 */
bool claw::tween::tweener_group::do_is_finished() const
{
  return m_tweeners.empty();
}

/**
 * \brief Update the tweeners by a given amount of time.
 * \param dt The duration of the update in time units since the last call.
 */
double claw::tween::tweener_group::do_update(double dt)
{
  typedef std::vector<tweener>::iterator iterator_type;
  double result(dt);

  iterator_type it = m_tweeners.begin();

  while(it != m_tweeners.end())
    {
      const double r = it->update(dt);
      result = std::min(result, r);

      if(it->is_finished())
        it = m_tweeners.erase(it);
      else
        ++it;
    }

  return result;
}

/**
 * \brief Tells if there is nothing in this group.
 */
bool claw::tween::tweener_group::empty() const
{
  return m_tweeners.empty();
}
