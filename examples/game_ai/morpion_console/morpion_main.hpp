/**
 * \file morpion_main.hpp
 * \brief Classe principale du jeu de morpion. Gère le déroulement du jeu.
 * \author Julien Jorge
 */

#ifndef __MORPION_MAIN_HPP__
#define __MORPION_MAIN_HPP__

#include "morpion.hpp"

/**
 * \brief Classe principale du jeu de morpion. Gère le déroulement du jeu.
 * \author Julien Jorge
 */
class morpion_main
{
public:
  morpion_main();

private:
  bool jouer_ordi(etat_morpion& etat, int difficulte);
  bool jouer_humain(etat_morpion& etat);
  void jouer_humain_premier( int difficulte );
  void jouer_ordi_premier( int difficulte );

}; //class morpion_main

#endif // __MORPION_MAIN_HPP__
