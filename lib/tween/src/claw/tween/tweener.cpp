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
 * \file claw/code/tween/tweener.cpp
 * \brief Implementation of the claw::tween::tweener class.
 * \author Julien Jorge
 */
#include <claw/tween/tweener.hpp>

#include <claw/tween/base_tweener.hpp>

/**
 * \brief Default constructor.
 */
claw::tween::tweener::tweener()
  : m_impl(NULL)
{}

/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
claw::tween::tweener::tweener(const tweener& that)
  : m_impl(that.m_impl == NULL ? NULL : that.m_impl->clone())
{}

/**
 * \brief Constructor from a base_tweener.
 * \param that The instance to use for the implementation.
 */
claw::tween::tweener::tweener(const base_tweener& that)
  : m_impl(that.clone())
{}

/**
 * \brief Destructor.
 */
claw::tween::tweener::~tweener()
{
  delete m_impl;
}

/**
 * \brief Assignment operator.
 * \param that The instance to copy from.
 */
claw::tween::tweener& claw::tween::tweener::operator=(const tweener& that)
{
  tweener tmp(that);
  swap(tmp);
  return *this;
}

/**
 * \brief Swap this instance with a given instance.
 * \param that The instance to swap with.
 */
void claw::tween::tweener::swap(tweener& that) throw()
{
  std::swap(m_impl, that.m_impl);
}

/**
 * \brief Tell if the tweener has reached his total duration.
 */
bool claw::tween::tweener::is_finished() const
{
  if(m_impl == NULL)
    return true;
  else
    return m_impl->is_finished();
}

/**
 * \brief Update the tweener of a given amount of time.
 * \param dt The duration of the update in time units since the last call.
 */
double claw::tween::tweener::update(double dt)
{
  if(m_impl == NULL)
    return dt;
  else
    return m_impl->update(dt);
}

/**
 * \brief Execute the callbacks notifying about the finish of the tweener.
 */
void claw::tween::tweener::on_finished(finish_callback f)
{
  if(m_impl != NULL)
    m_impl->on_finished(f);
}

/**
 * \brief Swap two tweeners.
 * \param a The first tweener.
 * \param b The second tweener.
 */
template <>
void std::swap(claw::tween::tweener& a, claw::tween::tweener& b)
{
  a.swap(b);
}
