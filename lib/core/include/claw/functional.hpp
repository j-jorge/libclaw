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
 * \file functional.hpp
 * \brief Some function object classes.
 * \author Julien Jorge
 */
#ifndef __CLAW_FUNCTIONAL_HPP__
#define __CLAW_FUNCTIONAL_HPP__

#include <utility>

namespace claw
{
  /**
   * \brief Fuction object to get the first element of a std::pair.
   * \author Julien Jorge
   */
  template <class T1, class T2>
  class first
  {
  public:
    using argument_type = std::pair<T1, T2>&;
    using result_type = T1&;

  public:
    T1& operator()(std::pair<T1, T2>& p) const
    {
      return p.first;
    }
  }; // class first

  /**
   * \brief Fuction object to get the first element of a std::pair.
   * \author Julien Jorge
   */
  template <class T1, class T2>
  class const_first
  {
  public:
    using argument_type = const std::pair<T1, T2>&;
    using result_type = const T1&;

  public:
    const T1& operator()(const std::pair<T1, T2>& p) const
    {
      return p.first;
    }

  }; // class const_first

  /**
   * \brief Fuction object to get the first element of a std::pair.
   *
   * \remark Contrary to claw::first, the template argument is a std::pair.
   * \author Julien Jorge
   */
  template <class Pair>
  class pair_first
    : public first<typename Pair::first_type, typename Pair::second_type>
  {
    // nothing
  }; // class pair_first

  /**
   * \brief Fuction object to get the first element of a std::pair.
   *
   * \remark Contrary to claw::first, the template argument is a std::pair.
   * \author Julien Jorge
   */
  template <class Pair>
  class const_pair_first
    : public const_first<typename Pair::first_type, typename Pair::second_type>
  {
    // nothing
  }; // class const_pair_first

  /**
   * \brief Fuction object to get the second element of a std::pair.
   * \author Julien Jorge
   */
  template <class T1, class T2>
  class second
  {
  public:
    using argument_type = std::pair<T1, T2>&;
    using result_type = T2&;

  public:
    T2& operator()(std::pair<T1, T2>& p) const
    {
      return p.second;
    }
  }; // class second

  /**
   * \brief Fuction object to get the second element of a std::pair.
   * \author Julien Jorge
   */
  template <class T1, class T2>
  class const_second
  {
  public:
    using argument_type = const std::pair<T1, T2>&;
    using result_type = const T2&;

  public:
    const T2& operator()(const std::pair<T1, T2>& p) const
    {
      return p.second;
    }

  }; // class const_second

  /**
   * \brief Fuction object to get the second element of a std::pair.
   *
   * \remark Contrary to claw::second, the template argument is a std::pair.
   * \author Julien Jorge
   */
  template <class Pair>
  class pair_second
    : public second<typename Pair::first_type, typename Pair::second_type>
  {
    // nothing
  }; // class pair_second

  /**
   * \brief Fuction object to get the second element of a std::pair.
   *
   * \remark Contrary to claw::second, the template argument is a std::pair.
   * \author Julien Jorge
   */
  template <class Pair>
  class const_pair_second
    : public const_second<typename Pair::first_type,
                          typename Pair::second_type>
  {
  public:
    const_pair_second()
    {}

    template <typename F, typename S>
    const_pair_second(const second<F, S>&)
    {}

  }; // class const_pair_second

  /**
   * \brief Always true unary predicate.
   *
   * \b Template \b arguments:
   * - T Doesn't matter.
   *
   * \remark Use it when you need to process all elements but a method needs
   *         an unary predicate parameter.
   * \author Julien Jorge
   */
  template <class T>
  class unary_true
  {
  public:
    using argument_type = const T&;
    using result_type = bool;

  public:
    bool operator()(const T& t) const
    {
      return true;
    }
  }; // class unary_true

  /**
   * \brief Always true binary predicate.
   *
   * \b Template \b arguments:
   * - T Doesn't matter,
   * - U Doesn't matter.
   *
   * \remark Use it when you need to process all elements but a method needs
   *         an binary predicate parameter.
   * \author Julien Jorge
   */
  template <class T, class U>
  class binary_true
  {
  public:
    using first_argument_type = const T&;
    using second_argument_type = const U&;
    using result_type = bool;

  public:
    bool operator()(const T& t, const U& u) const
    {
      return true;
    }
  }; // class binary_true

  /**
   * \brief Function object that compose two function objects.
   *
   * \b Template \b arguments:
   * - \a F1 The type of the first function, must be a model of Adaptable Unary
   *   Function,
   * - \a F2 The type of the second function, must be a model of Adaptable
   * Unary Function.
   *
   * \author Julien Jorge
   */
  template <typename F1, typename F2>
  class unary_compose
  {
  public:
    using argument_type = typename F2::argument_type;
    using result_type = typename F1::result_type;

  public:
    unary_compose()
    {}

    /**
     * \brief Copy constructor.
     * \param that The instance to copy from.
     *
     * There is actually nothing to copy but this constructor is useful for
     * casting function objects.
     */
    template <typename G1, typename G2>
    unary_compose(const unary_compose<G1, G2>& that)
    {}

    /**
     * \brief Return (F1 o F2)(a).
     */
    typename F1::result_type operator()(typename F2::argument_type& a) const
    {
      return F1()(F2()(a));
    }
  }; // class unary_compose

  /**
   * \brief Function object that deletes a pointer.
   *
   * \b Template \b arguments:
   * - \a T The pointer type.
   *
   * \remark Use this function object to clear your container of pointers.
   * \author Julien Jorge
   */
  template <typename T>
  class delete_function
  {
  public:
    using argument_type = const T&;
    using result_type = void;

  public:
    void operator()(const T& a) const
    {
      delete a;
    }
  }; // class delete_function

  /**
   * \brief Function object that clones a pointer.
   *
   * \b Template \b arguments:
   * - \a T The pointed type (ie. <tt>int</tt>, not <tt>int*</tt>.
   *
   * \remark Use this function object to copy your container of pointers.
   * \author Julien Jorge
   */
  template <typename T>
  class clone
  {
  public:
    using argument_type = const T*;
    using result_type = T*;

  public:
    T* operator()(const T* a) const
    {
      return new T(*a);
    }
  }; // class clone

  /**
   * \brief Function object that dereferences a pointer.
   *
   * \b Template \b arguments:
   * - \a T The pointed type (ie. <tt>int</tt>, not <tt>int*</tt>.
   *
   * \author Julien Jorge
   */
  template <typename T>
  class dereference
  {
  public:
    using argument_type = T*;
    using result_type = T&;

  public:
    T& operator()(T* a) const
    {
      return *a;
    }

  }; // class dereference

  /**
   * \brief Function object that dereferences a constant pointer.
   *
   * \b Template \b arguments:
   * - \a T The pointed type (ie. <tt>int</tt>, not <tt>int*</tt>.
   *
   * \author Julien Jorge
   */
  template <typename T>
  class const_dereference
  {
  public:
    using argument_type = const T*;
    using result_type = const T&;

  public:
    const_dereference()
    {}
    const_dereference(const dereference<T>&)
    {}
    const_dereference(const const_dereference<T>&)
    {}

    const T& operator()(const T* a) const
    {
      return *a;
    }

  }; // class const_dereference
}

#endif // __CLAW_FUNCTIONAL_HPP__
