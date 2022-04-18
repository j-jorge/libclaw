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
 * \file automaton.hpp
 * \brief A basic automaton structure.
 * \author Julien Jorge
 */
#ifndef __CLAW_AUTOMATON_HPP__
#define __CLAW_AUTOMATON_HPP__

#include <claw/avl.hpp>
#include <map>
#include <vector>

namespace claw
{
  //***************************** automate ************************************

  /**
   * \brief Basic automaton structure.
   *
   * An automaton is a quintuplet (A, E, I ,F, T) where
   * - A is the set of symbols,
   * - E is the set of states,
   * - I is the set of initial states,
   * - F is the set of finalstates,
   * - T is the set of transitions (labeled edges)
   *
   * \b Template \b parameters
   * - \a State The type of the states.
   * - \a Edge The type of the symbols in the alphabet.
   */
  template <class State, class Edge, class StateComp = std::less<State>,
            class EdgeComp = std::less<Edge> >
  class automaton
  {
  public:
    /** \brief The type of the states. */
    typedef State state_type;

    /** \brief The type of the symbols on the edges. */
    typedef Edge edge_type;

    /** \brief The type of the operator used to compare states. */
    typedef StateComp state_compare;

    /** \brief The type of the operator used to compare edge symbols. */
    typedef EdgeComp edge_compare;

    /** \brief The neighbours list associates states to edge symbols.*/
    typedef std::multimap<edge_type, state_type, edge_compare> neighbours_list;

    /** \brief Each state is given a set of reachable states with a neighbours
        list. */
    typedef std::map<state_type, neighbours_list, state_compare> adjacent_list;

    /** \brief The return type of the methods returning states. */
    typedef std::vector<state_type> result_state_list;

    /** \brief The return type of the methods returning edges. */
    typedef std::vector<edge_type> result_edge_list;

  public:
    void add_edge(const state_type& s1, const state_type& s2,
                  const edge_type& e);
    void remove_edge(const state_type& s1, const state_type& s2,
                     const edge_type& e);

    void add_state(const state_type& s);
    void add_initial_state(const state_type& s);
    void add_final_state(const state_type& s);

    bool state_exists(const state_type& s) const;
    bool state_is_final(const state_type& s) const;
    bool state_is_initial(const state_type& s) const;

    void states(result_state_list& v) const;
    void final_states(result_state_list& v) const;
    void initial_states(result_state_list& v) const;
    void alphabet(result_edge_list& v) const;

    template <class InputIterator>
    bool match(InputIterator first, InputIterator last) const;

    unsigned int states_count() const;

    void reachables(const state_type& s, const edge_type& e,
                    result_state_list& l) const;
    void reachables(const state_type& s, result_state_list& l) const;

    void edges(const state_type& s1, const state_type& s2,
               result_edge_list& l) const;
    void edges(const state_type& s1, const edge_type& edge,
               result_edge_list& l) const;

  private:
    template <class InputIterator>
    bool match_aux(const state_type& s, InputIterator first,
                   InputIterator last) const;

  private:
    /** \brief The predicate used to compare states. */
    static state_compare s_state_compare;

    /** \brief The set of symbols in the alphabet. */
    avl<edge_type, edge_compare> m_alphabet;

    /** \brief The set of initial states. */
    avl<state_type, state_compare> m_initial_states;

    /** \brief The set of final states. */
    avl<state_type, state_compare> m_final_states;

    /** \brief The adjacency list (the set of transitions). */
    adjacent_list m_states;
  }; // automaton

}

#include <claw/automaton.tpp>

#endif // __CLAW_AUTOMATON_HPP__
