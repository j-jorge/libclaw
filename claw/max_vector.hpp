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
 * \file max_vector.hpp
 * \brief Fill a container with some values, keeping only all the "maximum"
 *        inserted values.
 * \author Julien Jorge
 */
#ifndef __CLAW_MAX_VECTOR_HPP__
#define __CLAW_MAX_VECTOR_HPP__

#include <vector>
#include <functional>

namespace claw
{
  /**
   * \brief Fill a container with some values, keeping only all the "maximum"
   *        inserted values.
   *
   * Adding a value greater than the previous ones remove all the old values.
   *
   * \b Template \b parameters:
   * - \param E: the type of the stored values.
   * - \param Comp: how to compare the old values with the new ones.
   * - \param Container: the type of the container in which the values are
   *   stored.
   *
   * \author Julien Jorge
   */
  template< typename E, typename Comp = std::less<E>,
	    typename Container = std::vector<E> >
  class max_vector
  {
  public:
    /** \brief The type of the values stored in the container. */
    typedef E value_type;

    /** \brief The comparator used to compare the values inserted. */
    typedef Comp comparator_type;

    /** \brief The type of the container in which the values are stored. */
    typedef Container container_type;

  public:
    max_vector();
    explicit max_vector(const value_type& e);

    void add(const value_type& e);
    const container_type& get_v() const;

  private:
    /** \brief The maximum values. */
    container_type m_values;

  }; // max_vector

} // namespace claw

#include <claw/impl/max_vector.tpp>

#endif // __CLAW_MAX_VECTOR_HPP__
