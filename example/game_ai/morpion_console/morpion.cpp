/**
 * \file morpion.cpp
 * \brief Impl�mentation des classes symbole_morpion et etat_morpion.
 * \author Julien Jorge
 */
#include "morpion.hpp"

#include <iostream>

template <>
const int claw::ai::game::game_state<int>::s_min_score = -100;
template <>
const int claw::ai::game::game_state<int>::s_max_score = 100;

//*********************** symbole_morpion ************************************

/**
 * \brief Constructeur.
 */
symbole_morpion::symbole_morpion()
  : s(EM_RIEN)
{}

/**
 * \brief Constructeur avec un symbole.
 */
symbole_morpion::symbole_morpion(symbole symb)
  : s(symb)
{}

/**
 * \brief Op�rateur d'�galit�.
 */
bool symbole_morpion::operator==(const symbole_morpion& symb) const
{
  return s == symb.s;
}

/**
 * \brief Op�rateur de diff�rence.
 */
bool symbole_morpion::operator!=(const symbole_morpion& symb) const
{
  return !(s == symb.s);
}

/**
 * \brief Symbole oppos�.
 */
symbole symbole_morpion::operator!() const
{
  switch(s)
    {
    case EM_CROIX:
      return EM_ROND;
    case EM_ROND:
      return EM_CROIX;
    default:
      return EM_RIEN;
    }
}

/**
 * \brief Forme textuelle.
 * \param os Flux sur lequel �crire.
 * \param symb symbole � �crire.
 */
std::ostream& operator<<(std::ostream& os, const symbole_morpion& symb)
{
  switch(symb.s)
    {
    case EM_CROIX:
      os << "X";
      break;
    case EM_ROND:
      os << "O";
      break;
    case EM_RIEN:
      os << ".";
      break;
    }

  return os;
}

//************************** etat_morpion ************************************

symbole_morpion etat_morpion::s_symbole_ordi;

/**
 * \brief Affiche un �tat sur un flux.
 * \param os Flux sur lequel �crire.
 * \param e Damier � afficher.
 */
std::ostream& operator<<(std::ostream& os, const etat_morpion& e)
{
  int i;

  os << "symbole : " << e.m_symbole << std::endl;

  for(i = 0; i != 3; ++i)
    os << e.cases()[i];

  os << std::endl;

  for(i = 3; i != 6; ++i)
    os << e.cases()[i];

  os << std::endl;

  for(i = 6; i != 9; ++i)
    os << e.cases()[i];

  os << std::endl;

  return os;
}

/**
 * \brief Constructeur.
 * \param symb symbole du prochain joueur.
 */
etat_morpion::etat_morpion(symbole symb /*= EM_RIEN*/)
  : m_symbole(symb)
  , m_cases(9, EM_RIEN)
{}

/**
 * \brief Joue le symbole en cours dans une case et passe au symbole suivant.
 * \param c Indice de la case.
 */
bool etat_morpion::jouer(int c)
{
  bool ret = false;
  m_cases[c] = m_symbole;

  ret = gagne(m_symbole);

  m_symbole = !m_symbole;

  return ret;
}

/**
 * \brief Indique si le jeu est plein.
 */
bool etat_morpion::plein() const
{
  bool ret = true;

  std::vector<symbole_morpion>::const_iterator it;

  for(it = m_cases.begin(); (it != m_cases.end() && ret); ++it)
    ret = *it != EM_RIEN;

  return ret;
}

/**
 * \brief Affecte la valeur du symbole de l'ordi.
 * \param symb Le nouveau symbole de l'ordi.
 */
void etat_morpion::symbole_ordi(const symbole_morpion& symb)
{
  s_symbole_ordi = symb;
}

/**
 * \brief Indique si l'�tat actuel est gagnant pour un symbole.
 * \param symb Symbole qui pourrait avoir gagn�.
 */
bool etat_morpion::gagne(const symbole_morpion& symb) const
{
  // lignes passant par le centre
  if(m_cases[4] == symb)
    {
      if((m_cases[0] == symb) && (m_cases[8] == symb))
        return true;
      else if((m_cases[1] == symb) && (m_cases[7] == symb))
        return true;
      else if((m_cases[2] == symb) && (m_cases[6] == symb))
        return true;
      else if((m_cases[3] == symb) && (m_cases[5] == symb))
        return true;
    }

  // lignes partant du coin haut gauche, sauf diagonale
  if(m_cases[0] == symb)
    {
      if((m_cases[1] == symb) && (m_cases[2] == symb))
        return true;
      else if((m_cases[3] == symb) && (m_cases[6] == symb))
        return true;
    }

  // lignes partant du coin bas droit, sauf diagonale
  if(m_cases[8] == symb)
    {
      if((m_cases[6] == symb) && (m_cases[7] == symb))
        return true;
      else if((m_cases[2] == symb) && (m_cases[5] == symb))
        return true;
    }

  return false;
}

/**
 * \brief R�cup�re les valeurs des cases.
 */
const std::vector<symbole_morpion>& etat_morpion::cases() const
{
  return m_cases;
}

/**
 * \brief Effectuer une action.
 * \param a L'action a effectuer.
 * \return L'�tat r�sultant de cette action.
 */
etat_morpion::state* etat_morpion::do_action(const int& a) const
{
  etat_morpion* em = new etat_morpion(*this);

  em->m_cases[a] = em->m_symbole; // on marque la case
  em->m_symbole = !em->m_symbole; // on passe la main

  return em;
}

/**
 * \brief Indique si la partie est termin�e.
 */
bool etat_morpion::final() const
{
  if(gagne(m_symbole))
    return true;
  else if(gagne(!m_symbole))
    return true;
  else
    return plein();
}

/**
 * \brief Calcule la liste des actions jouables depuis l'�tat actuel.
 * \param l (sortie) Liste des coups jouables.
 */
void etat_morpion::next_actions(std::list<int>& l) const
{
  for(int i = 0; i != 9; ++i)
    if(m_cases[i] == EM_RIEN)
      l.push_back(i);
}

/**
 * Evalue l'�tat actuel, du point de vue de l'ordinateur.
 */
int etat_morpion::evaluate() const
{
  int score;

  if(gagne(s_symbole_ordi))
    {
      if(gagne(!s_symbole_ordi))
        score = 0;
      else
        score = 100;
    }
  else if(gagne(!s_symbole_ordi))
    score = -100;
  else
    {
      int score_ligne;
      score = 0;

      score_ligne = poids_des_symboles(0, 1, 2);
      score += score_ligne * 4;

      score_ligne = poids_des_symboles(3, 4, 5);
      score += score_ligne * 4;

      score_ligne = poids_des_symboles(6, 7, 8);
      score += score_ligne * 4;

      score_ligne = poids_des_symboles(0, 3, 6);
      score += score_ligne * 4;

      score_ligne = poids_des_symboles(1, 4, 7);
      score += score_ligne * 4;

      score_ligne = poids_des_symboles(2, 5, 8);
      score += score_ligne * 4;

      score_ligne = poids_des_symboles(0, 4, 8);
      score += score_ligne * 4;

      score_ligne = poids_des_symboles(2, 4, 6);
      score += score_ligne * 4;
    }

  return score;
}

/**
 * Calcule la sup�riorit� du symbole de l'ordinateur sur son contraire sur
 * trois cases. \param a indice de la premi�re case. \param b indice de la
 * deuxi�me case. \param c indice de la troisi�me case.
 */
int etat_morpion::poids_des_symboles(int a, int b, int c) const
{
  int s = 0;
  symbole_morpion contraire = !s_symbole_ordi;

  if(m_cases[a] == s_symbole_ordi)
    ++s;
  else if(m_cases[a] == contraire)
    --s;

  if(m_cases[b] == s_symbole_ordi)
    ++s;
  else if(m_cases[b] == contraire)
    --s;

  if(m_cases[c] == s_symbole_ordi)
    ++s;
  else if(m_cases[c] == contraire)
    --s;

  return s;
}
