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
 * \file avl.tpp
 * \brief The avl class implementation.
 * \author Julien Jorge
 */
#include <cassert>

/**
 * \brief AVL constructor.
 * \post empty()
 */
template <class K, class Comp>
claw::avl<K, Comp>::avl()
{}

/**
 * \brief AVL copy constructor.
 * \param that AVL instance to copy from.
 */
template <class K, class Comp>
claw::avl<K, Comp>::avl(const avl<K, Comp>& that)
  : m_tree(that.m_tree)
{}

/**
 * \brief Constructor from a range.
 * \param first Iterator on the first element of the range.
 * \param last Iterator just past the last element of the range.
 */
template <class K, class Comp>
template <typename InputIterator>
claw::avl<K, Comp>::avl(InputIterator first, InputIterator last)
{
  m_tree.insert(first, last);
}

/**
 * \brief Add a value in a tree.
 * \param key Node key.
 * \post exists(key)
 */
template <class K, class Comp>
void claw::avl<K, Comp>::insert(const K& key)
{
  m_tree.insert(key);
}

/**
 * \brief Add a range of items in the tree.
 * \param first Iterator on the first item to add.
 * \param last Iterator past the last item to add.
 * \pre Iterator::value_type is K
 * \post exists( *it ) for all it in [first, last)
 */
template <class K, class Comp>
template <typename InputIterator>
void claw::avl<K, Comp>::insert(InputIterator first, InputIterator last)
{
  m_tree.insert(first, last);
}

/**
 * \brief Delete a value in a tree.
 * \param key Node key.
 * \post not exists(key)
 */
template <class K, class Comp>
void claw::avl<K, Comp>::erase(const K& key)
{
  m_tree.erase(key);
}

/**
 * \brief Clear a tree.
 * \post empty()
 */
template <class K, class Comp>
void claw::avl<K, Comp>::clear()
{
  m_tree.clear();
}

/**
 * \brief Get the size of a tree.
 * \return The size of the tree.
 */
template <class K, class Comp>
inline unsigned int claw::avl<K, Comp>::size() const
{
  return m_tree.size();
}

/**
 * \brief Tell if a tree is empty or not.
 * \return true if the tree is empty, false otherwise.
 */
template <class K, class Comp>
inline bool claw::avl<K, Comp>::empty() const
{
  return m_tree.empty();
}

/**
 * \brief Get an iterator on the nodes of the tree.
 */
template <class K, class Comp>
typename claw::avl<K, Comp>::const_iterator claw::avl<K, Comp>::begin() const
{
  return m_tree.begin();
}

/**
 * \brief Get an iterator after the end of the tree.
 */
template <class K, class Comp>
typename claw::avl<K, Comp>::const_iterator claw::avl<K, Comp>::end() const
{
  return m_tree.end();
}

/**
 * \brief Get an iterator on the nodes of the tree from a specified key.
 * \param key Key to find.
 */
template <class K, class Comp>
typename claw::avl<K, Comp>::const_iterator
claw::avl<K, Comp>::find(const K& key) const
{
  return m_tree.find(key);
}

/**
 * \brief Get an iterator on the nodes of the tree on the key imediatly after
 *        from a specified key.
 * \param key Key to find.
 */
template <class K, class Comp>
typename claw::avl<K, Comp>::const_iterator
claw::avl<K, Comp>::find_nearest_greater(const K& key) const
{
  return m_tree.find_nearest_greater(key);
}

/**
 * \brief Get an iterator on the nodes of the tree on the key imediatly before
 *        from a specified key.
 * \param key Key to find.
 */
template <class K, class Comp>
typename claw::avl<K, Comp>::const_iterator
claw::avl<K, Comp>::find_nearest_lower(const K& key) const
{
  return m_tree.find_nearest_lower(key);
}

/**
 * \brief Get an iterator on the lowest value of the tree.
 */
template <class K, class Comp>
typename claw::avl<K, Comp>::const_iterator
claw::avl<K, Comp>::lower_bound() const
{
  return m_tree.lower_bound();
}

/**
 * \brief Get an iterator on the gratest value of the tree.
 */
template <class K, class Comp>
typename claw::avl<K, Comp>::const_iterator
claw::avl<K, Comp>::upper_bound() const
{
  return m_tree.upper_bound();
}

/**
 * \brief Assignment.
 * \param that The instance to copy from.
 */
template <class K, class Comp>
claw::avl<K, Comp>& claw::avl<K, Comp>::operator=(const avl<K, Comp>& that)
{
  m_tree = that.m_tree;
  return *this;
}

/**
 * \brief Equality.
 * \param that The instance to compare to.
 */
template <class K, class Comp>
bool claw::avl<K, Comp>::operator==(const avl<K, Comp>& that) const
{
  return m_tree == that.m_tree;
}

/**
 * \brief Disequality.
 * \param that The instance to compare to.
 */
template <class K, class Comp>
bool claw::avl<K, Comp>::operator!=(const avl<K, Comp>& that) const
{
  return m_tree != that.m_tree;
}

/**
 * \brief Less than operator.
 * \param that The instance to compare to.
 */
template <class K, class Comp>
bool claw::avl<K, Comp>::operator<(const avl<K, Comp>& that) const
{
  return m_tree < that.m_tree;
}

/**
 * \brief Greater than operator.
 * \param that The instance to compare to.
 */
template <class K, class Comp>
bool claw::avl<K, Comp>::operator>(const avl<K, Comp>& that) const
{
  return m_tree > that.m_tree;
}

/**
 * \brief Less or equal operator.
 * \param that The instance to compare to.
 */
template <class K, class Comp>
bool claw::avl<K, Comp>::operator<=(const avl<K, Comp>& that) const
{
  return m_tree <= that.m_tree;
}

/**
 * \brief Greater or equal operator.
 * \param that The instance to compare to.
 */
template <class K, class Comp>
bool claw::avl<K, Comp>::operator>=(const avl<K, Comp>& that) const
{
  return m_tree >= that.m_tree;
}
