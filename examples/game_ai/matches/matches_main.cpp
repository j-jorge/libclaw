/**
 * \file matches_main.cpp
 * \brief Implementation of the matches_main class.
 * \author Julien Jorge
 */
#include "matches.hpp"
#include "matches_main.hpp"

#include <iostream>

/*---------------------------------------------------------------------------*/
/**
 * \brief Constructor. Start the game.
 */
matches_main::matches_main()
{
  int  matches = 0;
  int  difficulty = 0;
  char first = 'o';

  std::cout << "How many matches? [3 or more] : ";
  std::cin >> matches;

  std::cout << "Difficulty [1 or more] : ";
  std::cin >> difficulty;

  std::cout << "Do you start (y/n) : ";
  std::cin >> first;

  if ( (first=='y') || (first=='Y') )
    play_human_first(matches, difficulty);
  else
    play_computer_first(matches, difficulty);
} // matches_main()

/*---------------------------------------------------------------------------*/
/**
 * \brief Make the computer to play.
 * \param state The current state of the game.
 * \param difficulty Difficulty level.
 * \return true if the game is over.
 */
bool matches_main::play_computer(state_matches& state, int difficulty)
{
  claw::ai::game::select_action< claw::ai::game::min_max<state_matches> > select;
  int action = 1;
  select(difficulty, state, action, true);
        
  return state.play(action);
} // play_computer()

/*---------------------------------------------------------------------------*/
/**
 * \brief Ask the human player to play.
 * \param state The current state of the game.
 * \return true if the game is over.
 */
bool matches_main::play_human(state_matches& state)
{
  unsigned int c;

  std::cout << "How many matches do you take? [1..3] : ";
  std::cin >> c;

  if ( (c > 3) || (c==0) )
    return true;
  else 
    return state.play(c);
} // play_human()

/*---------------------------------------------------------------------------*/
/**
 * \brief Start a game beginning by the human player.
 * \param matches Original count of matches.
 * \param difficulty Difficulty level.
 */
void matches_main::play_human_first( int matches, int difficulty )
{
  state_matches state(matches, false);

  std::cout << state << std::endl;

  bool stop = play_human(state);

  if (stop)
    std::cout << "you lost" << std::endl;
  else
    {
      while ( !stop )
        {
          std::cout << state << std::endl;
          stop = play_computer( state, difficulty );

          if ( !stop )
            {
              std::cout << state << std::endl;
              stop = play_human(state);

              if (stop)
                std::cout << "you lost" << std::endl;
            }
          else
            std::cout << "you win" << std::endl;
        }

      std::cout << state << std::endl;
    }
}

/*---------------------------------------------------------------------------*/
/**
 * \brief Start a game beginning by the human player.
 * \param matches Original count of matches.
 * \param difficulty Difficulty level.
 */
void matches_main::play_computer_first( int matches, int difficulty )
{
  state_matches state(matches, true);

  std::cout << state << std::endl;

  bool stop = play_computer( state, difficulty );

  if (stop)
    std::cout << "you win" << std::endl;
  else
    {
      while ( !stop )
        {
          std::cout << state << std::endl;
          stop = play_human(state);;

          if ( !stop )
            {
              std::cout << state << std::endl;
              stop = play_computer(state, difficulty);

              if (stop)
                std::cout << "you win" << std::endl;
            }
          else
            std::cout << "you lost" << std::endl;
        }

      std::cout << state << std::endl;
    }
}

