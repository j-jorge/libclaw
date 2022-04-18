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
 * \file claw/real_number.hpp
 * \brief Custom precision real numbers.
 * \author Julien Jorge
 */
#ifndef __CLAW_REAL_NUMBER_HPP__
#define __CLAW_REAL_NUMBER_HPP__

#include <iostream>
#include <limits>

namespace claw
{
  template <typename T>
  class real_number;
}

template <typename T>
std::istream& operator>>(std::istream& is, claw::real_number<T>& self);

namespace claw
{
  /**
   * \brief This class generates an epsilon value of a given precision.
   * \author Julien Jorge
   */
  template <typename T>
  struct make_epsilon
  {
    static T value(T v)
    {
      return std::abs(v) <= 1
                 ? std::numeric_limits<T>::epsilon()
                 : std::abs(v) * std::numeric_limits<T>::epsilon();
    }
  }; // struct make_epsilon

  /**
   * \brief Custom precision real numbers.
   * \author Julien Jorge
   */
  template <typename T>
  class real_number
  {
    friend std::istream& ::operator>><>(std::istream& is,
                                        real_number<T>& self);

  public:
    typedef T value_type;
    typedef real_number<T> self_type;

  public:
    real_number();
    real_number(const value_type& v);
    real_number(const self_type& that);

    self_type abs() const;

    bool operator<(const self_type& that) const;
    bool operator<=(const self_type& that) const;
    bool operator>(const self_type& that) const;
    bool operator>=(const self_type& that) const;
    bool operator==(const self_type& that) const;
    bool operator!=(const self_type& that) const;

    self_type operator+(const self_type& that) const;
    self_type operator-(const self_type& that) const;
    self_type operator*(const self_type& that) const;
    self_type operator/(const self_type& that) const;

    self_type& operator+=(const self_type& that);
    self_type& operator-=(const self_type& that);
    self_type& operator*=(const self_type& that);
    self_type& operator/=(const self_type& that);

    std::ostream& output(std::ostream& os) const;

    template <typename U>
    operator U() const;

  private:
    /** \brief The value of the number. */
    value_type m_value;

    /** \brief Precision applied to operators. */
    value_type m_epsilon;

  }; // class real_number
}

namespace std
{
  template <typename T>
  struct numeric_limits<claw::real_number<T> > : public numeric_limits<T>
  {}; // struct numeric_limits

  template <typename T>
  claw::real_number<T> abs(const claw::real_number<T>& v);
}

// unary minus
template <typename T>
claw::real_number<T> operator-(const claw::real_number<T>& self);

template <typename T>
claw::real_number<T> operator-(T v, const claw::real_number<T>& self);

template <typename T>
std::ostream& operator<<(std::ostream& os, const claw::real_number<T>& self);
template <typename T>
std::istream& operator>>(std::istream& is, claw::real_number<T>& self);

#include "claw/real_number.tpp"

#endif // __CLAW_REAL_NUMBER_HPP__
