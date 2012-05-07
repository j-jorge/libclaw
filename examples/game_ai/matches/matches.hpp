/**
 * \file allumettes.hpp
 * \brief The class representing the state of the game.
 * \author Julien Jorge
 */
#ifndef __MATCHES_HPP__
#define __MATCHES_HPP__

#include "claw/game_ai.hpp"

#include <iostream>

/**
 * \brief The class representing the state of the game.
 * \author Julien Jorge
 */
class state_matches:
  public claw::ai::game::game_state<int>
{
private:
  typedef claw::ai::game::game_state<int> state;

public:
  friend std::ostream& operator<<
  (std::ostream& os, const state_matches& e);

  state_matches( int matches, bool computer );

  bool play( int c );

  //-
  // methods from game_state
  //-

  state* do_action( const int& a ) const;
  bool final() const;
  void next_actions( std::list<int>& l ) const;
  int evaluate() const;

  //- 
  // End of the methods from game_state
  //-

private:
  /** \brief Tell if it is the computer's turn. */
  bool m_computer;

  /** \brief Remaining matches. */
  int m_matches;

}; // state_matches

#endif // MATCHES_HPP
