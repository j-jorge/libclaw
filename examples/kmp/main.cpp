/**
 * \file main.cpp
 * \brief Test program for the Knuth-Morris-Pratt algorithm.
 * \author Julien Jorge
 */

#include <vector>
#include <string>
#include <iostream>

#include "claw/kmp.hpp"


typedef std::string::const_iterator it_chaine_t;

/**
 * \brief Predicate passed to KMP.
 * \param i Match position
 */
bool print_pos( unsigned int i )
{
  std::cout << "Pattern found at position " << i << std::endl;
  return true;
}

/**
 * \brief Main procedure.
 * \param argc Parameters count. Should be 3.
 * \param argv Parameters. argv[1] is the pattern, argv[2] is the text.
 */
int main(int argc, char* argv [])
{
  if (argc < 3)
    {
      std::cerr << "./kmp pattern text" << std::endl;
      return 1;
    }
  else
    {
      std::string pattern = argv[1];
      std::string text = argv[2];
      claw::text::kmp< it_chaine_t > kmp;

      kmp(pattern.begin(), pattern.end(), text.begin(), text.end(), print_pos);
    }
}
