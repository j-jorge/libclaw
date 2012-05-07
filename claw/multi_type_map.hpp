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
 * \file multi_type_map.hpp
 * \brief This class can associate values of different types to a key.
 * \author Julien Jorge
 */
#ifndef __CLAW_MULTI_TYPE_MAP_HPP__
#define __CLAW_MULTI_TYPE_MAP_HPP__

#include <claw/meta/no_type.hpp>
#include <claw/meta/type_list.hpp>
#include <map>

namespace claw
{
  /**
   * \brief This class provides types and methods to allow the call of methods
   *        for a given type in the inheritance of a multi_type_map.
   * \remark This class is to be used internally.
   *
   * Suppose we have defined the following variable
   *
   * <tt>multi_type_map<int, type_list<std::string, type_list<int, no_type> > >
   * my_map;</tt>
   *
   * The call <tt>int i = my_map.get<int>( 24 );</tt> doesn't match the type
   * of the end class. So the call is repercuted on the parent class until the
   * types match. Then, we can get the values in m_data.
   *
   * \b Template \b parameters
   * - \a ValueType is the type of the values on which the action is done.
   * - \a Map is the type of the map in which the action is done.
   *
   * \author Julien Jorge
   */
  template<typename ValueType, typename Map>
  class multi_type_map_wrapper;

  /**
   * \brief This class provides types and methods to allow the call of methods
   *        recursively along the inherintance hierarchy.
   * \remark This class is to be used internally.
   */
  template<typename Map>
  class multi_type_map_helper;

  /**
   * \brief This class can associate values of different types to a key.
   *
   * \b Template \b parameters
   * - \a Key The type to use for the keys.
   * - \a TypeList The list of the types that the map should support.
   *
   * \b Type \b requirements
   * - \a Key must be \a LessThanComparable.
   * - The last type in \a TypeList must be claw::meta::no_type.
   * - type_list_is_a_set<TypeList>::result == true
   *
   * \b Example
   *
   * <tt>multi_type_map<int, type_list<std::string, type_list<int, no_type> > >
   * my_map;</tt>
   *
   * \author Julien Jorge
   */
  template<typename Key, typename TypeList>
  class multi_type_map;

  /**
   * \brief Partial specialization, to stop the inheritance recursivity.
   */
  template<typename Key>
  class multi_type_map<Key, meta::no_type>
  {

  }; // class multi_type_map

  /**
   * \brief This class can associate values of different types to a key.
   * \author Julien Jorge
   */
  template<typename Key, typename Head, typename Tail>
  class multi_type_map< Key, meta::type_list<Head, Tail> >:
    public multi_type_map<Key, Tail>
  {
  public:
    typedef Key key_type;
    typedef Head value_type;
    typedef meta::type_list<Head, Tail> value_type_list;
    typedef multi_type_map< Key, meta::type_list<Head, Tail> > self_type;
    typedef std::map<key_type, value_type> container_type;
    typedef multi_type_map<Key, Tail> super;

    friend struct multi_type_map_wrapper<value_type, self_type>;
    friend struct multi_type_map_helper<self_type>;

    /** \brief This structure is used to create an iterator on the values of a
        given type. */
    template<typename ValueType>
    struct iterator
    {
      /** \brief The type of the iterator. */
      typedef typename std::map<key_type, ValueType>::iterator type;

      /** \brief The type of the const_iterator. */
      typedef
      typename std::map<key_type, ValueType>::const_iterator const_type;
    }; // struct iterator

  private:
    typedef typename iterator<value_type>::type iterator_type;
    typedef typename iterator<value_type>::const_type const_iterator_type;

  public:
    template<typename ValueType>
    void erase( typename iterator<ValueType>::type it );

    template<typename ValueType>
    std::size_t erase( const key_type& k );

    template<typename ValueType>
    const ValueType& get( const key_type& k ) const;

    template<typename ValueType>
    ValueType& get( const key_type& k );

    template<typename ValueType>
    void set( const key_type& k, const ValueType& v );

    void set( const self_type& m );

    template<typename ValueType>
    bool exists( const key_type& k ) const;

    std::size_t size() const;

    template<typename ValueType>
    typename iterator<ValueType>::type begin();

    template<typename ValueType>
    typename iterator<ValueType>::type end();

    template<typename ValueType>
    typename iterator<ValueType>::const_type begin() const;

    template<typename ValueType>
    typename iterator<ValueType>::const_type end() const;

  private:
    /** \brief Data stored for the first type of the list. */
    container_type m_data;

  }; // class multi_type_map

} // namespace claw

#include <claw/impl/multi_type_map.tpp>

#endif // __CLAW_MULTI_TYPE_MAP_HPP__
