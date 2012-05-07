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
 * \file binary_node.hpp
 * \brief Basic binary node.
 * \author Julien Jorge
 */
#ifndef __CLAW_BINARY_NODE_HPP__
#define __CLAW_BINARY_NODE_HPP__

namespace claw
{

  /**
   * \brief Basic binary node.
   * \author Julien Jorge
   */
  template< class U >
  class binary_node
  {
  public :
    /** \brief Left child. */
    U* left;
    /** \brief Right child */
    U* right;

    explicit binary_node();
    explicit binary_node( const binary_node& that );
    ~binary_node();

    void clear();
  }; // binary_node;

} // namespace claw

#include <claw/impl/binary_node.tpp>

#endif // __CLAW_BINARY_NODE_HPP__

