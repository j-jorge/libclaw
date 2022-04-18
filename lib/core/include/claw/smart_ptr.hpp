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
 * \file smart_ptr.hpp
 * \brief A pointer with a reference counter.
 * \author Julien Jorge
 */

#ifndef __CLAW_SMART_PTR_HPP__
#define __CLAW_SMART_PTR_HPP__

namespace claw
{
  namespace memory
  {
    /**
     * \brief A pointer with a reference counter.
     *
     * Smart pointers allow the user to stop caring about the release of
     * dynamically allocated memory. When no more pointers point to the
     * allocated memory, this memory is released.
     *
     * \b Template \b parameters:
     * - \a T The type of the pointed data.
     *
     * \author Julien Jorge
     */
    template <typename T>
    class smart_ptr
    {
    public:
      /** \brief The type of the pointed data. */
      typedef T value_type;

      /** \brief The type of the current class. */
      typedef smart_ptr<value_type> self_type;

      /** \brief Reference on the type of the stored data. */
      typedef T& reference;

      /** \brief Pointer on the type of the stored data. */
      typedef T* pointer;

      /** \brief Constant reference on the type of the stored data. */
      typedef const T& const_reference;

      /** \brief Constant pointer on the type of the stored data. */
      typedef const T* const const_pointer;

    public:
      smart_ptr();
      smart_ptr(pointer data);
      smart_ptr(const self_type& that);
      ~smart_ptr();

      self_type& operator=(const self_type& that);

      bool operator==(const self_type& that) const;
      bool operator!=(const self_type& that) const;
      bool operator<(const self_type& that) const;
      bool operator<=(const self_type& that) const;
      bool operator>(const self_type& that) const;
      bool operator>=(const self_type& that) const;

      pointer operator->();
      pointer operator->() const;
      reference operator*();
      reference operator*() const;

    private:
      void copy(const self_type& that);
      void release();

    private:
      /** \brief Number of smart_ptr pointing on this memory area. */
      unsigned int* m_ref_count;

      /** \brief The pointed item. */
      pointer m_ptr;

    }; // class smart_ptr
  }
}

#include <claw/smart_ptr.tpp>

#endif // __CLAW_SMART_PTR_HPP__
