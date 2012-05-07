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
 * \file avl.tpp
 * \brief The avl class implementation.
 * \author Julien Jorge
 */
#include <cassert>

//**************************** avl_base::avl_node ******************************

/*----------------------------------------------------------------------------*/
/**
 * \brief AVL's node constructor
 * \param k Value of the node
 */
template<class K, class Comp>
claw::avl_base<K, Comp>::avl_node::avl_node( const K& k ) 
  : super(), key(k), balance(0), father(NULL) 
{
  assert(!super::left);
  assert(!super::right);
} // avl_node::avl_node() [constructeur]

/*----------------------------------------------------------------------------*/
/** 
 * \brief AVL's node destructor 
 */
template<class K, class Comp>
claw::avl_base<K, Comp>::avl_node::~avl_node() 
{

} // avl_node::~avl_node() [destructeur]

/*----------------------------------------------------------------------------*/
/**
 * \brief Duplicate node and his subtrees.
 * \param count (out) Count of duplicated nodes.
 * \remark Count isn't initialized. You should call duplicate with count = 0.
 */
template<class K, class Comp>
typename claw::avl_base<K, Comp>::avl_node*
claw::avl_base<K, Comp>::avl_node::duplicate( unsigned int& count ) const
{
  avl_node* node_copy = new avl_node(key);
  ++count;
  node_copy->balance = balance;
  node_copy->father = NULL;

  if (super::left) 
    {
      node_copy->left = super::left->duplicate(count);
      node_copy->left->father = node_copy;
    }
  else
    node_copy->left = NULL;
  
  if (super::right) 
    {
      node_copy->right = super::right->duplicate(count);
      node_copy->right->father = node_copy;
    }
  else
    node_copy->right = NULL;

  return node_copy;
} // avl_node::duplicate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Delete current node and his subtrees.
 * \post left == NULL && right == NULL
 */
template<class K, class Comp>
void claw::avl_base<K, Comp>::avl_node::del_tree()
{
  if (super::left) 
    {
      delete super::left;
      super::left = NULL;
    }
  if (super::right)
    {
      delete super::right;
      super::right = NULL;
    }
  assert( !super::left );
  assert( !super::right );
} // avl_node::del_tree

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the depth of a tree.
 * \remark For validity check.
 * \return 1 + max( this->left->depth(), this->right->depth() )
 */
template<class K, class Comp>
unsigned int claw::avl_base<K, Comp>::avl_node::depth() const
{
  unsigned int pl=0, pr=0;

  if (super::left)  pl = super::left->depth();
  if (super::right) pr = super::right->depth();

  if (pl > pr)
    return 1 + pl;
  else
    return 1 + pr;
} // avl_node::depth()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a pointer on the node of the tree with a specified key.
 * \param key Key to find.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_node*
claw::avl_base<K,Comp>::avl_node::find( const K& key )
{
  bool ok = false;
  avl_node* node = this;

  while (node && !ok)
    if ( avl_base<K, Comp>::s_key_less(key, node->key) )
      node = node->left;
    else if ( avl_base<K, Comp>::s_key_less(node->key, key) )
      node = node->right;
    else
      ok = true;

  return node;
} // avl_base::avl_node::find()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a pointer on the node of the tree with a specified key.
 * \param key Key to find.
 */
template<class K, class Comp>
const typename claw::avl_base<K,Comp>::avl_node*
claw::avl_base<K,Comp>::avl_node::find( const K& key ) const
{
  bool ok = false;
  const avl_node* node = this;

  while (node && !ok)
    if ( avl_base<K, Comp>::s_key_less(key, node->key) )
      node = node->left;
    else if ( avl_base<K, Comp>::s_key_less(node->key, key) )
      node = node->right;
    else
      ok = true;

  return node;
} // avl_base::avl_node::find()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the nodes of the tree on the key imediatly after
 *        from a specified key.
 * \param key Key to find.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_node*
claw::avl_base<K,Comp>::avl_node::find_nearest_greater( const K& key )
{
  bool ok = false;
  avl_node* node = this;
  avl_node* prev_node = NULL;

  while (node && !ok)
    if ( avl_base<K, Comp>::s_key_less(key, node->key) )
      {
        prev_node = node;
        node = node->left;
      }
    else if ( avl_base<K, Comp>::s_key_less(node->key, key) )
      {
        prev_node = node;
        node = node->right;
      }
    else
      ok = true;

  if (ok)
    return node->next();
  else if (prev_node)
    {
      if ( avl_base<K, Comp>::s_key_less(key, prev_node->key) )
        return prev_node->next();
      else
        return prev_node;
    }
  else
    return node;
} // avl_base::find_nearest_greater()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the nodes of the tree on the key imediatly after
 *        from a specified key.
 * \param key Key to find.
 */
template<class K, class Comp>
const typename claw::avl_base<K,Comp>::avl_node*
claw::avl_base<K,Comp>::avl_node::find_nearest_greater( const K& key ) const
{
  bool ok = false;
  const avl_node* node = this;
  const avl_node* prev_node = NULL;

  while (node && !ok)
    if ( avl_base<K, Comp>::s_key_less(key, node->key) )
      {
        prev_node = node;
        node = node->left;
      }
    else if ( avl_base<K, Comp>::s_key_less(node->key, key) )
      {
        prev_node = node;
        node = node->right;
      }
    else
      ok = true;

  if (ok)
    return node->next();
  else if (prev_node)
    {
      if ( avl_base<K, Comp>::s_key_less(key, prev_node->key) )
        return prev_node->next();
      else
        return prev_node;
    }
  else
    return node;
} // avl_base::find_nearest_greater()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the nodes of the tree on the key imediatly before
 *        from a specified key.
 * \param key Key to find.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_node*
claw::avl_base<K,Comp>::avl_node::find_nearest_lower( const K& key )
{
  bool ok = false;
  avl_node* node = this;
  avl_node* prev_node = NULL;

  while (node && !ok)
    if ( s_key_less(key, node->key) )
      {
        prev_node = node;
        node = node->left;
      }
    else if ( s_key_less(node->key, key) )
      {
        prev_node = node;
        node = node->right;
      }
    else
      ok = true;

  if (ok)
    return node->prev();
  else if (prev_node)
    {
      if ( s_key_less(key, prev_node->key) )
        return prev_node;
      else
        return prev_node->prev();
    }
  else
    return node;
} // avl_base::find_nearest_lower()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the nodes of the tree on the key imediatly before
 *        from a specified key.
 * \param key Key to find.
 */
template<class K, class Comp>
const typename claw::avl_base<K,Comp>::avl_node*
claw::avl_base<K,Comp>::avl_node::find_nearest_lower( const K& key ) const
{
  bool ok = false;
  const avl_node* node = this;
  const avl_node* prev_node = NULL;

  while (node && !ok)
    if ( s_key_less(key, node->key) )
      {
        prev_node = node;
        node = node->left;
      }
    else if ( s_key_less(node->key, key) )
      {
        prev_node = node;
        node = node->right;
      }
    else
      ok = true;

  if (ok)
    return node->prev();
  else if (prev_node)
    {
      if ( s_key_less(key, prev_node->key) )
        return prev_node;
      else
        return prev_node->prev();
    }
  else
    return node;
} // avl_base::find_nearest_lower()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a pointer on the lowest value of the tree.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_node*
claw::avl_base<K,Comp>::avl_node::lower_bound()
{
  avl_node* node = this;

  if (node)
    while (node->left)
      node = node->left;

  return node;
} // avl_base::lower_bound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a pointer on the lowest value of the tree.
 */
template<class K, class Comp>
const typename claw::avl_base<K,Comp>::avl_node*
claw::avl_base<K,Comp>::avl_node::lower_bound() const
{
  const avl_node* node = this;

  if (node)
    while (node->left)
      node = node->left;

  return node;
} // avl_base::lower_bound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a pointer on the greatest value of the tree.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_node*
claw::avl_base<K,Comp>::avl_node::upper_bound()
{
  avl_node* node = this;

  if (node)
    while (node->right)
      node = node->right;

  return node;
} // avl_base::upper_bound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a pointer on the greatest value of the tree.
 */
template<class K, class Comp>
const typename claw::avl_base<K,Comp>::avl_node*
claw::avl_base<K,Comp>::avl_node::upper_bound() const
{
  const avl_node* node = this;

  if (node)
    while (node->right)
      node = node->right;

  return node;
} // avl_base::upper_bound()

/*----------------------------------------------------------------------------*/
/** 
 * \brief Get the node immediately greater than \a this.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_node*
claw::avl_base<K,Comp>::avl_node::next()
{
  avl_node* result = this;

  // node have a right subtree : go to the min
  if (result->right != NULL)
    {
      result = result->right;
  
      while (result->left != NULL)
        result = result->left;
    }
  else
    {
      bool done = false;
      avl_node* previous_node = this;

      // get parent node
      while (result->father && !done)
        {
          if (result->father->left == result)
            done = true;

          result = result->father;
        }

      // came back from the max node to the root
      if (!done)
        result = previous_node;
    }
                        
  return result;
} // avl_iterator::avl_node::next()

/*----------------------------------------------------------------------------*/
/** 
 * \brief Get the node immediately greater than \a this.
 */
template<class K, class Comp>
const typename claw::avl_base<K,Comp>::avl_node*
claw::avl_base<K,Comp>::avl_node::next() const
{
  const avl_node* result = this;

  // node have a right subtree : go to the min
  if (result->right != NULL)
    {
      result = result->right;
  
      while (result->left != NULL)
        result = result->left;
    }
  else
    {
      bool done = false;
      const avl_node* previous_node = this;

      // get parent node
      while (result->father && !done)
        {
          if (result->father->left == result)
            done = true;

          result = result->father;
        }

      // came back from the max node to the root
      if (!done)
        result = previous_node;
    }
                        
  return result;
} // avl_iterator::avl_node::next()

/*----------------------------------------------------------------------------*/
/** 
 * \brief Get the node immediately before \a this.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_node*
claw::avl_base<K,Comp>::avl_node::prev()
{
  avl_node* result = this;

  // node have a left subtree : go to the max
  if (result->left != NULL)
    {
      result = result->left;
                
      while (result->right != NULL)
        result = result->right;
    }
  else
    {
      bool done = false;

      // get parent node
      while (result->father && !done)
        {
          if (result->father->right == result)
            done = true;

          result = result->father;
        }
    }
  
  return result;
} // avl_iterator::avl_node::prev()

/*----------------------------------------------------------------------------*/
/** 
 * \brief Get the node immediately before \a this.
 */
template<class K, class Comp>
const typename claw::avl_base<K,Comp>::avl_node*
claw::avl_base<K,Comp>::avl_node::prev() const
{
  const avl_node* result = this;

  // node have a left subtree : go to the max
  if (result->left != NULL)
    {
      result = result->left;
                
      while (result->right != NULL)
        result = result->right;
    }
  else
    {
      bool done = false;

      // get parent node
      while (result->father && !done)
        {
          if (result->father->right == result)
            done = true;

          result = result->father;
        }
    }
  
  return result;
} // avl_iterator::avl_node::prev()





/*=================================== private ===============================*/



/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that Node to copy from.
 * \remark Shouldn't be use.
 */
template<class K, class Comp>
claw::avl_base<K, Comp>::avl_node::avl_node( const avl_node& that )
  : super(that), key(that.key), balance(that.balance), father(NULL) 
{
  assert(0);
} // avl_node::depth()





//**************************** avl_base::avl_iterator **************************



/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<class K, class Comp>
claw::avl_base<K,Comp>::avl_iterator::avl_iterator()
  : m_current(NULL), m_is_final(true)
{

} // avl_iterator::avl_iterator() [constructeur]

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<class K, class Comp>
claw::avl_base<K,Comp>::avl_iterator::avl_iterator
( avl_node_ptr node, bool final )
  : m_current(node), m_is_final(final)
{

} // avl_iterator::avl_iterator() [constructeur with node]

/*----------------------------------------------------------------------------*/
/** 
 * \brief Preincrement.
 * \pre not final(this).
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_iterator&
claw::avl_base<K,Comp>::avl_iterator::operator++()
{
  assert(!m_is_final);
  assert(m_current);

  avl_node* p = m_current->next();

  if ( m_current == p )
    m_is_final = true;
  else
    m_current = p;

  return *this;
} // avl_iterator::operator++() [preincrement]

/*----------------------------------------------------------------------------*/
/** 
 * \brief Postincrement.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_iterator
claw::avl_base<K,Comp>::avl_iterator::operator++(int)
{
  avl_iterator it = *this;
  ++(*this);
  return it;
} // avl_iterator::operator++ [postincrement]

/*----------------------------------------------------------------------------*/
/** 
 * \brief Predecrement.
 * \pre iterator is not at the begining of the container.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_iterator&
claw::avl_base<K,Comp>::avl_iterator::operator--()
{
  assert(m_current);

  if (m_is_final)
    m_is_final = !m_is_final;
  else
    m_current = m_current->prev();

  assert(m_current != NULL);
  
  return *this;
} // avl_iterator::operator--() [predecrement]

/*----------------------------------------------------------------------------*/
/** 
 * \brief Postdecrement.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_iterator
claw::avl_base<K,Comp>::avl_iterator::operator--(int)
{
  avl_iterator it = *this;
  --(*this);
  return it;
} // avl_iterator::operator-- [postdecrement]

/*----------------------------------------------------------------------------*/
/** 
 * \brief Dereference.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_iterator::reference
claw::avl_base<K,Comp>::avl_iterator::operator*() const 
{
  return m_current->key; 
} // avl_iterator::operator*() [dereference]

/*----------------------------------------------------------------------------*/
/** 
 * \brief Reference.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_iterator::pointer
claw::avl_base<K,Comp>::avl_iterator::operator->() const 
{
  return &m_current->key; 
} // avl_iterator::operator->()

/*----------------------------------------------------------------------------*/
/** 
 * \brief Equality.
 * \param it Iterator to compare to.
 */
template<class K, class Comp>
bool
claw::avl_base<K,Comp>::avl_iterator::operator==(const avl_iterator& it) const 
{
  return (m_current == it.m_current) && (m_is_final == it.m_is_final); 
} // avl_iterator::operator==()

/*----------------------------------------------------------------------------*/
/** 
 * \brief Difference.
 * \param it Iterator to compare to.
 */
template<class K, class Comp>
bool
claw::avl_base<K,Comp>::avl_iterator::operator!=(const avl_iterator& it) const 
{
  return !( *this == it ); 
} // avl_iterator::operator!=()





//************************* avl_base::avl_const_iterator ***********************



/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<class K, class Comp>
claw::avl_base<K,Comp>::avl_const_iterator::avl_const_iterator()
  : m_current(NULL), m_is_final(true)
{

} // avl_const_iterator::avl_const_iterator() [constructeur]

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<class K, class Comp>
claw::avl_base<K,Comp>::avl_const_iterator::avl_const_iterator
( const_avl_node_ptr node, bool final )
  : m_current(node), m_is_final(final)
{

} // avl_const_iterator::avl_const_iterator() [constructeur with node]

/*----------------------------------------------------------------------------*/
/** 
 * \brief Preincrement.
 * \pre not final(this).
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_const_iterator&
claw::avl_base<K,Comp>::avl_const_iterator::operator++()
{
  assert(!m_is_final);
  assert(m_current);

  const_avl_node_ptr p = m_current->next();

  if ( m_current == p )
    m_is_final = true;
  else
    m_current = p;

  return *this;
} // avl_const_iterator::operator++() [preincrement]

/*----------------------------------------------------------------------------*/
/** 
 * \brief Postincrement.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_const_iterator
claw::avl_base<K,Comp>::avl_const_iterator::operator++(int)
{
  avl_const_iterator it = *this;
  ++(*this);
  return it;
} // avl_const_iterator::operator++ [postincrement]

/*----------------------------------------------------------------------------*/
/** 
 * \brief Predecrement.
 * \pre iterator is not at the begining of the container.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_const_iterator&
claw::avl_base<K,Comp>::avl_const_iterator::operator--()
{
  assert(m_current);

  if (m_is_final)
    m_is_final = !m_is_final;
  else
    m_current = m_current->prev();

  assert(m_current != NULL);
  
  return *this;
} // avl_const_iterator::operator--() [predecrement]

/*----------------------------------------------------------------------------*/
/** 
 * \brief Postdecrement.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_const_iterator
claw::avl_base<K,Comp>::avl_const_iterator::operator--(int)
{
  avl_const_iterator it = *this;
  --(*this);
  return it;
} // avl_const_iterator::operator-- [postdecrement]

/*----------------------------------------------------------------------------*/
/** 
 * \brief Dereference.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_const_iterator::reference
claw::avl_base<K,Comp>::avl_const_iterator::operator*() const 
{
  return m_current->key; 
} // avl_const_iterator::operator*() [dereference]

/*----------------------------------------------------------------------------*/
/** 
 * \brief Reference.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_const_iterator::pointer
claw::avl_base<K,Comp>::avl_const_iterator::operator->() const 
{
  return &m_current->key; 
} // avl_const_iterator::operator->()

/*----------------------------------------------------------------------------*/
/** 
 * \brief Equality.
 * \param it Iterator to compare to.
 */
template<class K, class Comp>
bool claw::avl_base<K,Comp>::avl_const_iterator::operator==
(const avl_const_iterator& it) const 
{
  return (m_current == it.m_current) && (m_is_final == it.m_is_final); 
} // avl_const_iterator::operator==()

/*----------------------------------------------------------------------------*/
/** 
 * \brief Difference.
 * \param it Iterator to compare to.
 */
template<class K, class Comp>
bool claw::avl_base<K,Comp>::avl_const_iterator::operator!=
(const avl_const_iterator& it) const 
{
  return !( *this == it ); 
} // avl_const_iterator::operator!=()





//******************************* avl_base (main) ******************************


/*----------------------------------------------------------------------------*/
template<class K, class Comp>
typename claw::avl_base<K,Comp>::key_less claw::avl_base<K,Comp>::s_key_less;

/*----------------------------------------------------------------------------*/
/**
 * \brief AVL constructor.
 * \post empty()
 */
template<class K, class Comp>
claw::avl_base<K,Comp>::avl_base()
  : m_size(0), m_tree(NULL) 
{

} // avl_base::avl_base() [constructeur]

/*----------------------------------------------------------------------------*/
/**
 * \brief AVL copy constructor.
 * \param that AVL instance to copy from.
 */
template<class K, class Comp>
claw::avl_base<K,Comp>::avl_base( const avl_base<K, Comp>& that )
{
  m_size = 0;

  if (that.m_tree)
    m_tree = that.m_tree->duplicate( m_size );
  else
    m_tree = NULL;
} // avl_base::avl_base() [copy constructor]

/*----------------------------------------------------------------------------*/
/**
 * \brief AVL destructor.
 */
template<class K, class Comp>
claw::avl_base<K,Comp>::~avl_base()
{
  if (m_tree)
    {
      m_tree->del_tree();
      delete m_tree;
    }
} // avl_base::~avl_base() [destructeur]

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a value in a tree.
 * \param key Node key.
 * \post exists(key)
 */
template<class K, class Comp>
void claw::avl_base<K,Comp>::insert( const K& key )
{
  assert( validity_check() );

  if ( m_tree == NULL )
    {
      m_tree = new avl_node(key);
      m_size = 1;
    }
  else
    insert_node(key);

  assert( validity_check() );
} // avl_base::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a range of items in the tree.
 * \param first Iterator on the first item to add.
 * \param last Iterator past the last item to add.
 * \pre Iterator::value_type is K
 * \post exists( *it ) for all it in [first, last)
 */
template<class K, class Comp>
template<typename Iterator>
void claw::avl_base<K,Comp>::insert( Iterator first, Iterator last )
{
  for ( ; first!=last; ++first )
    insert( *first );
} // avl_base::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Delete a value in a tree.
 * \param key Node key.
 * \post not exists(key)
 */
template<class K, class Comp>
void claw::avl_base<K,Comp>::erase( const K& key )
{
  assert( validity_check() );

  if (m_tree != NULL)
    recursive_delete( m_tree, key );

  assert( validity_check() );
} // avl_base::erase()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear a tree.
 * \post empty()
 */
template<class K, class Comp>
void claw::avl_base<K,Comp>::clear()
{
  if (m_tree != NULL)
    {
      m_tree->del_tree();
      delete m_tree;
                        
      m_tree = NULL;
      m_size = 0;
    }
} // avl_base::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of a tree.
 * \return The size of the tree.
 */
template<class K, class Comp>
inline unsigned int claw::avl_base<K,Comp>::size() const
{
  return m_size; 
} // avl_base::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a tree is empty or not.
 * \return true if the tree is empty, false otherwise.
 */
template<class K, class Comp>
inline bool claw::avl_base<K,Comp>::empty() const
{
  return m_size == 0; 
} // avl_base::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the nodes of the tree.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::iterator claw::avl_base<K,Comp>::begin()
{
  if (m_tree == NULL)
    return iterator(NULL, true);
  else
    return lower_bound();
} // avl_base::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the nodes of the tree.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::const_iterator
claw::avl_base<K,Comp>::begin() const
{
  if (m_tree == NULL)
    return const_iterator(NULL, true);
  else
    return lower_bound();
} // avl_base::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator after the end of the tree.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::iterator claw::avl_base<K,Comp>::end()
{
  if (m_tree == NULL)
    return iterator(NULL, true);
  else
    return iterator(m_tree->upper_bound(), true);
} // avl_base::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator after the end of the tree.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::const_iterator
claw::avl_base<K,Comp>::end() const
{
  if (m_tree == NULL)
    return const_iterator(NULL, true);
  else
    return const_iterator(m_tree->upper_bound(), true);
} // avl_base::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the nodes of the tree from a specified key.
 * \param key Key to find.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::iterator
claw::avl_base<K,Comp>::find( const K& key )
{
  return make_iterator( m_tree->find(key) );
} // avl_base::find()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the nodes of the tree from a specified key.
 * \param key Key to find.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::const_iterator
claw::avl_base<K,Comp>::find( const K& key ) const
{
  return make_const_iterator( m_tree->find(key) );
} // avl_base::find()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the nodes of the tree on the key imediatly after
 *        from a specified key.
 * \param key Key to find.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::iterator
claw::avl_base<K,Comp>::find_nearest_greater( const K& key )
{
  return make_iterator( m_tree->find_nearest_greater(key) );
} // avl_base::find_nearest_greater()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the nodes of the tree on the key imediatly after
 *        from a specified key.
 * \param key Key to find.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::const_iterator
claw::avl_base<K,Comp>::find_nearest_greater( const K& key ) const
{
  return make_const_iterator( m_tree->find_nearest_greater(key) );
} // avl_base::find_nearest_greater()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the nodes of the tree on the key imediatly before
 *        from a specified key.
 * \param key Key to find.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::iterator
claw::avl_base<K,Comp>::find_nearest_lower( const K& key )
{
  return make_iterator( m_tree->find_nearest_lower(key) );
} // avl_base::find_nearest_lower()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the nodes of the tree on the key imediatly before
 *        from a specified key.
 * \param key Key to find.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::const_iterator
claw::avl_base<K,Comp>::find_nearest_lower( const K& key ) const
{
  return make_const_iterator( m_tree->find_nearest_lower(key) );
} // avl_base::find_nearest_lower()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the lowest value of the tree.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::iterator claw::avl_base<K,Comp>::lower_bound()
{
  return make_iterator( m_tree->lower_bound() );
} // avl_base::lower_bound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the lowest value of the tree.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::const_iterator
claw::avl_base<K,Comp>::lower_bound() const
{
  return make_const_iterator( m_tree->lower_bound() );
} // avl_base::lower_bound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the gratest value of the tree.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::iterator claw::avl_base<K,Comp>::upper_bound()
{
  return make_iterator( m_tree->upper_bound() );
} // avl_base::upper_bound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the gratest value of the tree.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::const_iterator
claw::avl_base<K,Comp>::upper_bound() const
{
  return make_const_iterator( m_tree->upper_bound() );
} // avl_base::upper_bound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator
 * \param that AVL instance to copy from.
 */
template<class K, class Comp>
claw::avl_base<K, Comp>&
claw::avl_base<K, Comp>::operator=( const avl_base<K, Comp>& that )
{
  if (this != &that)
    {
      if (m_tree)
        {
          m_tree->del_tree();
          delete m_tree;
        }

      m_size = 0;

      if (that.m_tree)
        m_tree = that.m_tree->duplicate( m_size );
      else
        m_tree = NULL;
    }

  return *this;
} // avl_base::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param that AVL top compare to.
 */
template<class K, class Comp>
bool claw::avl_base<K, Comp>::operator==( const avl_base<K, Comp>& that ) const
{
  if (m_size != that.m_size)
    return false;
  else
    return std::equal( begin(), end(), that.begin(), s_key_less );
} // avl_base::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disequality.
 * \param that AVL top compare to.
 */
template<class K, class Comp>
bool claw::avl_base<K, Comp>::operator!=( const avl_base<K, Comp>& that ) const
{
  return !( *this == that );
} // avl_base::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Less than operator.
 * \param that AVL top compare to.
 */
template<class K, class Comp>
bool claw::avl_base<K, Comp>::operator<( const avl_base<K, Comp>& that ) const
{
  return std::lexicographical_compare
    ( begin(), end(), that.begin(), that.end(), s_key_less );
} // avl_base::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Greater than operator.
 * \param that AVL top compare to.
 */
template<class K, class Comp>
bool claw::avl_base<K, Comp>::operator>( const avl_base<K, Comp>& that ) const
{
  return that < *this;
} // avl_base::operator>()

/*----------------------------------------------------------------------------*/
/**
 * \brief Less or equal operator.
 * \param that AVL top compare to.
 */
template<class K, class Comp>
bool claw::avl_base<K, Comp>::operator<=( const avl_base<K, Comp>& that ) const
{
  return !( that < *this );
} // avl_base::operator<=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Greater or equal operator.
 * \param that AVL top compare to.
 */
template<class K, class Comp>
bool claw::avl_base<K, Comp>::operator>=( const avl_base<K, Comp>& that ) const
{
  return !( *this < that );
} // avl_base::operator>=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap the values with an other tree.
 * \param that The other tree.
 */
template<class K, class Comp>
void claw::avl_base<K, Comp>::swap( avl_base<K, Comp>& that )
{
  std::swap(m_size, that.m_size);
  std::swap(m_tree, that.m_tree);
} // avl_base::swap()

/*================================= private =================================*/

//-----------------------------------------------------------------------------
// We need some methods to check the validity of our trees

/*----------------------------------------------------------------------------*/
/**
 * \brief This method will check order in our trees.
 * \param node Root of the tree to check.
 * \param min Lower bound of the valid range for tree's nodes.
 * \param max Higher bound of the valid range for tree's nodes.
 * \remark For validity check.
 * \return true if bounds are ok, false otherwise.
 */
template<class K, class Comp>
bool claw::avl_base<K,Comp>::check_in_bounds
( const avl_node_ptr node, const K& min, const K& max ) const
{
  if (node == NULL)
    return true;
  else if ( !( s_key_less(node->key, min) || s_key_less(min, node->key) ) )
    return (node->left == NULL) 
      && check_in_bounds( node->right, node->key, max );
  else if ( !( s_key_less(node->key, max) || s_key_less(max, node->key) ) )
    return (node->right == NULL) 
      && check_in_bounds( node->left, min, node->key );
  else
    return s_key_less(node->key, max) && s_key_less(min, node->key) 
      && check_in_bounds( node->left, min, node->key )
      && check_in_bounds( node->right, node->key, max );
} // check_less_than()

/*----------------------------------------------------------------------------*/
/**
 * \brief This method will check balance in our trees.
 * \param node Root of the tree to check.
 * \remark For validity check.
 * \return true if the absolute difference between left subtree's depth and 
 *         right subtree's depth is 1 for node and each of its subtrees. 
 *         false otherwise.
 */
template<class K, class Comp>
bool claw::avl_base<K,Comp>::check_balance( const avl_node_ptr node ) const
{
  int pl=0, pr=0;

  if (node == NULL)
    return true;
  else
    {
      if (node->left) pl = node->left->depth();
      if (node->right) pr = node->right->depth();

      return (pl-pr>=-1) && (pl-pr<=1) && (pl-pr == node->balance)
        && check_balance(node->left) && check_balance(node->right);
    }
} // check_balance()

/*----------------------------------------------------------------------------*/
/**
 * \brief This method will check if each node is a son of his father.
 * \param node Node to check.
 * \remark For validity check.
 * \return true if the AVL is valid, false otherwise.
 */
template<class K, class Comp>
bool claw::avl_base<K,Comp>::correct_descendant( const avl_node_ptr node ) const
{
  bool valid = true;

  if (node != NULL)
    {
      if (node->father != NULL)
        {
          valid = (node->father->left == node) ^ (node->father->right == node);
          valid = valid && correct_descendant( node->left ) 
            && correct_descendant( node->right );
        }
      else
        valid = false;
    }
          
  return valid;
} // correct_descendant()

/*----------------------------------------------------------------------------*/
/**
 * \brief This method will check orderliness in our trees : balance and order.
 *
 * \remark For validity check.
 * \return true if the AVL is valid, false otherwise.
 */
template<class K, class Comp>
bool claw::avl_base<K,Comp>::validity_check() const
{
  bool valid = true;

  if (m_tree != NULL)
    {
      avl_node *node_min, *node_max;

      // get lower and higher bounds, hope they are correct
      for (node_min = m_tree; node_min->left!=NULL; node_min = node_min->left);
      for (node_max = m_tree; node_max->right!=NULL; 
           node_max = node_max->right);
                  
      valid = check_in_bounds(m_tree->left, node_min->key, m_tree->key);
      valid = valid 
        && check_in_bounds(m_tree->right, m_tree->key, node_max->key);
                  
      valid = valid && (m_tree->father == NULL) 
        && correct_descendant( m_tree->left ) 
        && correct_descendant( m_tree->right );
                  
    }
  
  return valid && check_balance(m_tree);
} // validity_check()





/*----------------------------------------------------------------------------*/
/**
 * \brief Create an iterator from a pointer to a node.
 * \param node The node on which we want the iterator.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::iterator
claw::avl_base<K,Comp>::make_iterator( avl_node_ptr node ) const
{
  if (node != NULL)
    return iterator(node, false);
  else
    return end();
} // avl_base::make_iterator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an iterator from a pointer to a node.
 * \param node The node on which we want the iterator.
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::const_iterator
claw::avl_base<K,Comp>::make_const_iterator( const_avl_node_ptr node ) const
{
  if (node != NULL)
    return const_iterator(node, false);
  else
    return end();
} // avl_base::make_const_iterator()




//-----------------------------------------------------------------------------
// Tree management methods

/*----------------------------------------------------------------------------*/
/**
 * \brief Node right rotation
 * \param node Node to rotate.
 * \pre (node != NULL) && node->left != NULL
 * \pre node->balance in [1,2] and node->left->balance in [-1,2]
 * \pre (node->left->balance == 2) ==> (node->balance == 2)
 */
template<class K, class Comp>
void claw::avl_base<K,Comp>::rotate_right( avl_node_ptr& node )
{
  avl_node_ptr p;
  char old_node_balance;
  char old_subtree_balance;

  assert( node != NULL );
  assert( node->left != NULL );
  assert( (1 <= node->balance) && (node->balance <= 2) ) ;
  assert( (-1 <= node->left->balance) && (node->left->balance <= 2) );
  assert( (node->left->balance != 2) || (node->balance == 2) );

  old_node_balance = node->balance;
  old_subtree_balance = node->left->balance;

  // rotate nodes
  p = node->left;
  p->father = node->father;

  node->left = p->right;

  if (p->right)
    p->right->father = node;

  p->right = node;
  node->father = p;

  node = p;

  // adjust balance
  switch(old_subtree_balance)
    {
    case -1 : 
      node->balance = -2;
      node->right->balance = old_node_balance - 1;
      break;
    case  0 : 
      node->balance = -1;
      node->right->balance = old_node_balance - 1;
      break;
    case  1 : 
      node->balance = old_node_balance - 2;
      node->right->balance = old_node_balance - 2;
      break;
    case  2 :
      // old_node_balance is 2 too.
      node->balance = 0;
      node->right->balance = - 1;
      break;
    }
} // rotate_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Node left rotation
 * \param node Node to rotate.
 * \pre (node != NULL) && node->right != NULL
 * \pre node->balance in [-2,-1] and node->right->balance in [-2,1]
 * \pre (node->right->balance == -2) ==> (node->balance == -2)
 */
template<class K, class Comp>
void claw::avl_base<K,Comp>::rotate_left( avl_node_ptr& node )
{
  avl_node_ptr p;
  char old_node_balance;
  char old_subtree_balance;

  assert( node != NULL );
  assert( node->right != NULL );
  assert( (-2 <= node->balance) && (node->balance <= -1) ) ;
  assert( (-2 <= node->right->balance) && (node->right->balance <= 1) );
  assert( (node->right->balance != -2) || (node->balance == -2) );

  old_node_balance = node->balance;
  old_subtree_balance = node->right->balance;

  // rotate nodes
  p = node->right;
  p->father = node->father;

  node->right = p->left;
          
  if (p->left)
    p->left->father = node;

  p->left = node;
  node->father = p;

  node = p;

  // adjust balance
  switch(old_subtree_balance)
    {
    case  -2 :
      // old_node_balance is -2 too.
      node->balance = 0;
      node->left->balance = 1;
      break;
    case -1 : 
      node->balance = old_node_balance + 2;
      node->left->balance = old_node_balance + 2;
      break;
    case  0 : 
      node->balance = 1;
      node->left->balance = old_node_balance + 1;
      break;
    case  1 : 
      node->balance = 2;
      node->left->balance = old_node_balance + 1;
      break;
    }
} // rotate_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Node left-right rotation
 * \param node Node to rotate.
 */
template<class K, class Comp>
void claw::avl_base<K,Comp>::rotate_left_right( avl_node_ptr& node )
{
  assert( node != NULL );

  rotate_left( node->left );
  rotate_right( node );
} // rotate_left_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Node right-left rotation
 * \param node Node to rotate.
 */
template<class K, class Comp>
void claw::avl_base<K,Comp>::rotate_right_left( avl_node_ptr& node )
{
  assert( node != NULL );

  rotate_right( node->right );
  rotate_left( node );
} // rotate_right_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update balance of each node by increasing depth of the substree 
 *        containing key, from node to the node key.
 * \param node Root of the subtree to update.
 * \param key Key of the just-added node.
 * \pre (node != NULL) && ( key is in the tree starting from root node )
 * \post balance is ok for each node from node to key
 */
template<class K, class Comp>
void claw::avl_base<K,Comp>::update_balance( avl_node_ptr node, const K& key )
{
  assert(node != NULL);
  bool done = false;

  while (!done)
    if ( s_key_less(key, node->key) )
      {
        ++node->balance;
        node = node->left;
      }
    else if ( s_key_less(node->key, key) )
      {
        --node->balance;
        node = node->right;
      }
    else
      done = true;
} // update_balance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust balance of a node so it will be in range [-1;1].
 * \param node Node to adjust.
 * \pre (node != NULL).
 * \post node->balance is in range [-1;1]
 */
template<class K, class Comp>
void claw::avl_base<K,Comp>::adjust_balance( avl_node_ptr& node )
{
  assert(node != NULL);

  if ( node->balance == 2)
    adjust_balance_left(node);
  else if ( node->balance == -2)
    adjust_balance_right(node);
} // adjust_balance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust balance of a node leaning on the left so it will be 
 *        in range [-1;1].
 * \param node Node to adjust.
 * \pre (node != NULL) && (*node != NULL) && ( (*node)->balance == 2).
 * \post node->balance is in range [-1;1]
 */
template<class K, class Comp>
void claw::avl_base<K,Comp>::adjust_balance_left( avl_node_ptr& node )
{
  assert(node != NULL);
  assert(node->balance == 2);

  if (node->left->balance > -1)
    rotate_right( node );
  else if ( node->left->balance == -1)
    rotate_left_right(node);
} // adjust_balance_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust balance of a node leaning on the right so it will be 
 *        in range [-1;1].
 * \param node Node to adjust.
 * \pre (node != NULL) && (*node != NULL) && ( (*node)->balance == -2).
 * \post node->balance is in range [-1;1]
 */
template<class K, class Comp>
void claw::avl_base<K,Comp>::adjust_balance_right( avl_node_ptr& node )
{
  assert(node != NULL);
  assert(node->balance == -2);

  if (node->right->balance < 1)
    rotate_left( node );
  else if ( node->right->balance == 1)
    rotate_right_left(node);
} // adjust_balance_right()


/*----------------------------------------------------------------------------*/
//    Methods for insertion
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**
 * \brief Add a node to the tree.
 * \param key Key for the new value.
 * \post exists(key)
 *           && (exists(old this, key)==0 => size(this) == size(old this) + 1 )
 */
template<class K, class Comp>
void claw::avl_base<K,Comp>::insert_node( const K& key )
{
  avl_node_ptr* new_node;
  avl_node_ptr node_father;
  avl_node_ptr last_imbalanced;
  avl_node_ptr last_imbalanced_father;
          
  assert(m_tree != NULL);
  
  new_node = find_node_reference(key, last_imbalanced, node_father  );

  if (*new_node == NULL) // this key isn't in use. Let's create a new node
    {
      *new_node = new avl_node(key);
      (*new_node)->father = node_father;

      ++m_size;
      last_imbalanced_father = last_imbalanced->father;

      // Update balance of the last imbalanced node
      update_balance( last_imbalanced, key );
      // then adjust it to be in range [-1, 1]
      adjust_balance( last_imbalanced );
                  
      // pointer update after rotation
      if ( last_imbalanced_father == NULL )
        {
          m_tree = last_imbalanced;
          m_tree->father = NULL;
        }
      else if (s_key_less(last_imbalanced->key, last_imbalanced_father->key)) 
        // left child
        last_imbalanced_father->left = last_imbalanced;
      else
        last_imbalanced_father->right = last_imbalanced;
    }
} // insert_node()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the node where to insert a new value at key.
 * \param key Key for the new value.
 * \param last_imbalanced (out) Pointer to the last imbalanced node seen.
 * \param node_father (out) Pointer to the father of the new node.
 * \return Pointer to the node corresponding of the key (if exists). Otherwise
 *         a pointer to a NULL node where to insert the new one.
 * \post ( exists(this, key) => *result == find(this, key) )
 *           && ( !exists(this, key) => *result the good node to allocate )
 *           && ( *last_imbalance and *last_imbalance are correct regarding to
 *                previous definitions )
 */
template<class K, class Comp>
typename claw::avl_base<K,Comp>::avl_node_ptr* 
claw::avl_base<K,Comp>::find_node_reference
( const K& key, avl_node_ptr& last_imbalanced, avl_node_ptr& node_father)
{
  avl_node_ptr* node; // node for search
  bool exists = false;        // if this key already exists

  last_imbalanced = m_tree;
  node = & m_tree;
  node_father = NULL;

  while ( ((*node) != NULL) && !exists )
    {
      if ( (*node)->balance != 0 )
        last_imbalanced = *node;

                  
      // find next node
      if ( s_key_less(key, (*node)->key) )
        {
          node_father = *node;
          node = & (*node)->left;
        }
      else if ( s_key_less((*node)->key, key) )
        {
          node_father = *node;
          node = & (*node)->right;
        }
      else
        exists = true;
    }

  return node;
} // find_node_reference()


/*----------------------------------------------------------------------------*/
//    Methods for deletion
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**
 * \brief Delete a node. Search is done recursively.
 * \param node Tree to which the node is removed.
 * \param key Key of the node to remove.
 * \return true if the balance of the node has changed.
 * \pre node != NULL
 * \post (exists(this, key) == 0)
 */
template<class K, class Comp>
bool
claw::avl_base<K,Comp>::recursive_delete( avl_node_ptr& node, const K& key )
{
  bool result = false;

  if ( node != NULL )
    {
      // try to find the key in the left subtree
      if ( s_key_less(key, node->key) ) 
        {
          if ( recursive_delete( node->left, key ) )
            result = new_balance( node, -1 );
        }
      // try to find the key in the right subtree
      else if ( s_key_less(node->key, key) ) 
        {
          if ( recursive_delete( node->right, key ) )
            result = new_balance( node, 1 ); 
        }
      else // we got it !
        {
          --m_size;
          result = recursive_delete_node( node );
        }
    }

  return result;
} // recursive_delete()


/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust balance of a node.
 * \param node Node to balance.
 * \param imbalance Imbalance to add to the node's balance.
 * \return true if the balance of the node has changed.
 * \pre node != NULL
 * \pre (imbalance==1) || (imbalance==-1)
 * \post node tree is an AVL
 */
template<class K, class Comp>
bool claw::avl_base<K,Comp>::new_balance( avl_node_ptr& node, int imbalance )
{
  assert( (imbalance==1) || (imbalance==-1) );
  assert( node != NULL );

  node->balance += imbalance;

  switch(node->balance)
    {
      // balance == 0 so as it was != 0 before deletion
      // balance of the tree had changed
    case 0: return true;
      // balance == 2 so it must be 1 before deletion and node
      // was deleted in the right subtree
      // if after ajusting the balance is equal to 1, it means that
      // our subtree got back his old balance (so it's unchanged).
      // if it's equal to -1, it means that subtree now lean to the
      // otherside. But in those cases, depth didn't changed.
    case 2: adjust_balance_left(node); return node->balance == 0;
      // same thing but symetric
    case -2: adjust_balance_right(node); return node->balance == 0;
    default : return false;
    }
} // new_balance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the root of an AVL 
 *        (exchange with the descendant immediatly lower).
 * \param node Node to remove.
 * \return true if the balance of the subtree has changed.
 * \pre node != NULL
 * \post node tree is an AVL
 */
template<class K, class Comp>
bool claw::avl_base<K,Comp>::recursive_delete_node( avl_node_ptr& node )
{
  assert( node != NULL );

  if ( node->left == NULL) // this node doesn't have a lower descendant
    {
      // Get right subtree of current node
      avl_node_ptr right_subtree = node->right; 

      if (right_subtree)
        right_subtree->father = node->father;

      // Free memory pointed by the current node
      node->clear();
      delete node;

      // then rise the old right subtree
      node = right_subtree;

      return true;
    }
  else // this node has a lower descendant, let's get it
    if ( recursive_delete_max( node->left, node ) )
      {
        // left subtree depth has decreased
        // so reajust balance (rem. balance is not changed by delete_max)
        --(node->balance);

        if ( node->balance == -2 )
          {
            // old balance was -1
            adjust_balance_right(node);
            return node->balance == 0; // tell if depth has changed
          }
        else if ( node->balance == 0 ) 
          // node had at least one subtree and old balance - 1 == 0
          // so old balance = 1
          return true;
        else // node's balance is -1
          // As node's balance is (old balance - 1), node's balance must be -1
          // (otherwise old balance is 2, that's impossible)
          // So old balance is 0.
          // Moreover old node have at least a left subtree. It means that 
          // old node had 2 subtrees and their depths were equals.
          // It means bstn_depth(old node) == bstn_depth((old node)->right) + 1
          // We deleted a node in left subtree and so right subtree is
          // unchanged. So bstn_depth(node) == bstn_depth(node->right) + 1 
          // == bstn_depth( (old node)->right) ) + 1 == bstn_depth(old node)
          // => Node depth is unchanged.
          return false;
      }
    else // depth is unchanged
      return false;
} // recursive_delete_node()

/*----------------------------------------------------------------------------*/
/**
 * \brief Replace a node key and data by the one of the node with the 
 *        maximum key in tree.
 * \param root Root of the tree in which find new values.
 * \param node Node Wich gets values founded
 * \return true if the balance of the tree from root has changed.
 * \pre node != NULL
 * \pre root != NULL
 * \pre root is an AVL
 * \post (root tree is an AVL) && (find(root, max(old root)) == 0)
 */
template<class K, class Comp>
int claw::avl_base<K,Comp>::recursive_delete_max
( avl_node_ptr& root, avl_node_ptr node )
{
  assert(node!=NULL);
  assert(root!=NULL);

  if ( root->right == NULL ) // We have the maximum
    {
      // node have only a left subtree if any.
      // This subtree has one node, at most.
      avl_node_ptr left_subtree;

      node->key = root->key;
      left_subtree = root->left;

      if (left_subtree)
        left_subtree->father = root->father;

      root->clear();
      delete root;
                  
      // rise the left subtree
      root = left_subtree;

      // depth have changed
      return true;
    }
  else // try to find the max in the right subtree
    if ( recursive_delete_max( root->right, node ) )
      {
        // depth of the subtree changed (ie. decreased)
        // so root's tree lean to the left
        ++(root->balance);

        if (root->balance == 2) // tree is leaning too much
          {
            // old balance was 1
            adjust_balance_left( root );
            return root->balance == 0; // Say if balance is changed
          }
        else 
          // if balance is 0, it means that old root leant to the left
          // and so his depth changed.
          // The other value for balance is 1, in this case
          // depth didn't change. See recursive_delete_node code comments.
          return root->balance == 0;
      }
    else // depth of the subtree didn't change
      return false;
} // recursive_delete_max()
