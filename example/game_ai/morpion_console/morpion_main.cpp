/**
 * \file morpion_main.cpp
 * \brief Implémentation de la classe morpion_main.
 * \author Julien Jorge
 */
#include "morpion_main.hpp"
#include "morpion.hpp"

#include <iostream>

/**
 * \brief Constructeur. Lance le jeu.
 */
morpion_main::morpion_main()
{
  int difficulte = 0;
  char commencer = 'o';

  std::cout << "Difficulté [1..9] : ";
  std::cin >> difficulte;

  std::cout << "Vous commencez ? (o/n) : ";
  std::cin >> commencer;

  if((commencer == 'o') || (commencer == 'O'))
    jouer_humain_premier(difficulte);
  else
    jouer_ordi_premier(difficulte);
}

/*================================== private ================================*/

/**
 * \brief Joue un coup, de l'ordinateur.
 * \param etat Etat du jeu.
 * \param difficulte Niveau de difficulté.
 * \return vrai si la partie est terminée.
 */
bool morpion_main::jouer_ordi(etat_morpion& etat, int difficulte)
{
  claw::ai::game::select_random_action<
      claw::ai::game::alpha_beta<etat_morpion> >
      select;

  int action = 0;

  select(difficulte, etat, action, true);

  if(etat.jouer(action))
    return true;
  else if(etat.plein())
    return true;
  else
    return false;
}

/**
 * \brief Joue un coup, de l'humain.
 * \param etat Etat du jeu.
 * \return vrai si la partie est terminée.
 */
bool morpion_main::jouer_humain(etat_morpion& etat)
{
  unsigned int c;
  std::cin >> c;

  if(c >= 9)
    return true;
  else if(etat.jouer(c))
    return true;
  else if(etat.plein())
    return true;
  else
    return false;
}

/**
 * \brief Lance le jeu, l'humain joue en premier.
 * \param difficulte Niveau de difficulté.
 */
void morpion_main::jouer_humain_premier(int difficulte)
{
  etat_morpion etat(EM_CROIX);
  etat.symbole_ordi(EM_ROND);

  std::cout << etat << std::endl;

  bool fini = jouer_humain(etat);

  while(!fini)
    {
      std::cout << etat << std::endl;
      fini = jouer_ordi(etat, difficulte);

      if(!fini)
        {
          std::cout << etat << std::endl;
          fini = jouer_humain(etat);
        }
    }

  std::cout << etat << std::endl;
}

/**
 * \brief Lance le jeu, l'ordinateur joue en premier.
 * \param difficulte Niveau de difficulté.
 */
void morpion_main::jouer_ordi_premier(int difficulte)
{
  etat_morpion etat(EM_ROND);
  etat.symbole_ordi(EM_ROND);

  std::cout << etat << std::endl;

  bool fini = jouer_ordi(etat, difficulte);

  while(!fini)
    {
      std::cout << etat << std::endl;
      fini = jouer_humain(etat);
      ;

      if(!fini)
        {
          std::cout << etat << std::endl;
          fini = jouer_ordi(etat, difficulte);
        }
    }

  std::cout << etat << std::endl;
}
