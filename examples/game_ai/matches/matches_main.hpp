/**
 * \file matches_main.hpp
 * \brief Main class of the game.
 * \author Julien Jorge
 */

#ifndef __MATCHES_MAIN_HPP__
#define __MATCHES_MAIN_HPP__

#include "matches.hpp"

/**
 * \brief Main class of the game.
 * \author Julien Jorge
 */
class matches_main
{
public:
  matches_main();

private:
  bool play_computer(state_matches& state, int difficulty);
  bool play_human(state_matches& state);
  void play_human_first( int matches, int difficulty );
  void play_computer_first( int matches, int difficulty );

}; //class matches_main

#endif // __MATCHES_MAIN_HPP__
