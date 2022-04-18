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
 * \file trie.tpp
 * \brief Implementation of the trie structure.
 * \author Julien Jorge
 */
#include <cassert>
#include <iostream>

//*************************** trie::trie_node *********************************

/**
 * \brief Trie node constructor.
 * \param val Value of the node.
 * \param c Count for the node.
 * \post (value==val) && (count==c)
 */
template <class T, class Comp>
claw::trie<T, Comp>::trie_node::trie_node(const T& val, unsigned int c /*= 0*/)
  : claw::binary_node<typename claw::trie<T, Comp>::trie_node>()
  , value(val)
  , count(0)
{}

/**
 * \brief Trie node copy constructor.
 * \param that Node to copy from.
 */
template <class T, class Comp>
claw::trie<T, Comp>::trie_node::trie_node(const trie_node& that)
  : claw::binary_node<typename claw::trie<T, Comp>::trie_node>(that)
  , value(that.value)
  , count(that.count)
{}

//********************************* trie **************************************

template <class T, class Comp>
typename claw::trie<T, Comp>::value_equal_to
    claw::trie<T, Comp>::s_value_equal_to;

/**
 * \brief Trie constructor.
 * \post empty()
 */
template <class T, class Comp>
claw::trie<T, Comp>::trie()
{
  m_size = 0;
  m_tree = NULL;

  assert(empty());
}

/*
 * \brief Trie copy constructor.
 */
template <class T, class Comp>
claw::trie<T, Comp>::trie(const claw::trie<T, Comp>& that)
{
  if(that.m_tree)
    m_tree = new trie_node(*that.m_tree);
  else
    m_tree = NULL;

  m_size = that.m_size;
}

/**
 * \brief Trie destructor.
 */
template <class T, class Comp>
claw::trie<T, Comp>::~trie()
{
  if(m_tree)
    delete m_tree;
}

/**
 * \brief Gets size (words count) of the structure.
 */
template <class T, class Comp>
unsigned int claw::trie<T, Comp>::size() const
{
  return m_size;
}

/**
 * \brief Tell if the structure is empty or not.
 */
template <class T, class Comp>
bool claw::trie<T, Comp>::empty() const
{
  return m_tree == NULL;
}

/**
 * \brief Clear the trie.
 * \post this->empty() == true
 */
template <class T, class Comp>
void claw::trie<T, Comp>::clear()
{
  if(m_tree)
    {
      delete m_tree;
      m_tree = NULL;
      m_size = 0;
    }
}

/**
 * \brief Add a word to the structure.
 * \remark Type requirements :
 *  - *InputIterator is T.
 * \param first First item of the word.
 * \param last Item just after the last to add.
 * \pre first != last
 * \post !empty() && count(first, last) == old(count(first, last)) + 1
 */
template <class T, class Comp>
template <class InputIterator>
void claw::trie<T, Comp>::insert(InputIterator first, InputIterator last)
{
  assert(first != last);

  trie_node_ptr* p = &m_tree;     // for tree search
  trie_node_ptr last_node = NULL; // last node of the inserted word

  // Try to insert a maximum of items
  while(*p && (first != last))
    if(s_value_equal_to((*p)->value, *first))
      {
        last_node = *p;
        p = &(*p)->right;
        ++first;
      }
    else
      p = &(*p)->left;

  // If we haven't inserted the full word,
  // create the whole subtree.
  while(first != last)
    {
      *p = new trie_node(*first);
      last_node = *p;

      ++first;
      p = &(*p)->right;
    }

  ++(last_node->count);

  // Don't forget to increase words count.
  ++m_size;
}

/**
 * \brief Gets a word count.
 * \remark Type requirements :
 *  - *InputIterator is T.
 * \param first First item of the word.
 * \param last Item just after the last to find.
 * \pre first != last
 */
template <class T, class Comp>
template <class InputIterator>
unsigned int claw::trie<T, Comp>::count(InputIterator first,
                                        InputIterator last)
{
  assert(first != last);

  trie_node_ptr* p = &m_tree;     // for tree search
  trie_node_ptr last_node = NULL; // last node of the word

  // Try to find the word
  while(*p && (first != last))
    if(s_value_equal_to((*p)->value, *first))
      {
        last_node = *p;
        p = &(*p)->right;
        ++first;
      }
    else
      p = &(*p)->left;

  // found ?
  if(first == last)
    return last_node->count;
  else
    return 0;
}
