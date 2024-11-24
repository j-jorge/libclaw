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
 * \file it_index.hpp
 * \brief A class to manage an index and an iterator easily.
 * \author Julien Jorge
 */
#ifndef __CLAW_IT_INDEX_HPP__
#define __CLAW_IT_INDEX_HPP__

#include <iostream>

namespace claw
{
  /**
   * \brief A class to manage an index and an iterator easily.
   * \param T Type of the iterator.
   * \author Julien Jorge
   */
  template <class T>
  class it_index
  {
  public:
    typedef typename std::iterator_traits<T>::value_type value_type;
    typedef typename std::iterator_traits<T>::difference_type difference_type;
    typedef typename std::iterator_traits<T>::pointer pointer;
    typedef typename std::iterator_traits<T>::reference reference;

  private:
    /** \brief Iterator. */
    T m_it;

    /** \brief Iterator's position. */
    int m_index;

  public:
    /** \brief Constructor. */
    it_index()
      : m_it()
      , m_index()
    {}

    /**
     * \brief Constructor.
     * \param it The iterator.
     * \param index Iterator's position.
     */
    it_index(const T& it, int index = 0)
      : m_it(it)
      , m_index(index)
    {}

    /**
     * \brief Copy constructor.
     * \param that it_index to copy from.
     */
    it_index(const it_index<T>& that)
      : m_it(that.m_it)
      , m_index(that.m_index)
    {}

    /**
     * \brief Change the current pair.
     * \param it The new iterator.
     * \param index New iterator's position.
     */
    void set(const T& it, int index)
    {
      m_it = it;
      m_index = index;
    }

    bool operator<(const it_index<T>& that) const
    {
      return m_index < that.m_index;
    }

    bool operator<(const T& it) const
    {
      return m_it < it;
    }
    bool operator<(int index) const
    {
      return m_index < index;
    }

    bool operator<=(const it_index<T>& that) const
    {
      return (*this < that) || (*this == that);
    }
    bool operator<=(const T& it) const
    {
      return m_it <= it;
    }
    bool operator<=(int index) const
    {
      return m_index <= index;
    }

    bool operator>(const it_index<T>& that) const
    {
      return m_index > that.m_index;
    }
    bool operator>(const T& it) const
    {
      return m_it > it;
    }
    bool operator>(int index) const
    {
      return m_index > index;
    }

    bool operator>=(const it_index<T>& that) const
    {
      return (*this > that) || (*this == that);
    }
    bool operator>=(const T& it) const
    {
      return m_it >= it;
    }
    bool operator>=(int index) const
    {
      return m_index >= index;
    }

    bool operator==(const it_index<T>& that) const
    {
      return (m_it == that.m_it) && (m_index == that.m_index);
    }
    bool operator==(const T& it) const
    {
      return m_it == it;
    }
    bool operator==(int index) const
    {
      return m_index == index;
    }

    bool operator!=(const it_index<T>& that) const
    {
      return !(*this == *that);
    }
    bool operator!=(const T& it) const
    {
      return m_it != it;
    }
    bool operator!=(int index) const
    {
      return m_index != index;
    }

    it_index<T> operator+(int index) const
    {
      return it_index<T>(m_it + index, m_index + index);
    }
    it_index<T> operator-(int index) const
    {
      return it_index<T>(m_it - index, m_index - index);
    }
    it_index<T> operator*(int index) const
    {
      return it_index<T>(m_it + (index - 1) * m_index, m_index * index);
    }
    it_index<T> operator/(int index) const
    {
      return it_index<T>(m_it - (m_index - m_index / index), m_index / index);
    }

    reference operator*() const
    {
      return *m_it;
    }
    pointer operator->() const
    {
      return &*m_it;
    }

    // Préincrément
    it_index<T>& operator++()
    {
      ++m_it;
      ++m_index;
      return *this;
    }

    // Postincrément
    it_index<T> operator++(int)
    {
      it_index<T> r(*this);
      ++(*this);
      return r;
    }

    // Préincrément
    it_index<T>& operator--()
    {
      --m_it;
      --m_index;
      return *this;
    }

    // Postincrément
    it_index<T> operator--(int)
    {
      it_index<T> r(*this);
      --(*this);
      return r;
    }

    it_index<T>& operator+=(int index)
    {
      m_it += index;
      m_index += index;
      return *this;
    }

    it_index<T>& operator-=(int index)
    {
      m_it -= index;
      m_index -= index;
      return *this;
    }

    it_index<T>& operator*=(int index)
    {
      m_it += (index - 1) * m_index;
      m_index *= index;
      return *this;
    }

    it_index<T>& operator/=(int index)
    {
      m_it -= m_index - m_index / index;
      m_index /= index;
      return *this;
    }

    operator int() const
    {
      return m_index;
    }
    operator T() const
    {
      return m_it;
    }

  }; // it_index;

}

#endif // __CLAW_IT_INDEX_HPP__
