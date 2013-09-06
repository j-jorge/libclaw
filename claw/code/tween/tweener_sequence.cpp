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
 * \file claw/code/tween/tweener_sequence.cpp
 * \brief Implementation of the claw::tween::tweener_sequence class.
 * \author Julien Jorge
 */
#include "claw/tween/tweener_sequence.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a tweener in the sequence.
 * \param t The tweener.
 */
void claw::tween::tweener_sequence::insert( const tweener& t )
{
  m_tweeners.push_back(t);
} // tweener_sequence::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all the tweeners from the sequence.
 */
void claw::tween::tweener_sequence::clear()
{
  m_tweeners.clear();
} // tweener_sequence::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this, allocated with new.
 */
claw::tween::tweener_sequence* claw::tween::tweener_sequence::do_clone() const
{
  return new tweener_sequence(*this);
} // tweener_sequence::do_clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the tweener has reached his total duration.
 */
bool claw::tween::tweener_sequence::do_is_finished() const
{
  return m_tweeners.empty();
} // tweener_sequence::do_is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the tweeners by a given amount of time.
 * \param dt The duration of the update in time units since the last call.
 */
double claw::tween::tweener_sequence::do_update( double dt )
{
  double result(dt);

  while ( (result != 0) && !m_tweeners.empty() )
    {
      result = m_tweeners.front().update(result);

      if ( m_tweeners.front().is_finished() )
        m_tweeners.erase( m_tweeners.begin() );
    }

  return result;
} // tweener_sequence::do_update()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if there is nothing in this sequence.
 */
bool claw::tween::tweener_sequence::empty() const
{
  return m_tweeners.empty();
} // tweener_sequence::empty()

