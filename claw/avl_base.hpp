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
 * \file avl_base.hpp
 * \brief Base implementation for the AVL Binary search tree.
 * \author Julien Jorge
 */
#ifndef __CLAW_AVL_BASE_HPP__
#define __CLAW_AVL_BASE_HPP__

#include <iterator>
#include <cstddef>

#include <claw/binary_node.hpp>

namespace claw
{
  //---------------------------------------------------------------------------
  /**
   * \brief Binary search tree base AVL implementation.
   *
   * Each key appears only once. Nodes are sorted as 
   * left_child < node < right_child.
   * \invariant this->empty() <==> (this->size() == 0)
   * \invariant this is an AVL.
   * \remark Type requirements :
   *  - K is LessThanComparable ;
   *  - Comp is a binary predicate such that Comp(K a, K b) == true if a < b.
   * \remark Code is taken from a C implementation, so perhaps it doesn't 
   *         really look nice for C++. Nevertheless it works perfectly and it's
   *         fast conversion : that good things.
   * \author Julien Jorge
   */
  template < class K, class Comp = std::less<K> >
  class avl_base
  {
  private:

    //**************************** avl_node ***********************************

      /**
       * \brief Node of a binary search tree (AVL).
       */
    class avl_node:
      public binary_node< typename claw::avl_base<K,Comp>::avl_node >
    {
    private:
      /** \brief The type of the parent class. */
      typedef binary_node< typename claw::avl_base<K,Comp>::avl_node > super;

    public:
      explicit avl_node( const K& k );
      ~avl_node();

      avl_node* duplicate( unsigned int& count ) const;

      void del_tree();
      unsigned int depth() const;

      avl_node* find( const K& k );
      const avl_node* find( const K& k ) const;

      avl_node* find_nearest_greater( const K& key );
      const avl_node* find_nearest_greater( const K& key ) const;

      avl_node* find_nearest_lower( const K& key );
      const avl_node* find_nearest_lower( const K& key ) const;

      avl_node* lower_bound();
      const avl_node* lower_bound() const;

      avl_node* upper_bound();
      const avl_node* upper_bound() const;

      avl_node* prev();
      const avl_node* prev() const;

      avl_node* next();
      const avl_node* next() const;

    private:
      explicit avl_node( const avl_node& that );

    public:
      /** \brief Node key */
      K key;

      /** 
       *  \brief Balance of the node is -1, 0 or 1. 
       *  Equals to the difference between left child depth and right child 
       *  depth.
       */
      signed char balance;

      /** \brief Father of the node. Null if this node is root */
      avl_node *father;

    }; // class avl_node

  private:
    typedef avl_node* avl_node_ptr;
    typedef avl_node const* const_avl_node_ptr;

  public:
    //*************************** avl::avl_iterator ***************************

      /**
       * \brief AVL iterator.
       */
    class avl_iterator
    {
    public:
      typedef K  value_type;
      typedef K& reference;
      typedef K* const pointer;
      typedef ptrdiff_t difference_type;
          
      typedef std::bidirectional_iterator_tag iterator_category;

    public:
      avl_iterator();
      avl_iterator( avl_node_ptr node, bool final );

      avl_iterator& operator++();
      avl_iterator operator++(int);
      avl_iterator& operator--();
      avl_iterator operator--(int);
      reference operator*() const;
      pointer   operator->() const;
      bool operator==(const avl_iterator& it) const;
      bool operator!=(const avl_iterator& it) const;

    private:
      /** \brief Current node in the tree. */
      avl_node_ptr m_current;

      /** \brief True if we've gone past the last node. */
      bool m_is_final;

    }; // class avl_iterator

      /**
       * \brief AVL iterator.
       */
    class avl_const_iterator
    {
    public:
      typedef K  value_type;
      typedef const K& reference;
      typedef const K* const pointer;
      typedef ptrdiff_t difference_type;
          
      typedef std::bidirectional_iterator_tag iterator_category;

    public:
      avl_const_iterator();
      avl_const_iterator( const_avl_node_ptr node, bool final );

      avl_const_iterator& operator++();
      avl_const_iterator operator++(int);
      avl_const_iterator& operator--();
      avl_const_iterator operator--(int);
      reference operator*() const;
      pointer   operator->() const;
      bool operator==(const avl_const_iterator& it) const;
      bool operator!=(const avl_const_iterator& it) const;

    private:
      /** \brief Current node in the tree. */
      const_avl_node_ptr m_current;

      /** \brief True if we've gone past the last node. */
      bool m_is_final;

    }; // class avl_const_iterator

  public:
    typedef K value_type;
    typedef K key_type;
    typedef K referent_type;
    typedef Comp key_less;
    typedef const K& const_reference;
    typedef avl_iterator iterator;
    typedef avl_const_iterator const_iterator;

  public:
    //**************************** avl_base (main) *****************************

    avl_base();
    explicit avl_base( const avl_base<K, Comp>& that );
    ~avl_base();

    void insert( const K& key );
    template<typename Iterator>
    void insert( Iterator first, Iterator last );

    void erase( const K& key );
    void clear();

    inline unsigned int size() const;
    inline bool empty() const;

    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    iterator find( const K& key );
    const_iterator find( const K& key ) const;

    iterator find_nearest_greater( const K& key );
    const_iterator find_nearest_greater( const K& key ) const;

    iterator find_nearest_lower( const K& key );
    const_iterator find_nearest_lower( const K& key ) const;

    iterator lower_bound();
    const_iterator lower_bound() const;

    iterator upper_bound();
    const_iterator upper_bound() const;

    avl_base<K, Comp>& operator=( const avl_base<K, Comp>& that );
    bool operator==( const avl_base<K, Comp>& that ) const;
    bool operator!=( const avl_base<K, Comp>& that ) const;
    bool operator<( const avl_base<K, Comp>& that ) const;
    bool operator>( const avl_base<K, Comp>& that ) const;
    bool operator<=( const avl_base<K, Comp>& that ) const;
    bool operator>=( const avl_base<K, Comp>& that ) const;

    void swap( avl_base<K, Comp>& that );

  private:
    //-------------------------------------------------------------------------
    // We need some methods to check the validity of our trees

    bool check_in_bounds( const avl_node_ptr node, const K& min, 
                          const K& max ) const;
    bool check_balance( const avl_node_ptr node ) const;
    bool correct_descendant( const avl_node_ptr node ) const;
    bool validity_check() const;

  private:
    iterator make_iterator( avl_node_ptr node ) const;
    const_iterator make_const_iterator( const_avl_node_ptr node ) const;

    //-------------------------------------------------------------------------
    // Tree management methods

    void rotate_right( avl_node_ptr& node );
    void rotate_left( avl_node_ptr& node );
    void rotate_left_right( avl_node_ptr& node );
    void rotate_right_left( avl_node_ptr& node );

    void update_balance( avl_node_ptr node, const K& key );
    void adjust_balance( avl_node_ptr& node );
    void adjust_balance_left( avl_node_ptr& node );
    void adjust_balance_right( avl_node_ptr& node );


    //-------------------------------------------------------------------------
    //    Methods for insertion
    //-------------------------------------------------------------------------


    void insert_node( const K& key );
    avl_node_ptr* find_node_reference( const K& key, 
                                       avl_node_ptr& last_imbalanced, 
                                       avl_node_ptr& node_father);


    //-------------------------------------------------------------------------
    //    Methods for deletion
    //-------------------------------------------------------------------------


    bool recursive_delete( avl_node_ptr& node, const K& key );
    bool new_balance( avl_node_ptr& node, int imbalance );
    bool recursive_delete_node( avl_node_ptr& node );
    int recursive_delete_max( avl_node_ptr& root, avl_node_ptr node );

  public:
    /** \brief Function object used to compare keys. */
    static key_less s_key_less;

  private:
    /** \brief Nodes count. */
    unsigned int m_size;

    /** \brief Nodes. */
    avl_node_ptr m_tree;

  }; // class avl_base
} // namespace claw

#include <claw/impl/avl_base.tpp>

#endif // __CLAW_AVL_HPP__
