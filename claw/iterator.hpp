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
 * \file iterator.hpp
 * \brief Some special kind of iterators. As an example: iterator on the keys of
 *        a map.
 * \author Julien Jorge
 */
#ifndef __CLAW_ITERATOR_HPP__
#define __CLAW_ITERATOR_HPP__

#include <iterator>

namespace claw
{
  /*-------------------------------------------------------------------------*/
  /**
   * \brief Base class for wrapped iterators.
   * \author Julien Jorge
   */
  template< typename Category, typename Value, typename Iterator,
	    typename Function >
  class wrapped_iterator_by_category
  {

  }; // class wrapped_iterator_by_category

  /*-------------------------------------------------------------------------*/
  /**
   * \brief Base class for wrapped iterators, specialized for forward iterators.
   * \author Julien Jorge
   */
  template<typename Value, typename Iterator, typename Function>
  class wrapped_iterator_by_category
  <std::forward_iterator_tag, Value, Iterator, Function>
  {
  public:
    typedef typename std::iterator_traits<Iterator>::difference_type
    difference_type;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef typename std::iterator_traits<Iterator>::iterator_category
    iterator_category;

    typedef
    wrapped_iterator_by_category
    <std::forward_iterator_tag, Value, Iterator, Function>
    self_type;

  public:
    wrapped_iterator_by_category() {}
    wrapped_iterator_by_category( const Iterator& it )
      : m_it(it)
    { }
    wrapped_iterator_by_category( const Iterator& it, const Function& f )
      : m_it(it), m_fun(f)
    { }
    template<typename C, typename V, typename I, typename F>
    wrapped_iterator_by_category
    ( const wrapped_iterator_by_category<C, V, I, F>& that )
      : m_it(that.get_iterator()), m_fun(that.get_function())
    { }

    const Iterator& get_iterator() const { return m_it; }
    const Function& get_function() const { return m_fun; }

    self_type& operator++()
    {
      ++m_it;
      return *this;
    }

    self_type operator++(int)
    {
      self_type tmp(*this);
      ++m_it;
      return tmp;
    }

    reference operator*() const { return m_fun(*m_it); }
    pointer operator->() const { return &m_fun(*m_it); }

    bool operator==( const self_type& that ) const { return m_it == that.m_it; }
    bool operator!=( const self_type& that ) const { return m_it != that.m_it; }
    bool operator==( const Iterator& it ) const { return m_it == it; }
    bool operator!=( const Iterator& it ) const { return m_it != it; }

  private:
    /** \brief The effective iterator. */
    Iterator m_it;

    /** \brief The function applied to the pointed items. */
    Function m_fun;

  }; // class wrapped_iterator_by_category [forward_iterator_tag]

  /*-------------------------------------------------------------------------*/
  /**
   * \brief Base class for wrapped iterators, specialized for bidirectional
   *        iterators.
   * \author Julien Jorge
   */
  template<typename Value, typename Iterator, typename Function>
  class wrapped_iterator_by_category
  <std::bidirectional_iterator_tag, Value, Iterator, Function>
  {
  public:
    typedef typename std::iterator_traits<Iterator>::difference_type
    difference_type;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef typename std::iterator_traits<Iterator>::iterator_category
    iterator_category;

    typedef
    wrapped_iterator_by_category
    <std::bidirectional_iterator_tag, Value, Iterator, Function> self_type;

  public:
    wrapped_iterator_by_category() {}
    wrapped_iterator_by_category( const Iterator& it )
      : m_it(it)
    { }
    wrapped_iterator_by_category( const Iterator& it, const Function& f )
      : m_it(it), m_fun(f)
    { }
    template<typename C, typename V, typename I, typename F>
    wrapped_iterator_by_category
    ( const wrapped_iterator_by_category<C, V, I, F>& that )
      : m_it(that.get_iterator()), m_fun(that.get_function())
    { }

    const Iterator& get_iterator() const { return m_it; }
    const Function& get_function() const { return m_fun; }

    self_type& operator++()
    {
      ++m_it;
      return *this;
    }

    self_type operator++(int)
    {
      self_type tmp(*this);
      ++m_it;
      return tmp;
    }

    self_type& operator--()
    {
      --m_it;
      return *this;
    }

    self_type operator--(int)
    {
      self_type tmp(*this);
      --m_it;
      return tmp;
    }

    reference operator*() const { return m_fun(*m_it); }
    pointer operator->() const { return &m_fun(*m_it); }
    
    bool operator==( const self_type& that ) const { return m_it == that.m_it; }
    bool operator!=( const self_type& that ) const { return m_it != that.m_it; }
    bool operator==( const Iterator& it ) const { return m_it == it; }
    bool operator!=( const Iterator& it ) const { return m_it != it; }

 private:
    /** \brief The effective iterator. */
    Iterator m_it;

    /** \brief The function applied to the pointed items. */
    Function m_fun;

  }; // class wrapped_iterator_by_category [bidirectional_iterator_tag]

  /*-------------------------------------------------------------------------*/
  /**
   * \brief Base class for wrapped iterators, specialized for random iterators.
   * \author Julien Jorge
   */
  template<typename Value, typename Iterator, typename Function>
  class wrapped_iterator_by_category
  <std::random_access_iterator_tag, Value, Iterator, Function>
  {
  public:
    typedef typename std::iterator_traits<Iterator>::difference_type
    difference_type;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef typename std::iterator_traits<Iterator>::iterator_category
    iterator_category;

    typedef
    wrapped_iterator_by_category
    <std::random_access_iterator_tag, Value, Iterator, Function>
    self_type;

  public:
    wrapped_iterator_by_category() {}
    wrapped_iterator_by_category( const Iterator& it )
      : m_it(it)
    { }
    wrapped_iterator_by_category( const Iterator& it, const Function& f )
      : m_it(it), m_fun(f)
    { }
    template<typename V, typename I>
    wrapped_iterator_by_category
    ( const wrapped_iterator_by_category
      <std::random_access_iterator_tag, V, I, Function>& that )
      : m_it(that.m_it), m_fun(that.m_fun)
    { }
    template<typename C, typename V, typename I, typename F>
    wrapped_iterator_by_category
    ( const wrapped_iterator_by_category<C, V, I, F>& that )
      : m_it(that.get_iterator()), m_fun(that.get_function())
    { }

    const Iterator& get_iterator() const { return m_it; }
    const Function& get_function() const { return m_fun; }

    self_type& operator++()
    {
      ++m_it;
      return *this;
    }

    self_type operator++(int)
    {
      self_type tmp(*this);
      ++m_it;
      return tmp;
    }

    self_type& operator--()
    {
      --m_it;
      return *this;
    }

    self_type operator--(int)
    {
      self_type tmp(*this);
      --m_it;
      return tmp;
    }

    reference operator*() const { return m_fun(*m_it); }
    pointer operator->() const { return &m_fun(*m_it); }

    bool operator==( const self_type& that ) const { return m_it == that.m_it; }
    bool operator!=( const self_type& that ) const { return m_it != that.m_it; }
    bool operator==( const Iterator& it ) const { return m_it == it; }
    bool operator!=( const Iterator& it ) const { return m_it != it; }
    bool operator<( const self_type& that ) const { return m_it < that.m_it; }
    bool operator<=( const self_type& that ) const { return m_it <= that.m_it; }
    bool operator>( const self_type& that ) const { return m_it > that.m_it; }
    bool operator>=( const self_type& that ) const { return m_it >= that.m_it; }

    self_type& operator+=(int n)
    {
      m_it += n;
      return *this;
    }

    self_type operator+(int n) const
    {
      self_type result(*this);
      result += n;
      return result;
    }

    self_type& operator-=(int n) { return *this += -n; }

    self_type operator-(int n) const
    {
      self_type result(*this);
      result -= n;
      return result;
    }

    reference operator[](int n) { return m_fun(m_it[n]); }

  private:
    /** \brief The effective iterator. */
    Iterator m_it;

    /** \brief The function applied to the pointed items. */
    Function m_fun;

  }; // class wrapped_iterator_by_category [random_access_iterator_tag]

  template<typename Value, typename Iterator, typename Function>
  wrapped_iterator_by_category
  <std::random_access_iterator_tag, Value, Iterator, Function>
  operator+
  ( int n,
    const wrapped_iterator_by_category
    < std::random_access_iterator_tag, Value, Iterator, Function >& it )
  {
    return it + n;
  }

  template<typename Value, typename Iterator, typename Function>
  wrapped_iterator_by_category
  <std::random_access_iterator_tag, Value, Iterator, Function>
  operator-
  ( int n,
    const wrapped_iterator_by_category
    < std::random_access_iterator_tag, Value, Iterator, Function >& it )
  {
    return it - n;
  }

  /*-------------------------------------------------------------------------*/
  /**
   * \brief This class defines an iterator resulting of the appliance of a
   *        function to an effective iterator.
   *
   * \b Template \b arguments:
   * - \a Value The value_type of the resulting iterator,
   * - \a Iterator The original iterator type,
   * - \a Function The function applied to the pointed values (this is the
   *   wrapper).
   *
   * \author Julien Jorge
   */
  template <typename Value, typename Iterator, typename Function>
  class wrapped_iterator
  {
  public:
    /** \brief This is the type of the iterator that you want. */
    typedef wrapped_iterator_by_category
    < typename std::iterator_traits<Iterator>::iterator_category,
      Value, Iterator, Function >
    iterator_type;

  }; // class wrapped_iterator
} // namespace claw

#endif // __CLAW_ITERATOR_HPP__
