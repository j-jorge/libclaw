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
 * \file avl.hpp
 * \brief AVL Binary search tree.
 * \author Julien Jorge
 */
#ifndef __CLAW_AVL_HPP__
#define __CLAW_AVL_HPP__

#include <claw/avl_base.hpp>

namespace claw
{
  //---------------------------------------------------------------------------
  /**
   * \brief Binary search tree AVL implementation.
   * \author Julien Jorge
   */
  template < class K, class Comp = std::less<K> >
  class avl
  {
  private:
    /** \brief The type of the implementation of this tree. */
    typedef avl_base<K, Comp> impl_type;

  public:
    /** \brief The type of the values in the tree. */
    typedef K value_type;

    /** \brief The type of the keys in the tree. */
    typedef K key_type;

    /** \brief The type passed to the template. */
    typedef K referent_type;

    /** \brief The comparator to use to compare the keys. */
    typedef Comp key_less;

    /** \brief The type of a const reference on the values. */
    typedef const K& const_reference;

    /** \brief The type of the iterator on the values of the tree. */
    typedef typename impl_type::avl_const_iterator const_iterator;

  public:
    avl();
    explicit avl( const avl<K, Comp>& that );
    template<typename InputIterator>
    avl( InputIterator first, InputIterator last );

    void insert( const K& key );
    template<typename InputIterator>
    void insert( InputIterator first, InputIterator last );

    void erase( const K& key );
    void clear();

    unsigned int size() const;
    bool empty() const;

    const_iterator begin() const;
    const_iterator end() const;
    const_iterator find( const K& key ) const;
    const_iterator find_nearest_greater( const K& key ) const;
    const_iterator find_nearest_lower( const K& key ) const;
    const_iterator lower_bound() const;
    const_iterator upper_bound() const;

    avl<K, Comp>& operator=( const avl<K, Comp>& that );
    bool operator==( const avl<K, Comp>& that ) const;
    bool operator!=( const avl<K, Comp>& that ) const;
    bool operator<( const avl<K, Comp>& that ) const;
    bool operator>( const avl<K, Comp>& that ) const;
    bool operator<=( const avl<K, Comp>& that ) const;
    bool operator>=( const avl<K, Comp>& that ) const;

  private:
    /** \brief Implementation. */
    impl_type m_tree;

  }; // class avl
} // namespace claw

#include <claw/impl/avl.tpp>

#endif // __CLAW_AVL_HPP__
