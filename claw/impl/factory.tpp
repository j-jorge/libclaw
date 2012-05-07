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
 * \file factory.tpp
 * \brief Implementation of the claw::pattern::factory class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
template<typename BaseClass, typename IdentifierType>
claw::pattern::factory
<BaseClass, IdentifierType>::class_creator_base::~class_creator_base()
{

} //  factory::class_creator_base::~class_creator_base()




/*----------------------------------------------------------------------------*/
/**
 * \brief Create an instance of the derived type..
 */
template<typename BaseClass, typename IdentifierType>
template<typename Derived>
Derived* claw::pattern::factory
<BaseClass, IdentifierType>::class_creator<Derived>::create() const
{
  return new Derived;
} // factory::class_creator::create()




/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
template<typename BaseClass, typename IdentifierType>
claw::pattern::factory<BaseClass, IdentifierType>::~factory()
{
  typename class_map::const_iterator it;

  for (it=m_classes.begin(); it!=m_classes.end(); ++it)
    delete it->second;

  m_classes.clear();
} // factory::~factory()

/*----------------------------------------------------------------------------*/
/**
 * \brief Register a type in the factory.
 * \param id The identifier to which the type is associated.
 * \return true if successful.
 *
 * \b Template \b parameters:
 * - \a T The type to register.
 */
template<typename BaseClass, typename IdentifierType>
template<typename T>
bool
claw::pattern::factory<BaseClass, IdentifierType>::register_type
( const identifier_type& id )
{
  typename class_map::iterator it = m_classes.find(id);

  if ( it == m_classes.end() )
    {
      m_classes[id] = new class_creator<T>;
      return true;
    }
  else
    return false;
} // factory::register_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a new instance.
 * \param id The identifier of the type to instanciate.
 * \return A pointer to the newly created instance.
 */
template<typename BaseClass, typename IdentifierType>
typename claw::pattern::factory<BaseClass, IdentifierType>::base_class*
claw::pattern::factory<BaseClass, IdentifierType>::create
( const identifier_type& id ) const
{
  typename class_map::const_iterator it = m_classes.find(id);

  if ( it==m_classes.end() )
    throw bad_type_identifier();
  else
    return it->second->create();
} // factory::create()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell in a type with a given id is known by the factory.
 * \param id The identifier of the type to check.
 */
template<typename BaseClass, typename IdentifierType>
bool claw::pattern::factory<BaseClass, IdentifierType>::is_known_type
( const identifier_type& id ) const
{
  return m_classes.find(id) != m_classes.end();
} // factory::is_known_type()
