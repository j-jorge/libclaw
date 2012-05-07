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
 * \file binary_node.tpp
 * \brief The binary_node class implementation.
 * \author Julien Jorge
 */

#include <cstddef>

/*---------------------------------------------------------------------------*/
/**
 * \brief Binary node constructor.
 * \post (left==NULL) && (right==NULL)
 */
template< class U >
claw::binary_node<U>::binary_node() 
  : left(NULL), right(NULL)
{

} //binary_node()

/*---------------------------------------------------------------------------*/
/**
 * \brief Binary node copy constructor.
 * \param that Node to copy from.
 */
template< class U >
claw::binary_node<U>::binary_node( const binary_node& that )
{
  if (that.left)
    left = new U( * that.left );
  else
    left = NULL;

  if ( that.right )
    right = new U( * that.right );
  else
    right = NULL;
} // binary_node() [copy constructor]

/*---------------------------------------------------------------------------*/
/**
 * \brief Clear right and left childs. Memory's not freed.
 * \post this->left == NULL && this->right == NULL
 */
template< class U >
void claw::binary_node<U>::clear()
{
  left = NULL;
  right = NULL;
} // clear()

/*---------------------------------------------------------------------------*/
/**
 * \brief Binary node destructor.
 * Deletes the whole subtree. 
 */
template< class U >
claw::binary_node<U>::~binary_node()
{
  if (left)
    delete left;

  if (right)
    delete right;
} // ~binary_node() [destructor]
