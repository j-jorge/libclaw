/*
  CLAW - a C++ Library Absolutely Wonderful

  CLAW is a free library without any particular aim but being useful to
  anyone.

  Copyright (C) 2005 Sébastien Angibaud
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
 * \file game_ai.tpp
 * \brief Implémentation de fonctions d'intelligence artificielle.
 * \author Julien Jorge & Sébastien Angibaud
 */
#include <claw/max_vector.hpp>

#include <cstdlib>

//**************************** gamestate **************************************

/**
 * \brief Destructor.
 */
template <typename Action, typename Numeric>
claw::ai::game::game_state<Action, Numeric>::~game_state()
{
  // nothing to do
}

/**
 * \brief Get the minimal score a state can get.
 */
template <typename Action, typename Numeric>
typename claw::ai::game::game_state<Action, Numeric>::score
claw::ai::game::game_state<Action, Numeric>::min_score()
{
  return s_min_score;
}

/**
 * \brief Get the maximal score a state can get.
 */
template <typename Action, typename Numeric>
typename claw::ai::game::game_state<Action, Numeric>::score
claw::ai::game::game_state<Action, Numeric>::max_score()
{
  return s_max_score;
}

/**
 * \brief Truncate a score to fit in the range (min_score(), max_score()).
 * \param score_val The value to fit.
 */
template <typename Action, typename Numeric>
typename claw::ai::game::game_state<Action, Numeric>::score
claw::ai::game::game_state<Action, Numeric>::fit(score score_val) const
{
  if(s_max_score < score_val)
    return s_max_score;
  else if(score_val < s_min_score)
    return s_min_score;
  else
    return score_val;
}

//**************************** action_eval ************************************

/**
 * \brief Constructor.
 * \param a The evaluated action.
 * \param e The evaluation of the action.
 */
template <typename Action, typename Numeric>
claw::ai::game::action_eval<Action, Numeric>::action_eval(const Action& a,
                                                          const Numeric& e)
  : action(a)
  , eval(e)
{}

/**
 * \brief Compare with an otreh action.
 * \param ae The other action.
 */
template <typename Action, typename Numeric>
bool claw::ai::game::action_eval<Action, Numeric>::operator<(
    const action_eval& ae) const
{
  return eval < ae.eval;
}

#if 0
/**
 * \brief Egalité de deux actions.
 * \return vrai si this->eval == ae.eval.
 */
template <typename Action, typename Numeric>
bool claw::ai::game::action_eval<Action, Numeric>::operator==
  ( const action_eval& ae ) const 
{
  return eval == ae.eval; 
}
#endif

//********************************* min_max ***********************************

/**
 * \brief Apply the min-max algorithm to find the best action.
 * \param depth Depth of the search subtree we are allowed to explore.
 * \param current_state The state of the game.
 * \param computer_turn Tell if the next action is done by the computer.
 */
template <typename State>
typename claw::ai::game::min_max<State>::score
claw::ai::game::min_max<State>::operator()(int depth,
                                           const state& current_state,
                                           bool computer_turn) const
{
  score score_val;

  // we reached a final state or we are not allowed to search more.
  if(current_state.final() || (depth == 0))
    score_val = current_state.evaluate();
  else
    {
      std::list<action> next_actions;
      typename std::list<action>::const_iterator it;
      state* new_state;

      // get all reachable states
      current_state.next_actions(next_actions);

      if(next_actions.empty())
        score_val = current_state.evaluate();
      else if(computer_turn)
        {
          score_val = current_state.min_score();

          for(it = next_actions.begin(); it != next_actions.end(); ++it)
            {
              new_state = static_cast<state*>(current_state.do_action(*it));

              // evaluate the action of the human player
              score s = (*this)(depth - 1, *new_state, false);

              // and keep the best action he can do.
              if(s > score_val)
                score_val = s;

              delete new_state;
            }
        }
      else // human player's turn
        {
          score_val = current_state.max_score();

          for(it = next_actions.begin(); it != next_actions.end(); ++it)
            {
              new_state = static_cast<state*>(current_state.do_action(*it));

              // evaluate the action of the computer player
              score s = (*this)(depth - 1, *new_state, true);

              // and keep the worst action he can do
              if(s < score_val)
                score_val = s;

              delete new_state;
            }
        }
    }

  return score_val;
}

//******************************** alpha_beta *********************************

/**
 * \brief Apply the alpha-beta algorithm to find the best action.
 * \param depth Depth of the search subtree we are allowed to explore.
 * \param current_state The state of the game.
 * \param computer_turn Tell if the next action is done by the computer.
 */
template <typename State>
typename State::score claw::ai::game::alpha_beta<State>::operator()(
    int depth, const state& current_state, bool computer_turn) const
{
  return this->compute(depth, current_state, computer_turn,
                       current_state.min_score(), current_state.max_score());
}

/**
 * \brief Find the best action using an alpha-beta algorithm.
 * \param depth Depth of the search subtree we are allowed to explore.
 * \param current_state The state of the game.
 * \param computer_turn Tell if the next action is done by the computer.
 * \param alpha Worst score of the current player.
 * \param beta Best score of the other player.
 */
template <typename State>
typename claw::ai::game::alpha_beta<State>::score
claw::ai::game::alpha_beta<State>::compute(int depth,
                                           const state& current_state,
                                           bool computer_turn, score alpha,
                                           score beta) const
{
  score score_val;

  // we reached a final state or we are not allowed to search more.
  if(current_state.final() || (depth == 0))
    score_val = current_state.evaluate();
  else
    {
      std::list<action> next_actions;
      typename std::list<action>::const_iterator it;
      State* new_state;

      // get all reachable states
      current_state.next_actions(next_actions);

      if(next_actions.empty())
        score_val = current_state.evaluate();
      else if(computer_turn)
        {
          score_val = current_state.min_score();

          it = next_actions.begin();

          while(it != next_actions.end() && (score_val < beta))
            {
              new_state = static_cast<state*>(current_state.do_action(*it));

              // evaluate the action of the human player
              score s = compute(depth - 1, *new_state, false,
                                std::max(alpha, score_val), beta);

              // and keep the best action he can do.
              if(s > score_val)
                score_val = s;

              delete new_state;

              ++it;
            }
        }
      else // human player's turn
        {
          score_val = current_state.max_score();

          it = next_actions.begin();

          while(it != next_actions.end() && (score_val > alpha))
            {
              new_state = static_cast<state*>(current_state.do_action(*it));

              // evaluate the action of the computer player
              score s = compute(depth - 1, *new_state, true, alpha,
                                std::min(beta, score_val));

              // and keep the worst action he can do
              if(s < score_val)
                score_val = s;
              ++it;

              delete new_state;
            }
        }
    }

  return score_val;
}

//***************************** select_action *********************************

/**
 * \brief Select an action using the given method.
 * \param depth Maximum depth of the search tree.
 * \param current_state The state of the game.
 * \param new_action (in/out) Best known action.
 * \param computer_turn Tell if the action is done by the computer.
 */
template <typename Method>
void claw::ai::game::select_action<Method>::operator()(
    int depth, const state& current_state, action& new_action,
    bool computer_turn) const
{
  std::list<action> l;
  typename std::list<action>::iterator it;
  score best_eval;
  Method method;

  // get all reachable states
  current_state.next_actions(l);
  best_eval = current_state.min_score();

  for(it = l.begin(); it != l.end(); ++it)
    {
      state* new_state;
      score eval;

      // try and evaluate each action
      new_state = static_cast<state*>(current_state.do_action(*it));
      eval = method(depth - 1, *new_state, !computer_turn);

      delete new_state;

      // we keep one of the best actions
      if(eval > best_eval)
        {
          best_eval = eval;
          new_action = *it;
        }
    }
}

//*************************** select_random_action ****************************

/**
 * \brief Select a random action among the best ones.
 * \param depth Maximum depth of the search tree.
 * \param current_state The state of the game.
 * \param new_action (in/out) Best known action.
 * \param computer_turn Tell if the action is done by the computer.
 */
template <typename Method>
void claw::ai::game::select_random_action<Method>::operator()(
    int depth, const state& current_state, action& new_action,
    bool computer_turn) const
{
  std::list<action> l;
  typename std::list<action>::iterator it;
  action_eval<action, score> eval(new_action, current_state.min_score());
  Method method;
  max_vector<action_eval<action, score> > events(eval);

  // get all reachable states
  current_state.next_actions(l);

  for(it = l.begin(); it != l.end(); ++it)
    {
      state* new_state;

      // try and evaluate each action
      new_state = static_cast<state*>(current_state.do_action(*it));

      eval.action = *it;
      eval.eval = method(depth - 1, *new_state, !computer_turn);

      delete new_state;

      // keep the best actions.
      events.add(eval);
    }

  std::size_t i = (double)rand() / (RAND_MAX + 1) * events.get_v().size();
  new_action = events.get_v()[i].action;
}
