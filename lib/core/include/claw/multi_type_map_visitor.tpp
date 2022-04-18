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
 * \file multi_type_map_visitor.tpp
 * \brief Implementation of the claw::multi_type_map_visitor class.
 * \author Julien Jorge
 */

namespace claw
{
  /**
   * \brief This class goes through all entries of a given type in a
   *        multi_type_map and apply a function to them.
   * \author Julien Jorge
   */
  template <typename Type>
  class multi_type_map_visitor_process
  {
  public:
    template <typename Key, typename TailType, typename Function>
    void
    execute(multi_type_map<Key, claw::meta::type_list<Type, TailType> >& m,
            Function f)
    {
      typedef claw::meta::type_list<Type, TailType> type_list_type;
      typedef multi_type_map<Key, type_list_type> map_type;
      typedef typename map_type::template iterator<Type>::type iterator_type;

      iterator_type it(m.template begin<Type>());
      const iterator_type eit(m.template end<Type>());

      while(it != eit)
        {
          iterator_type current(it);
          ++it;
          f(current->first, current->second);
        }
    }

  }; // class multi_type_map_visitor_process

  /**
   * \brief This class goes through all entries in a multi_type_map and apply a
   *        function to them.
   * \author Julien Jorge
   */
  template <typename Key, typename TypeList>
  class multi_type_map_visitor_rec;

  /**
   * \brief Specialization of multi_type_map_visitor_rec for an empty type
   * list. \author Julien Jorge
   */
  template <typename Key>
  class multi_type_map_visitor_rec<Key, claw::meta::no_type>
  {
  public:
    template <typename Function>
    void execute(multi_type_map<Key, claw::meta::no_type>& vars, Function f)
    {
      // nothing to do.
    }

  }; // class multi_type_map_visitor

  /**
   * \brief Specialization of multi_type_map_visitor_rec for a non empty type
   *        list.
   * \author Julien Jorge
   */
  template <typename KeyType, typename HeadType, typename TailType>
  class multi_type_map_visitor_rec<KeyType,
                                   claw::meta::type_list<HeadType, TailType> >
  {
  public:
    template <typename Function>
    void execute(
        multi_type_map<KeyType, claw::meta::type_list<HeadType, TailType> >& m,
        Function f)
    {
      multi_type_map_visitor_process<HeadType> process;
      multi_type_map_visitor_rec<KeyType, TailType> rec_call;

      process.execute(m, f);
      rec_call.execute(m, f);
    }

  }; // class multi_type_map_visitor_rec

}

/**
 * \brief Execute the visitor.
 * \param m The map to visit.
 * \param f The function to apply to the entries.
 */
template <typename Key, typename TypeList, typename Function>
void claw::multi_type_map_visitor::run(multi_type_map<Key, TypeList>& m,
                                       Function f) const
{
  multi_type_map_visitor_rec<Key, TypeList> rec_call;
  rec_call.execute(m, f);
}
