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
 * \file trie.hpp
 * \brief A trie structure.
 * \author Julien Jorge
 */
#ifndef __CLAW_TRIE_HPP__
#define __CLAW_TRIE_HPP__

#include <claw/binary_node.hpp>
#include <functional>
#include <list>

namespace claw
{

  /**
   * \brief This class is a trie tree.
   *
   * Trie trees are used for storage and count of linear datas with similar
   * prefixes, typically words.
   * For example, if you insert words
   * - ant
   * - antagonize
   * - antagonism
   * - ant
   * It will use as much memory space as
   * - antagonize
   * - antagonism
   * Nodes for "ant" are shared between words, likewise for "antagoni" for the
   * two last word, and a counter is set for each word. So "ant" will have a
   * count of 2.
   * \remark Type requirements :
   *  - T is EquallyComparable ;
   *  - Comp is a binary predicate such that Comp(T a,T b) == true if a == b.
   * \invariant empty <=> (size()==0)
   * \author Julien Jorge
   */
  template <class T, class Comp = std::equal_to<T> >
  class trie
  {
  private:
    //************************ trie::trie_node ********************************

    /**
     * \brief Node of our trie.
     * Left subtree will be other suggestions for the current position,
     * right subtree will be following items for the word seen from the root
     * to here.
     */
    struct trie_node : public binary_node<trie_node>
    {
      /** \brief Value of the node */
      T value;
      /**
       * \brief Times we found the word from the root to this node.
       * Zero if never seen
       */
      unsigned int count;

      trie_node(const T& val, unsigned int c = 0);
      trie_node(const trie_node& that);

    }; // trie_node;

  public:
    //****************************** trie *************************************

    typedef const T value_type;
    typedef Comp value_equal_to;

  private:
    typedef trie_node* trie_node_ptr;

  public:
    trie();
    trie(const trie<T, Comp>& that);
    ~trie();

    unsigned int size() const;
    bool empty() const;

    void clear();

    template <class InputIterator>
    void insert(InputIterator first, InputIterator last);

    template <class InputIterator>
    unsigned int count(InputIterator first, InputIterator last);

  private:
    /** \brief Function object use to check if nodes have the same value. */
    static value_equal_to s_value_equal_to;

    /** \brief Main structure*/
    trie_node_ptr m_tree;

    /** \brief Words count */
    unsigned int m_size;
  }; // class trie

}

#include <claw/trie.tpp>

#endif // __CLAW_TRIE_HPP__
