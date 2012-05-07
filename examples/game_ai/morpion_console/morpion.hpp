/**
 * \file morpion.hpp
 * \brief Description du damier du morpion.
 * \author Julien Jorge
 */
#ifndef __MORPION_HPP__
#define __MORPION_HPP__

#include "claw/game_ai.hpp"

#include <iostream>

//*********************** symbole_morpion ************************************

  /** \brief Les symboles que l'on peut trouver dans les cases. */
enum symbole
{
  EM_RIEN,
  EM_CROIX,
  EM_ROND
};

/** \brief Une classe pour nous simplifier la manipulation des symboles. */
struct symbole_morpion
{
  symbole s;

  symbole_morpion();
  symbole_morpion(symbole symb);

  bool operator==(const symbole_morpion& symb) const;
  bool operator!=(const symbole_morpion& symb) const;
  symbole operator!() const;

  inline friend std::ostream& operator<<(std::ostream& os, 
                                         const symbole_morpion& symb);
}; // symbole_morpion

//************************** etat_morpion ************************************


  /**
   * \brief Description du damier du morpion.
   * \author Julien Jorge
   */
class etat_morpion : public claw::ai::game::game_state<int>
{
private:
  typedef claw::ai::game::game_state<int,int> state;

public:
  friend std::ostream& operator<<(std::ostream& os, const etat_morpion& e);

  etat_morpion( symbole symbole = EM_RIEN );

  bool jouer( int c );
  bool plein() const;
  static void symbole_ordi( const symbole_morpion& symbole );

  bool gagne( const symbole_morpion& symbole ) const;
  const std::vector<symbole_morpion>& cases() const;

  //- 
  // Partie nécessaire à l'intelligence artificielle
  //-

  virtual state* do_action( const int& a ) const;
  virtual bool final() const;
  virtual void next_actions( std::list<int>& l ) const;
  int evaluate() const;

  //- 
  // Fin partie nécessaire à l'intelligence artificielle
  //-

private:
  int poids_des_symboles(int a, int b, int c) const;

private:
  /** \brief Symbole qui va jouer. */
  symbole_morpion m_symbole;
  /** \brief Etat des cases. */
  std::vector<symbole_morpion> m_cases;
  
  /** \brief Symbole de l'ordinateur. */
  static symbole_morpion s_symbole_ordi;
}; // etat_morpion

#endif // __MORPION_HPP__
