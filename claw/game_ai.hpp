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

  contact: julien.jorge@gamned.org
*/
/**
 * \file game_ai.hpp
 * \brief Artificial intelligence for games.
 * \author Julien Jorge, Sébastien Angibaud
 */
#ifndef __CLAW_GAME_AI_HPP__
#define __CLAW_GAME_AI_HPP__

#include <list>

namespace claw
{
  namespace ai
  {
    namespace game
    {
      //**************************** game_state ********************************

      /**
       * \brief A state of a game.
       *
       * \b Template \b parameters:
       * - \param Action: a type representing an action of a player.
       * - \param Numeric: the type used for evaluationg the players' scores.
       *
       * \author Julien Jorge
       */
      template<typename Action, typename Numeric = int>
      class game_state
      {
      public:
        /** \brief The type used for evaluationg the players' scores. */
        typedef Numeric score;

	/** \brief A type representing an action of a player. */
        typedef Action action;

      public:
        virtual ~game_state();

        /** \brief Evaluate this state of the game. */
        virtual score evaluate() const = 0;

        static score min_score();
        static score max_score();

        /** 
         * \brief Get all actions that can be done from this state.
         * \param l (out) The actions.
         */
        virtual void next_actions( std::list<action>& l ) const = 0;

        /** 
         * \brief Get a new state obtained when applying an action.
         * \param a The action to apply.
         * \return The state resulting from this action.
         */
        virtual game_state* do_action( const action& a ) const = 0;

        /** \brief Tell if the game is over. */
        virtual bool final() const = 0;

      protected :
        score fit( score score_val ) const;

      protected :
        /** \brief Minimal score that can be given to a state. */
        static const score s_min_score;

        /** \brief Maximal score that can be given to a state. */
        static const score s_max_score;

      }; // class game_state

      //**************************** action_eval ******************************

      /**
       * \brief A score associated with an action.
       *
       * \b Template \b parameters:
       * - \a Action: the type of the action,
       * - \a Action: the type of the evaluation.
       */
      template <typename Action, typename Numeric>
      class action_eval
      {
      public:
        action_eval( const Action& a, const Numeric& e);

        bool operator< ( const action_eval& ae ) const;
        //bool operator==( const action_eval& ae ) const;

      public:
        /** \brief The action. */
        Action action;

        /** \brief The score of the action. */
        Numeric eval;

      }; // class action_eval

      //*************************** min_max ***********************************

      /**
       * \brief Find an action with the MinMax algorithm.
       *
       * \b Template \b parameters:
       * - \a State: the type of a state of the game. Must inherit from
       *   game_state.
       *
       * \author Julien Jorge
       */
      template <typename State>
      class min_max
      {
      public:
        typedef State state;
        typedef typename State::action action;
        typedef typename State::score score;

        score operator()
	( int depth, const state& current_state,  bool computer_turn ) const;
      }; // class min_max

      //*************************** alpha_beta ********************************

      /**
       * \brief Find an action with the alpha-beta algorithm.
       *
       * \b Template \b parameters:
       * - \a State: the type of a state of the game. Must inherit from
       *   game_state.
       *
       * \author Julien Jorge, Sébastien Angibaud
       */
      template <typename State>
      class alpha_beta
      {
      public:
        typedef State state;
        typedef typename State::action action;
        typedef typename State::score score;

        score operator()
	( int depth, const state& current_state, bool computer_turn ) const;
      private:
        score compute
	( int depth, const state& current_state, bool computer_turn,
	  score alpha, score beta ) const;
      }; // class alpha_beta

      //*************************** select_action *****************************

      /**
       * \brief Select an action using a given method (min_max, alpha_beta).
       *
       * \b Template \b parameters:
       * - \a Method: the method to use to evaluate the actions.
       *
       * \author Julien Jorge, Sébastien Angibaud
       */
      template<typename Method>
      class select_action
      {
      public:
        typedef typename Method::state state;
        typedef typename Method::action action;
        typedef typename Method::score score;

        void operator()
	( int depth, const state& current_state, action& new_action,
	  bool computer_turn ) const;
      }; // class select_action

      //************************ select_random_action *************************

      /**
       * \brief Select a random action among the best ones.
       *
       * \b Template \b parameters:
       * - \a Method: the method to use to evaluate the actions.
       *
       * \author Julien Jorge, Sébastien Angibaud
       */
      template<typename Method>
      class select_random_action
      {
      public:
        typedef typename Method::state state;
        typedef typename Method::action action;
        typedef typename Method::score score;

        void operator()( int depth, const state& current_state,
                         action& new_action, bool computer_turn ) const;
      }; // class select_random_action

    } // namespace game
  } // namespace it
} // namespace claw

#include <claw/impl/game_ai.tpp>

#endif // __CLAW_IA_JEUX_HPP__
