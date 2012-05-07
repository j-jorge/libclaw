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
 * \file tree.tpp
 * \brief Implementation of the claw::tree class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
template<typename T>
claw::tree<T>::tree()
  : value()
{

} // tree::tree()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor with initialization.
 * \param v The value of the root node.
 */
template<typename T>
claw::tree<T>::tree( const T& v )
  : value(v)
{

} // tree::tree()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality operator.
 * \param that The tree to compare to.
 */
template<typename T>
bool claw::tree<T>::operator==( const self_type& that ) const
{
  bool result = ( value == that.value );

  if (result)
    {
      typename child_list::const_iterator it_me = m_child.begin();
      typename child_list::const_iterator it_him = that.m_child.begin();
      typename child_list::const_iterator eit_me = m_child.end();
      typename child_list::const_iterator eit_him = that.m_child.end();
  
      while ( result && (it_me!=eit_me) && (it_him!=eit_him) )
        result = (*it_me == *it_him);

      if ( (it_me!=eit_me) || (it_him!=eit_him) )
        result = false;
    }

  return result;
} // tree::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this node is a leaf (ie. it has no child).
 */
template<typename T>
bool claw::tree<T>::is_leaf() const
{
  return m_child.empty();
} // tree::is_leaf()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a child to this node.
 * \param v The value of the child to add.
 */
template<typename T>
typename claw::tree<T>::self_type& claw::tree<T>::add_child( const T& v )
{
  m_child.push_back( self_type(v) );
  return m_child.back();
} // tree::add_child()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a child subtree to this node.
 * \param v The tree to add.
 */
template<typename T>
typename claw::tree<T>::self_type&
claw::tree<T>::add_child( const self_type& v )
{
  m_child.push_back( v );
} // tree::add_child()

/*----------------------------------------------------------------------------*/
/**
 * \brief Search the first child having a given value.
 * \param v The value of the child to find.
 */
template<typename T>
typename claw::tree<T>::iterator claw::tree<T>::find( const T& v )
{
  typename child_list::iterator it;
  bool found = false;

  for ( it=m_child.begin(); !found && (it!=end()) ; )
    if ( it->value == v )
      found = true;
    else
      ++it;

  return iterator( it );
} // tree::find()

/*----------------------------------------------------------------------------*/
/**
 * \brief Search the first child having a given value.
 * \param v The value of the child to find.
 */
template<typename T>
typename claw::tree<T>::const_iterator claw::tree<T>::find( const T& v ) const
{
  typename child_list::const_iterator it;
  bool found = false;

  for ( it=m_child.begin(); !found && (it!=end()) ; )
    if ( it->value == v )
      found = true;
    else
      ++it;

  return const_iterator( it );
} // tree::find()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the begining of the children.
 */
template<typename T>
typename claw::tree<T>::iterator claw::tree<T>::begin()
{
  return iterator( m_child.begin() );
} // tree::begin()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator just past the end of the children.
 */
template<typename T>
typename claw::tree<T>::iterator claw::tree<T>::end()
{
  return iterator( m_child.end() );
} // tree::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a constant iterator on the begining of the children.
 */
template<typename T>
typename claw::tree<T>::const_iterator claw::tree<T>::begin() const
{
  return const_iterator( m_child.begin() );
} // tree::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a constant iterator just past the end of the children.
 */
template<typename T>
typename claw::tree<T>::const_iterator claw::tree<T>::end() const
{
  return const_iterator( m_child.end() );
} // tree::end()
