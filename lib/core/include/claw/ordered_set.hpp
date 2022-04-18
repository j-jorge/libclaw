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
 * \file ordered_set.hpp
 * \brief A class to manage sets of ordered items.
 * \author Julien Jorge
 */
#ifndef __CLAW_ORDERED_SET_HPP__
#define __CLAW_ORDERED_SET_HPP__

#include <claw/avl.hpp>

namespace claw
{
  namespace math
  {
    /**
     * \brief A class to manage sets of ordered items.
     * \author Julien Jorge
     */
    template <class K, class Comp = std::less<K> >
    class ordered_set : public avl<K, Comp>
    {
    private:
      /** \brief The type of the parent class. */
      typedef avl<K, Comp> super;

    public:
      /** \brief The type of the iterator used to access non modifiable
          values. */
      typedef typename super::const_iterator const_iterator;

      /** \brief The type of the values in the set. */
      typedef typename super::value_type value_type;

      /** \brief The type passed to the template. */
      typedef typename super::referent_type referent_type;

      /** \brief The type of a const reference on the values. */
      typedef typename super::const_reference const_reference;

    public:
      ordered_set& operator*=(const ordered_set& that);
      ordered_set& operator+=(const ordered_set& that);
      ordered_set& operator-=(const ordered_set& that);
      ordered_set& operator/=(const ordered_set& that);

      bool operator>(const ordered_set& that) const;
      bool operator>=(const ordered_set& that) const;
      bool operator<(const ordered_set& that) const;
      bool operator<=(const ordered_set& that) const;

      ordered_set& intersection(const ordered_set& that);
      ordered_set& join(const ordered_set& that);
      ordered_set& difference(const ordered_set& that);
      ordered_set& symetric_difference(const ordered_set& that);

      bool contains(const ordered_set& that) const;
      bool strictly_contains(const ordered_set& that) const;

    private:
      /** \brief Function object used to compare keys. */
      static Comp s_key_comp;

    }; // class ordered_set
  }
}

#include <claw/ordered_set.tpp>

#endif // __CLAW_ORDERED_SET_HPP__
