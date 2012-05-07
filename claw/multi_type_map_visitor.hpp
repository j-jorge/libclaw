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
 * \file multi_type_map_visitor.hpp
 * \brief This class goes through all entries in a multi_type_map and apply a
 *        function to them.
 * \author Julien Jorge
 */
#ifndef __CLAW_MULTI_TYPE_MAP_VISITOR_HPP__
#define __CLAW_MULTI_TYPE_MAP_VISITOR_HPP__

#include "claw/multi_type_map.hpp"

namespace claw
{
  /**
   * \brief This class goes through all entries in a multi_type_map and apply
   *        a function to them.
   * \author Julien Jorge
   */
  class multi_type_map_visitor
  {
  public:
    template<typename Key, typename TypeList, typename Function>
    void run( multi_type_map<Key, TypeList>& m, Function f ) const;

  }; // class multi_type_map_visitor

} // namespace claw

#include "claw/impl/multi_type_map_visitor.tpp"

#endif // __CLAW_MULTI_TYPE_MAP_VISITOR_HPP__
