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
 * \file basic_singleton.tpp
 * \brief Implementation of the claw::pattern::basic_singleton class.
 * \author Julien Jorge
 */

/**
 * \brief Get the instance of the singleton.
 */
template <class T>
T& claw::pattern::basic_singleton<T>::get_instance()
{
  static T single_instance;

  return single_instance;
}

/**
 * \brief Constructor.
 */
template <class T>
claw::pattern::basic_singleton<T>::basic_singleton()
{}

/**
 * \brief Destructor.
 */
template <class T>
claw::pattern::basic_singleton<T>::~basic_singleton()
{}
