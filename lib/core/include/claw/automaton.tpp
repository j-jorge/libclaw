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
 * \file automaton.tpp
 * \brief Implementation of the claw::automaton class.
 * \author Julien Jorge
 */
#include <algorithm>
#include <claw/assert.hpp>
#include <claw/functional.hpp>

//***************************** automate **************************************

template <class State, class Edge, class StateComp, class EdgeComp>
typename claw::automaton<State, Edge, StateComp, EdgeComp>::state_compare
    claw::automaton<State, Edge, StateComp, EdgeComp>::s_state_compare;

/**
 * \brief Add an edge in the automaton.
 * \param s1 Source state.
 * \param s2 Target state.
 * \param e The label on the edge.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
void claw::automaton<State, Edge, StateComp, EdgeComp>::add_edge(
    const state_type& s1, const state_type& s2, const edge_type& e)
{
  add_state(s1);
  add_state(s2);

  m_states[s1].insert(typename neighbours_list::value_type(e, s2));
  m_alphabet.insert(e);
}

/**
 * \brief Remove an edge from the atomaton.
 * \param s1 Source state.
 * \param s2 Target state.
 * \param e The label on the edge.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
void claw::automaton<State, Edge, StateComp, EdgeComp>::remove_edge(
    const state_type& s1, const state_type& s2, const edge_type& e)
{
  typename neighbours_list::iterator it = m_states[s1].lower_bound(e);
  bool ok = false;

  while((it != m_states[s1].upper_bound(e)) && !ok)
    if(it->second == s2)
      ok = true;
    else
      ++it;

  if(ok)
    m_states[s1].erase(it);
}

/**
 * \brief Add a state in the automaton.
 * \param s The state to add.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
void claw::automaton<State, Edge, StateComp, EdgeComp>::add_state(
    const state_type& s)
{
  std::pair<state_type, neighbours_list> p;

  if(m_states.find(s) == m_states.end())
    {
      p.first = s;
      m_states.insert(p);
    }
}

/**
 * \brief Add an initial state.
 * \param s The state to add.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
void claw::automaton<State, Edge, StateComp, EdgeComp>::add_initial_state(
    const state_type& s)
{
  add_state(s);
  m_initial_states.insert(s);
}

/**
 * \brief Add a final state.
 * \param s The state to add.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
void claw::automaton<State, Edge, StateComp, EdgeComp>::add_final_state(
    const state_type& s)
{
  add_state(s);
  m_final_states.insert(s);
}

/**
 * \brief Tell of the automaton contains a given state.
 * \param s The state to check.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
bool claw::automaton<State, Edge, StateComp, EdgeComp>::state_exists(
    const state_type& s) const
{
  return (m_states.find(s) != m_states.end());
}

/**
 * \brief Tell if a state is final.
 * \param s The state to check.
 * \pre state_exists(s) == true
 */
template <class State, class Edge, class StateComp, class EdgeComp>
bool claw::automaton<State, Edge, StateComp, EdgeComp>::state_is_final(
    const state_type& s) const
{
  CLAW_PRECOND(state_exists(s));

  return m_final_states.find(s) != m_final_states.end();
}

/**
 * \brief Tell if a state is an initial state.
 * \param s The state to check.
 * \pre state_exists(s) == true
 */
template <class State, class Edge, class StateComp, class EdgeComp>
bool claw::automaton<State, Edge, StateComp, EdgeComp>::state_is_initial(
    const state_type& s) const
{
  CLAW_PRECOND(state_exists(s));

  return m_initial_states.find(s) != m_initial_states.end();
}

/**
 * \brief Get the states in the automaton.
 * \param v (out) The container in which to add the states.
 * \todo Remove this method and add iterator types.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
void claw::automaton<State, Edge, StateComp, EdgeComp>::states(
    result_state_list& v) const
{
  v.clear();
  v.resize(m_states.size());
  std::transform(m_states.begin(), m_states.end(), v.begin(),
                 const_first<state_type, neighbours_list>());
}

/**
 * \brief Get the final states.
 * \param v (out) The container in which to add the states.
 * \todo Remove this method and add iterator types.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
void claw::automaton<State, Edge, StateComp, EdgeComp>::final_states(
    result_state_list& v) const
{
  v.clear();
  v.resize(m_final_states.size());
  std::copy(m_final_states.begin(), m_final_states.end(), v.begin());
}

/**
 * \brief Get the final states.
 * \param v (out) The container in which to add the states.
 * \todo Remove this method and add iterator types.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
void claw::automaton<State, Edge, StateComp, EdgeComp>::initial_states(
    result_state_list& v) const
{
  v.clear();
  v.resize(m_initial_states.size());
  std::copy(m_initial_states.begin(), m_initial_states.end(), v.begin());
}

/**
 * \brief Get all symbols in the alphabet.
 * \param v (out) The container in which to add the symbols
 * \todo Remove this method and add iterator types.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
void claw::automaton<State, Edge, StateComp, EdgeComp>::alphabet(
    result_edge_list& v) const
{
  v.clear();
  v.resize(m_alphabet.size());
  std::copy(m_alphabet.begin(), m_alphabet.end(), v.begin());
}

/**
 * \brief Tell if the automaton recognizes a given pattern.
 * \param first Iterator on the first symbol in the pattern.
 * \param last Iterator after the last symbol in the pattern.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
template <class InputIterator>
bool claw::automaton<State, Edge, StateComp, EdgeComp>::match(
    InputIterator first, InputIterator last) const
{
  bool ok = false;
  typename claw::avl<state_type>::const_iterator it;

  for(it = m_initial_states.begin(); (it != m_initial_states.end()) && !ok;
      ++it)
    ok = match_aux(*it, first, last);

  return ok;
}

/**
 * \brief Get the number of states.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
unsigned int
claw::automaton<State, Edge, StateComp, EdgeComp>::states_count() const
{
  return m_states.size();
}

/**
 * \brief Get the states that can be reached from a given state with a given
 *        symbol.
 * \param s Initial state.
 * \param e The symbol on the edge.
 * \param l (out) The list of reachable states.
 * \pre state_exists(s) == true
 * \todo Remove this method and add iterator types.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
void claw::automaton<State, Edge, StateComp, EdgeComp>::reachables(
    const state_type& s, const edge_type& e, result_state_list& l) const
{
  CLAW_PRECOND(state_exists(s));

  typename adjacent_list::const_iterator it = m_states.find(s);

  l.clear();
  l.resize(it->second.count(e));

  std::transform(it->second.lower_bound(e), it->second.upper_bound(e),
                 l.begin(), claw::second<edge_type, state_type>());
}

/**
 * \brief Get the states that can be reached from a given state, no matter the
 *        symbol.
 * \param s Initial state.
 * \param l (out) The list of reachable states.
 * \pre state_exists(s) == true
 * \todo Remove this method and add iterator types.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
void claw::automaton<State, Edge, StateComp, EdgeComp>::reachables(
    const state_type& s, result_state_list& l) const
{
  CLAW_PRECOND(state_exists(s));

  typename adjacent_list::const_iterator it_s = m_states.find(s);
  typename neighbours_list::const_iterator it;
  claw::avl<state_type, state_compare> reachable_states;

  for(it = it_s->second.begin(); it != it_s->second.end(); ++it)
    reachable_states.insert(it->second);

  l.clear();
  l.resize(reachable_states.size());

  std::copy(reachable_states.begin(), reachable_states.end(), l.begin());
}

/**
 * \brief Get all the edges between two states.
 * \param s1 Source state.
 * \param s2 Target state.
 * \param l (out) The list of edges.
 * \pre (state_exists(s1) == true) && (state_exists(s2) == true)
 * \todo Remove this method and add iterator types.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
void claw::automaton<State, Edge, StateComp, EdgeComp>::edges(
    const state_type& s1, const state_type& s2, result_edge_list& l) const
{
  CLAW_PRECOND(state_exists(s1));
  CLAW_PRECOND(state_exists(s2));

  typename adjacent_list::const_iterator it_s = m_states.find(s1);
  typename neighbours_list::const_iterator it;

  l.clear();
  l.reserve(it_s->second.size()); // pessimistic

  for(it = it_s->second.begin(); it != it_s->second.end(); ++it)
    if(!(s_state_compare(it->second, s2) || s_state_compare(s2, it->second)))
      l.push_back(it->first);
}

/**
 * \brief Get all out-edges of a given state, labeled with a given symbol.
 * \param s1 The source state of the edges.
 * \param e The symbol on the edges.
 * \param l (out) The list of edges.
 * \pre state_exists(s1) == true
 * \todo Remove this method and add iterator types.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
void claw::automaton<State, Edge, StateComp, EdgeComp>::edges(
    const state_type& s1, const edge_type& e, result_edge_list& l) const
{
  CLAW_PRECOND(state_exists(s1));

  typename adjacent_list::const_iterator it_s = m_states.find(s1);

  l.clear();
  l.resize(it_s->second.count(e));

  std::transform(it_s->second.lower_bound(e), it_s->second.upper_bound(e),
                 l.begin(), claw::first<edge_type, state_type>());
}

/*================================== private
 * =================================*/

/**
 * \brief Recognize a pattern (recursive and auxiliary method).
 * \param s The state on which we start the search.
 * \param first Iterator on the first symbol to recognize.
 * \param last Iterator past the last symbol to recognize.
 */
template <class State, class Edge, class StateComp, class EdgeComp>
template <class InputIterator>
bool claw::automaton<State, Edge, StateComp, EdgeComp>::match_aux(
    const state_type& s, InputIterator first, InputIterator last) const
{
  CLAW_PRECOND(state_exists(s));

  bool ok = false;

  if(first == last)
    ok = state_is_final(s);
  else
    {
      typename neighbours_list::const_iterator candidate, last_candidate;
      InputIterator next_symbol = first;
      ++next_symbol;

      candidate = m_states.find(s)->second.lower_bound(*first);
      last_candidate = m_states.find(s)->second.upper_bound(*first);

      for(; (candidate != last_candidate) && !ok; ++candidate)
        ok = match_aux(candidate->second, next_symbol, last);
    }

  return ok;
}
