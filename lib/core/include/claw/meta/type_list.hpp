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
 * \file type_list.hpp
 * \brief Template classes for lists of types.
 * \author Julien Jorge
 */
#ifndef __CLAW_TYPE_LIST_HPP__
#define __CLAW_TYPE_LIST_HPP__

#include <claw/meta/conditional.hpp>
#include <claw/meta/no_type.hpp>
#include <claw/meta/same_type.hpp>

namespace claw
{
  namespace meta
  {
    /**
     * \brief Use this class to make a list of types.
     *
     * \b Template \b parameters
     * - \a Head The first type of the list.
     * - \a Queue A list of the remaining types.
     *
     * All type_list methods suppose that the list ends withe the type no_type.
     *
     * \b Example
     *
     * <tt> type_list< int, typelist<float, no_type> ></tt> is a list of two
     * types. The last type (<tt>no_type</tt>) is used to indicate the end of
     * the list.
     *
     * \author Julien Jorge
     */
    template <typename Head, typename Queue>
    struct type_list
    {
      /** \brief The first type in the list. */
      typedef Head head_type;

      /** \brief The remaining types in the list. */
      typedef Queue queue_type;

    }; // struct type_list

    /**
     * \brief Split a type_list according to the first instance of a given
     * type.
     *
     * \b Template \b parameters
     * - \a Delimiter The type on which we split the list
     * - \a TypeList The type_list to split.
     */
    template <typename Delimiter, typename TypeList>
    struct split_type_list_at;

    /** \brief Specialisation of split_type_list_at for an empty list or the
        case where the delimiter is no_type. */
    template <typename Delimiter>
    struct split_type_list_at<Delimiter, no_type>
    {
      /** \brief There is no left part if there is no list. */
      typedef no_type left_part_type;

      /** \brief There is no right part if there is no list. */
      typedef no_type right_part_type;

    }; // struct split_type_list_at

    /**
     * \brief Split a type_list according to the first instance of a given
     * type.
     *
     * \b Template \b parameters
     * - \a Delimiter The type on which we split the list
     * - \a TypeList The type_list to split.
     */
    template <typename Delimiter, typename TypeList>
    struct split_type_list_at
    {
      /** \brief The left part of the list. The delimiter is not included. */
      typedef typename if_then_else<
          same_type<Delimiter, typename TypeList::head_type>::result,
          no_type,  /* delimiter is found, mark the end of the list. */
          type_list /* otherwise, cut in the remaining types. */
          <typename TypeList::head_type,
           typename split_type_list_at<
               Delimiter, typename TypeList::queue_type>::left_part_type> >::
          result left_part_type;

      /** \brief The right part of the list. The delimiter is included. */
      typedef typename if_then_else<
          same_type<Delimiter, typename TypeList::head_type>::result,
          TypeList, /* delimiter is found, this is the right part. */
          typename split_type_list_at<
              Delimiter, typename TypeList::queue_type>::right_part_type>::
          result right_part_type;

    }; // struct split_type_list_at

    /**
     * \brief A type list with a single type, more readable than an imbricated
     *        type_list.
     */
    template <typename T1>
    struct type_list_maker_1
    {
      /** \brief The list described by this implementation. */
      typedef type_list<T1, no_type> result;
    }; // struct type_list_maker_1

    /**
     * \brief A type list with two types, more readable than an imbricated
     *        type_list.
     */
    template <typename T1, typename T2>
    struct type_list_maker_2
    {
      /** \brief The list described by this implementation. */
      typedef type_list<T1, typename type_list_maker_1<T2>::result> result;
    }; // struct type_list_maker_2

    /**
     * \brief A type list with three types, more readable than an imbricated
     *        type_list.
     */
    template <typename T1, typename T2, typename T3>
    struct type_list_maker_3
    {
      /** \brief The list described by this implementation. */
      typedef type_list<T1, typename type_list_maker_2<T2, T3>::result> result;
    }; // struct type_list_maker_3

    /**
     * \brief A type list with four types, more readable than an imbricated
     *        type_list.
     */
    template <typename T1, typename T2, typename T3, typename T4>
    struct type_list_maker_4
    {
      /** \brief The list described by this implementation. */
      typedef type_list<T1, typename type_list_maker_3<T2, T3, T4>::result>
          result;
    }; // struct type_list_maker_4

    /**
     * \brief A type list with five types, more readable than an imbricated
     *        type_list.
     */
    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    struct type_list_maker_5
    {
      /** \brief The list described by this implementation. */
      typedef type_list<T1, typename type_list_maker_4<T2, T3, T4, T5>::result>
          result;
    }; // struct type_list_maker_5

    /**
     * \brief A type list with six types, more readable than an imbricated
     *        type_list.
     */
    template <typename T1, typename T2, typename T3, typename T4, typename T5,
              typename T6>
    struct type_list_maker_6
    {
      /** \brief The list described by this implementation. */
      typedef type_list<T1,
                        typename type_list_maker_5<T2, T3, T4, T5, T6>::result>
          result;
    }; // struct type_list_maker_6

    /**
     * \brief A type list with seven types, more readable than an imbricated
     *        type_list.
     */
    template <typename T1, typename T2, typename T3, typename T4, typename T5,
              typename T6, typename T7>
    struct type_list_maker_7
    {
      /** \brief The list described by this implementation. */
      typedef type_list<
          T1, typename type_list_maker_6<T2, T3, T4, T5, T6, T7>::result>
          result;
    }; // struct type_list_maker_7

    /**
     * \brief A type list with height types, more readable than an imbricated
     *        type_list.
     */
    template <typename T1, typename T2, typename T3, typename T4, typename T5,
              typename T6, typename T7, typename T8>
    struct type_list_maker_8
    {
      /** \brief The list described by this implementation. */
      typedef type_list<
          T1, typename type_list_maker_7<T2, T3, T4, T5, T6, T7, T8>::result>
          result;
    }; // struct type_list_maker_8

    /**
     * \brief A type list with nine types, more readable than an imbricated
     *        type_list.
     */
    template <typename T1, typename T2, typename T3, typename T4, typename T5,
              typename T6, typename T7, typename T8, typename T9>
    struct type_list_maker_9
    {
      /** \brief The list described by this implementation. */
      typedef type_list<T1, typename type_list_maker_8<T2, T3, T4, T5, T6, T7,
                                                       T8, T9>::result>
          result;
    }; // struct type_list_maker_9

    /**
     * \brief A type list with ten types, more readable than an imbricated
     *        type_list.
     */
    template <typename T1, typename T2, typename T3, typename T4, typename T5,
              typename T6, typename T7, typename T8, typename T9, typename T10>
    struct type_list_maker_10
    {
      /** \brief The list described by this implementation. */
      typedef type_list<T1, typename type_list_maker_9<T2, T3, T4, T5, T6, T7,
                                                       T8, T9, T10>::result>
          result;
    }; // struct type_list_maker_10

    /**
     * \brief A type list with eleven types, more readable than an imbricated
     *        type_list.
     */
    template <typename T1, typename T2, typename T3, typename T4, typename T5,
              typename T6, typename T7, typename T8, typename T9, typename T10,
              typename T11>
    struct type_list_maker_11
    {
      /** \brief The list described by this implementation. */
      typedef type_list<T1,
                        typename type_list_maker_10<T2, T3, T4, T5, T6, T7, T8,
                                                    T9, T10, T11>::result>
          result;
    }; // struct type_list_maker_11

    /**
     * \brief A type list with twelve types, more readable than an imbricated
     *        type_list.
     */
    template <typename T1, typename T2, typename T3, typename T4, typename T5,
              typename T6, typename T7, typename T8, typename T9, typename T10,
              typename T11, typename T12>
    struct type_list_maker_12
    {
      /** \brief The list described by this implementation. */
      typedef type_list<T1,
                        typename type_list_maker_11<T2, T3, T4, T5, T6, T7, T8,
                                                    T9, T10, T11, T12>::result>
          result;
    }; // struct type_list_maker_12

    /**
     * \brief A type list with thirteen types, more readable than an imbricated
     *        type_list.
     */
    template <typename T1, typename T2, typename T3, typename T4, typename T5,
              typename T6, typename T7, typename T8, typename T9, typename T10,
              typename T11, typename T12, typename T13>
    struct type_list_maker_13
    {
      /** \brief The list described by this implementation. */
      typedef type_list<
          T1, typename type_list_maker_12<T2, T3, T4, T5, T6, T7, T8, T9, T10,
                                          T11, T12, T13>::result>
          result;
    }; // struct type_list_maker_13

    /**
     * \brief A type list with fourteen types, more readable than an imbricated
     *        type_list.
     */
    template <typename T1, typename T2, typename T3, typename T4, typename T5,
              typename T6, typename T7, typename T8, typename T9, typename T10,
              typename T11, typename T12, typename T13, typename T14>
    struct type_list_maker_14
    {
      /** \brief The list described by this implementation. */
      typedef type_list<
          T1, typename type_list_maker_13<T2, T3, T4, T5, T6, T7, T8, T9, T10,
                                          T11, T12, T13, T14>::result>
          result;
    }; // struct type_list_maker_14

    /**
     * \brief A type list with fifteen types, more readable than an imbricated
     *        type_list.
     */
    template <typename T1, typename T2, typename T3, typename T4, typename T5,
              typename T6, typename T7, typename T8, typename T9, typename T10,
              typename T11, typename T12, typename T13, typename T14,
              typename T15>
    struct type_list_maker_15
    {
      /** \brief The list described by this implementation. */
      typedef type_list<
          T1, typename type_list_maker_14<T2, T3, T4, T5, T6, T7, T8, T9, T10,
                                          T11, T12, T13, T14, T15>::result>
          result;
    }; // struct type_list_maker_15

    /**
     * \brief A type list with up to six types, more readable than an
     *        imbricated type_list, more readable than type_list_maker_1 and
     *        others.
     */
    template <
        typename T1, typename T2 = no_type, typename T3 = no_type,
        typename T4 = no_type, typename T5 = no_type, typename T6 = no_type,
        typename T7 = no_type, typename T8 = no_type, typename T9 = no_type,
        typename T10 = no_type, typename T11 = no_type, typename T12 = no_type,
        typename T13 = no_type, typename T14 = no_type, typename T15 = no_type>
    struct type_list_maker
    {
      /** \brief The list described by this implementation. */
      typedef typename split_type_list_at<
          no_type, typename type_list_maker_15<
                       T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
                       T14, T15>::result>::left_part_type result;
    }; // struct type_list_maker

    /**
     * \brief Check if a type is present in a type_list.
     *
     * \b Template \b paramters
     * - \a T The type to find.
     * - \a List The list in which we search the type.
     *
     * \b Type \b requirements
     * - \a List is either no_type, either a type_list<> ended with no_type.
     *
     * \author Julien Jorge
     */
    template <typename T, typename List>
    struct type_list_find
    {
      enum
      {
        result = same_type < T,
        typename List::head_type > ::result || type_list_find < T,
        typename List::queue_type > ::result
      };
    }; // struct type_list_find

    template <typename T>
    struct type_list_find<T, no_type>
    {
      enum
      {
        result = same_type < T,
        no_type > ::result
      };
    }; // struct type_list_find

    /**
     * \brief This class checks if each type in a type_list<> is present only
     *        once.
     *
     * \author Julien Jorge
     */
    template <typename List>
    struct type_list_is_a_set
    {
      enum
      {
        result = !type_list_find < typename List::head_type,
        typename List::queue_type
            > ::result&& type_list_is_a_set<typename List::queue_type>::result
      };
    }; // struct type_list_is_a_set

    template <>
    struct type_list_is_a_set<no_type>
    {
      enum
      {
        result = true
      };
    }; // struct type_list_is_a_set [no_type]

    /**
     * \brief This class computes the length of a list of types.
     *
     * \author Julien Jorge
     */
    template <typename List>
    struct type_list_length
    {
      enum
      {
        result = 1 + type_list_length<typename List::queue_type>::result
      };
    }; // struct type_list_length

    template <>
    struct type_list_length<no_type>
    {
      enum
      {
        result = 0
      };
    }; // struct type_list_length [no_type]

    /**
     * \brief Tell if a given type list contains a given type.
     * \param T The type to search.
     * \param List The list in which the type is searched.
     *
     * \author Julien Jorge
     */
    template <typename T, typename List>
    struct type_list_contains;

    template <typename T, typename Tail>
    struct type_list_contains<T, type_list<T, Tail> >
    {
      enum
      {
        result = true
      };
    }; // struct type_list_contains

    template <typename T>
    struct type_list_contains<T, no_type>
    {
      enum
      {
        result = false
      };
    }; // struct type_list_contains

    template <typename T, typename Head, typename Tail>
    struct type_list_contains<T, type_list<Head, Tail> >
    {
      enum
      {
        result = type_list_contains < T,
        Tail > ::result
      };
    }; // struct type_list_contains

    /** \brief The list of the types of the C++ language. */
    typedef type_list_maker<
        signed char, unsigned char, signed short, unsigned short, signed int,
        unsigned int, signed long, unsigned long,
#ifndef __STRICT_ANSI__
        signed long long, unsigned long long,
#endif
        float, double, long double, bool>::result cpp_type_list;

  }
}

#endif // __CLAW_TYPE_LIST_HPP__
