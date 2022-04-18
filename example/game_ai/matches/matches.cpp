/**
 * \file matches.cpp
 * \brief Implementation of the state_matches class.
 * \author Julien Jorge
 */
#include "matches.hpp"
#include <iostream>

template <>
const int claw::ai::game::game_state<int>::s_min_score = -100;
template <>
const int claw::ai::game::game_state<int>::s_max_score = 100;

/**
 * \brief Output a state as text.
 * \param os The stream in which we write.
 * \param e The state to print.
 * \return os.
 */
std::ostream& operator<<(std::ostream& os, const state_matches& e)
{
  int i;

  for(i = 0; i != e.m_matches; ++i)
    {
      os << "|";
      if(i % 3 == 2)
        os << " ";
    }

  os << std::endl;

  return os;
}

/**
 * \brief Constructor.
 * \param matches Initial count of matches.
 * \param computer Tell if the computer starts to play.
 */
state_matches::state_matches(int matches, bool computer)
  : m_computer(computer)
  , m_matches(matches)
{}

/**
 * \brief Remove some matches and change the player.
 * \param c The count of matches to remove.
 * \return true if the action ends the game.
 */
bool state_matches::play(int c)
{
  m_matches -= std::min(m_matches, c);
  m_computer = !m_computer;

  return m_matches == 0;
}

/**
 * \brief Get a new state obtained when applying an action.
 * \param a The action to apply.
 * \return The state resulting from this action.
 */
state_matches::state* state_matches::do_action(const int& a) const
{
  state_matches* em = new state_matches(*this);

  em->play(a);

  return em;
}

/**
 * \brief Tell if the game is over.
 */
bool state_matches::final() const
{
  return m_matches == 0;
}

/**
 * \brief Get all actions that can be done from this state.
 * \param l (out) The actions.
 */
void state_matches::next_actions(std::list<int>& l) const
{
  for(int i = 0; i != 3; ++i)
    if(m_matches > i)
      l.push_back(i + 1);
}

/**
 * \brief Evaluate this state of the game.
 */
int state_matches::evaluate() const
{
  int score = 0;

  // no more matches.
  if(m_matches == 0)
    {
      // Its the computer's turn, so the player took the last match. Good.
      if(m_computer)
        score = 100;
      else // otherwise, the computer losts.
        score = -100;
    }

  return score;
}
