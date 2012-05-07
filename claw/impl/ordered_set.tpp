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
 * \file ordered_set.tpp
 * \brief Implementation of the claw::math::ordered_set
 * \author Julien Jorge
 */
#include <list>

/*----------------------------------------------------------------------------*/
template<class K, class Comp>
Comp claw::math::ordered_set<K,Comp>::s_key_comp;

/*----------------------------------------------------------------------------*/
/**
 * \brief Intersection.
 * \param that The instance to intersect from.
 */
template<class K, class Comp>
claw::math::ordered_set<K, Comp>&
claw::math::ordered_set<K, Comp>::operator*=( const ordered_set& that )
{
  return intersection( that );
} // ordered_set::operator*=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Union.
 * \param that The instance to join with.
 */
template<class K, class Comp>
claw::math::ordered_set<K, Comp>&
claw::math::ordered_set<K, Comp>::operator+=( const ordered_set& that )
{
  return join( that );
} // ordered_set::operator+=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Difference.
 * \param that The instance from which to remove items.
 */
template<class K, class Comp>
claw::math::ordered_set<K, Comp>&
claw::math::ordered_set<K, Comp>::operator-=( const ordered_set& that )
{
  return difference( that );
} // ordered_set::operator-=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Symetric difference.
 * \param that The instance to differ from.
 */
template<class K, class Comp>
claw::math::ordered_set<K, Comp>&
claw::math::ordered_set<K, Comp>::operator/=( const ordered_set& that )
{
  return symetric_difference( that );
} // ordered_set::operator/=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inclusion.
 * \param that The instance that should be contained.
 * \return true if that is strictly included in this.
 */
template<class K, class Comp>
bool
claw::math::ordered_set<K, Comp>::operator>( const ordered_set& that ) const
{
  return strictly_contains( that );
} // ordered_set::operator>()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inclusion or equality.
 * \param that The instance that should be contained.
 * \return true if that is included in this.
 */
template<class K, class Comp>
bool
claw::math::ordered_set<K, Comp>::operator>=( const ordered_set& that ) const
{
  return contains( that );
} // ordered_set::operator>=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inclusion.
 * \param that The instance that should contain.
 * \return true if that is strictly included in this.
 */
template<class K, class Comp>
bool
claw::math::ordered_set<K, Comp>::operator<( const ordered_set& that ) const
{
  return that.strictly_contains( *this );
} // ordered_set::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inclusion or equality.
 * \param that The instance that should be contained.
 * \return true if that is included in this.
 */
template<class K, class Comp>
bool
claw::math::ordered_set<K, Comp>::operator<=( const ordered_set& that ) const
{
  return that.contains( *this );
} // ordered_set::operator<=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Intersection.
 * \param that The instance to intersect from.
 */
template<class K, class Comp>
claw::math::ordered_set<K, Comp>&
claw::math::ordered_set<K, Comp>::intersection( const ordered_set& that )
{
  std::list<K> remove_us;
  const_iterator it;
  
  for (it=super::begin(); it!=super::end(); ++it)
    if ( that.find( *it ) == that.end() )
      remove_us.push_front( *it );

  typename std::list<K>::const_iterator remove_it;

  for (remove_it=remove_us.begin(); remove_it!=remove_us.end(); ++remove_it)
    super::erase( *remove_it );

  return *this;
} // ordered_set::intersection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Union.
 * \param that The instance to join with.
 */
template<class K, class Comp>
claw::math::ordered_set<K, Comp>&
claw::math::ordered_set<K, Comp>::join( const ordered_set& that )
{
  const_iterator it;
  
  for (it=that.begin(); it!=that.end(); ++it)
    super::insert( *it );

  return *this;
} // ordered_set::join()

/*----------------------------------------------------------------------------*/
/**
 * \brief Difference.
 * \param that The instance from which to remove items.
 */
template<class K, class Comp>
claw::math::ordered_set<K, Comp>&
claw::math::ordered_set<K, Comp>::difference( const ordered_set& that )
{
  std::list<K> remove_us;
  const_iterator it;
  
  for (it=super::begin(); it!=super::end(); ++it)
    if ( that.find( *it ) != that.end() )
      remove_us.push_front( *it );

  typename std::list<K>::const_iterator remove_it;

  for (remove_it=remove_us.begin(); remove_it!=remove_us.end(); ++remove_it)
    super::erase( *remove_it );

  return *this;
} // ordered_set::difference()

/*----------------------------------------------------------------------------*/
/**
 * \brief Symetric difference.
 * \param that The instance to differ from.
 */
template<class K, class Comp>
claw::math::ordered_set<K, Comp>&
claw::math::ordered_set<K, Comp>::symetric_difference( const ordered_set& that )
{
  ordered_set<K, Comp> my_copy(*this), his_copy(that);

  return difference( that ).join( his_copy.difference(my_copy) );
} // ordered_set::symetric_difference()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inclusion or equality.
 * \param that The instance that should be contained.
 * \return true if that is included in this.
 */
template<class K, class Comp>
bool
claw::math::ordered_set<K, Comp>::contains( const ordered_set& that ) const
{
  bool ok = super::size() >= that.size();
  const_iterator it_this( super::begin() );
  const_iterator it_that( that.begin() );

  while ( ok && (it_that != that.end()) && (it_this != super::end()) )
    if ( s_key_comp( *it_this, *it_that ) )
      ++it_this;
    else if ( s_key_comp( *it_that, *it_this ) )
      ok = false;
    else
      {
        ++it_this;
        ++it_that;
      }

  return it_that == that.end();
} // ordered_set::contains()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inclusion.
 * \param that The instance that should contain.
 * \return true if that is strictly included in this.
 */
template<class K, class Comp>
bool
claw::math::ordered_set<K, Comp>::strictly_contains
( const ordered_set& that ) const
{
  return contains(that) && ( super::size() > that.size() );
} // ordered_set::strictly_contains()

