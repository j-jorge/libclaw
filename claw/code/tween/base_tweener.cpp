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
 * \file claw/code/tween/base_tweener.cpp
 * \brief Implementation of the claw::tween::base_tweener class.
 * \author Julien Jorge
 */
#include <claw/tween/base_tweener.hpp>

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
claw::tween::base_tweener::~base_tweener()
{

} // base_tweener::~base_tweener()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this allocated with new.
 */
claw::tween::base_tweener* claw::tween::base_tweener::clone() const
{
  return do_clone();
} // base_tweener::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the tweener has reached his total duration.
 */
bool claw::tween::base_tweener::is_finished() const
{
  return do_is_finished();
} // base_tweener::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the base_tweener of a given amount of time.
 * \param dt The duration of the update in time units since the last call.
 * \return The amount of extra time in dt after the end of the update.
 */
double claw::tween::base_tweener::update( double dt )
{
  CLAW_PRECOND( dt >= 0 );

  const double result = do_update(dt);

  if ( is_finished() )
    notify_finished();

  CLAW_POSTCOND( result <= dt );
  CLAW_POSTCOND( result >= 0 );

  return result;
} // base_tweener::update()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute the callbacks notifying about the finish of the tweener.
 */
void claw::tween::base_tweener::on_finished( finish_callback f )
{
  m_on_finished.push_back(f);
} // base_tweener::on_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute the callbacks notifying about the finish of the tweener.
 */
void claw::tween::base_tweener::notify_finished() const
{
  // If one of the callbacks deletes the tweener, then m_on_finished will not be
  // available. Since we still want to execute all the callbacks, we iterate on
  // a copy of it.
  const std::list<finish_callback> callbacks(m_on_finished);

  for ( std::list<finish_callback>::const_iterator it=callbacks.begin();
        it!=callbacks.end(); ++it )
    (*it)();
} // base_tweener::notify_finished()
