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
 * \file multi_type_map.tpp
 * \brief Implementation of the claw::multi_type_map class.
 * \author Julien Jorge
 */

#include <claw/assert.hpp>
#include <claw/meta/same_type.hpp>

namespace claw
{
  /*
   * Here is the implementation of multi_type_map_wrapper for the case where
   * the ValueType is the first type in the type list of the map.
   */
  template <typename Key, typename Head, typename Tail>
  class multi_type_map_wrapper<
      Head, multi_type_map<Key, meta::type_list<Head, Tail> > >
  {
    typedef Key key_type;
    typedef Head value_type;
    typedef multi_type_map<Key, meta::type_list<Head, Tail> > map_type;
    typedef typename map_type::iterator_type iterator;
    typedef typename map_type::const_iterator_type const_iterator;

  public:
    static void erase(map_type& self, iterator it)
    {
      self.m_data.erase(it);
    }

    static std::size_t erase(map_type& self, const key_type& k)
    {
      return self.m_data.erase(k);
    }

    static const value_type& get(const map_type& self, const key_type& k)
    {
      CLAW_PRECOND(exists(self, k));
      return self.m_data.find(k)->second;
    }

    static value_type& get(map_type& self, const key_type& k)
    {
      CLAW_PRECOND(exists(self, k));
      return self.m_data.find(k)->second;
    }

    static void set(map_type& self, const key_type& k, const value_type& v)
    {
      self.m_data[k] = v;
    }

    static bool exists(const map_type& self, const key_type& k)
    {
      return self.m_data.find(k) != self.m_data.end();
    }

    static iterator begin(map_type& self)
    {
      return self.m_data.begin();
    }

    static iterator end(map_type& self)
    {
      return self.m_data.end();
    }

    static const_iterator begin(const map_type& self)
    {
      return self.m_data.begin();
    }

    static const_iterator end(const map_type& self)
    {
      return self.m_data.end();
    }

  }; // class multi_type_map_wrapper

  /*
   * Here is the implementation of multi_type_map_wrapper for the case where
   * the ValueType is not the first type in the type list of the map.
   */
  template <typename ValueType, typename Key, typename Head, typename Tail>
  class multi_type_map_wrapper<
      ValueType, multi_type_map<Key, meta::type_list<Head, Tail> > >
    : public multi_type_map_wrapper<ValueType, multi_type_map<Key, Tail> >
  {}; // class multi_type_map_wrapper

  /*
   * Here is the implementation of multi_type_map_helper for the case where the
   * ValueType is the first type in the type list of the map.
   */
  template <typename Key, typename Head, typename Tail>
  class multi_type_map_helper<
      multi_type_map<Key, meta::type_list<Head, Tail> > >
  {
    typedef Key key_type;
    typedef Head value_type;
    typedef multi_type_map<Key, meta::type_list<Head, Tail> > map_type;
    typedef typename map_type::iterator_type iterator;
    typedef typename map_type::const_iterator_type const_iterator;

  public:
    static void set(map_type& self, const map_type& that)
    {
      for(const_iterator it = that.template begin<value_type>();
          it != that.template end<value_type>(); ++it)
        self.template set<Head>(it->first, it->second);

      multi_type_map_helper<multi_type_map<Key, Tail> >::set(self, that);
    }

    static std::size_t size(const map_type& self)
    {
      return self.m_data.size()
             + multi_type_map_helper<multi_type_map<Key, Tail> >::size(self);
    }

  }; // class multi_type_map_helper

  /*
   * Here is the implementation of multi_type_map_helper that stops the
   * recursivity.
   */
  template <typename Key>
  class multi_type_map_helper<multi_type_map<Key, claw::meta::no_type> >
  {
  private:
    typedef multi_type_map<Key, claw::meta::no_type> map_type;

  public:
    static void set(map_type& self, const map_type& that)
    {
      // nothing to do
    }

    static std::size_t size(const map_type& self)
    {
      return 0;
    }

  }; // class multi_type_map_helper
}

/**
 * \brief Erase a value from the map.
 * \param k The key of the value to erase.
 * \return The number of removed elements (zero or one).
 */
template <typename Key, typename Head, typename Tail>
template <typename ValueType>
void claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::erase(
    typename iterator<ValueType>::type it)
{
  multi_type_map_wrapper<ValueType, self_type>::erase(*this, it);
}

/**
 * \brief Erase a value from the map.
 * \param k The key of the value to erase.
 * \return The number of removed elements (zero or one).
 */
template <typename Key, typename Head, typename Tail>
template <typename ValueType>
std::size_t
claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::erase(
    const key_type& k)
{
  return multi_type_map_wrapper<ValueType, self_type>::erase(*this, k);
}

/**
 * \brief Get a value from the map.
 * \param k The key of the value to get.
 */
template <typename Key, typename Head, typename Tail>
template <typename ValueType>
const ValueType&
claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::get(
    const key_type& k) const
{
  return multi_type_map_wrapper<ValueType, self_type>::get(*this, k);
}

/**
 * \brief Get a value from the map.
 * \param k The key of the value to get.
 */
template <typename Key, typename Head, typename Tail>
template <typename ValueType>
ValueType& claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::get(
    const key_type& k)
{
  return multi_type_map_wrapper<ValueType, self_type>::get(*this, k);
}

/**
 * \brief Set a value in the map.
 * \param k The key of the value to set.
 * \param v The value to set.
 */
template <typename Key, typename Head, typename Tail>
template <typename ValueType>
void claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::set(
    const key_type& k, const ValueType& v)
{
  return multi_type_map_wrapper<ValueType, self_type>::set(*this, k, v);
}

/**
 * \brief Set a value in the map.
 * \param k The key of the value to set.
 * \param v The value to set.
 */
template <typename Key, typename Head, typename Tail>
void claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::set(
    const self_type& m)
{
  multi_type_map_helper<self_type>::set(*this, m);
}

/**
 * \brief Tell if the map contains a value of a given type with a given key.
 * \param k The key of the value to get.
 */
template <typename Key, typename Head, typename Tail>
template <typename ValueType>
bool claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::exists(
    const key_type& k) const
{
  return multi_type_map_wrapper<ValueType, self_type>::exists(*this, k);
}

/**
 * \brief Return the number of elements in the map.
 */
template <typename Key, typename Head, typename Tail>
std::size_t
claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::size() const
{
  return multi_type_map_helper<self_type>::size(*this);
}

/**
 * \brief Get an iterator on the beginning of the map for a given type.
 */
template <typename Key, typename Head, typename Tail>
template <typename ValueType>
typename claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::
    template iterator<ValueType>::type
    claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::begin()
{
  return multi_type_map_wrapper<ValueType, self_type>::begin(*this);
}

/**
 * \brief Get an iterator on the end of the map for a given type.
 */
template <typename Key, typename Head, typename Tail>
template <typename ValueType>
typename claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::
    template iterator<ValueType>::type
    claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::end()
{
  return multi_type_map_wrapper<ValueType, self_type>::end(*this);
}

/**
 * \brief Get an iterator on the beginning of the map for a given type.
 */
template <typename Key, typename Head, typename Tail>
template <typename ValueType>
typename claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::
    template iterator<ValueType>::const_type
    claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::begin()
        const
{
  return multi_type_map_wrapper<ValueType, self_type>::begin(*this);
}

/**
 * \brief Get an iterator on the end of the map for a given type.
 */
template <typename Key, typename Head, typename Tail>
template <typename ValueType>
typename claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::
    template iterator<ValueType>::const_type
    claw::multi_type_map<Key, claw::meta::type_list<Head, Tail> >::end() const
{
  return multi_type_map_wrapper<ValueType, self_type>::end(*this);
}
