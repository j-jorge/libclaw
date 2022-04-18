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
 * \file tree.hpp
 * \brief A tree structure with any number of children.
 * \author Julien Jorge
 */
#ifndef __CLAW_TREE_HPP__
#define __CLAW_TREE_HPP__

#include <list>

namespace claw
{
  /**
   * \brief A tree structure with any number of children.
   * \author Julien Jorge
   */
  template <typename T>
  class tree
  {
  public:
    /** \brief The type of the value stored in the nodes. */
    typedef T value_type;

    /** \brief The type of the current class. */
    typedef tree<T> self_type;

  private:
    /** \brief The type of the list in which are stored the children. */
    typedef std::list<tree<T> > child_list;

  public:
    typedef typename child_list::iterator iterator;
    typedef typename child_list::const_iterator const_iterator;

  public:
    tree();
    explicit tree(const T& that);

    bool operator==(const self_type& that) const;

    bool is_leaf() const;

    self_type& add_child(const T& v);
    self_type& add_child(const self_type& v);

    iterator find(const T& v);
    const_iterator find(const T& v) const;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

  public:
    /** \brief The value in this node. */
    T value;

  private:
    /** \brief The children of this node. */
    child_list m_child;

  }; // class tree
}

#include <claw/tree.tpp>

#endif // __CLAW_TREE_HPP__
