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
 * \file max_vector.tpp
 * \brief Implementation of the claw::max_vector class.
 * \author Julien Jorge
 */

/**
 * \brief Constructor.
 */
template <typename E, typename Comp, typename Container>
claw::max_vector<E, Comp, Container>::max_vector()
{}

/**
 * \brief Constructor with an initial value.
 * \param e élément ajouté.
 */
template <typename E, typename Comp, typename Container>
claw::max_vector<E, Comp, Container>::max_vector(const value_type& e)
{
  add(e);
}

/**
 * \brief Insert a value in the vector.
 * \param e The value to insert.
 */
template <typename E, typename Comp, typename Container>
void claw::max_vector<E, Comp, Container>::add(const value_type& e)
{
  if(m_values.empty())
    *std::inserter(m_values, m_values.end()) = e;
  else
    {
      comparator_type comp;
      if(comp(m_values.front(), e))
        {
          m_values.clear();
          *std::inserter(m_values, m_values.end()) = e;
        }
      else if(!comp(e, m_values.front()))
        *std::inserter(m_values, m_values.end()) = e;
    }
}

/**
 * \brief Get the stored values.
 */
template <typename E, typename Comp, typename Container>
const typename claw::max_vector<E, Comp, Container>::container_type&
claw::max_vector<E, Comp, Container>::get_v() const
{
  return m_values;
}
