/**
 * \file main.cpp
 * \brief This is an example program to show a basic use of the claw::automaton
 *        class.
 * \author Julien Jorge
 */
#include <claw/automaton.hpp>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>

/**
 * \brief Print an automation.
 * \param os The stream to write in.
 * \param a The automaton to print.
 * \return os.
 */
template <typename Automaton>
std::ostream& print_automaton(std::ostream& os, const Automaton& a)
{
  typedef typename Automaton::state_type state_type;
  typedef typename Automaton::edge_type edge_type;

  std::vector<state_type> states;
  std::vector<edge_type> alphabet;

  typename std::vector<state_type>::const_iterator it_s;
  typename std::vector<edge_type>::const_iterator it_a;

  /* Print the alphabet. */
  a.alphabet(alphabet);
  if(alphabet.empty())
    os << "A = {}" << std::endl;
  else
    {
      it_a = alphabet.begin();

      os << "A = { " << *it_a;
      ++it_a;

      for(; it_a != alphabet.end(); ++it_a)
        os << ", " << *it_a;

      os << " }" << std::endl;
    }

  /* Print initial states. */
  a.initial_states(states);
  if(states.empty())
    os << "I = {}" << std::endl;
  else
    {
      it_s = states.begin();

      os << "I = { " << *it_s;
      ++it_s;

      for(; it_s != states.end(); ++it_s)
        os << ", " << *it_s;

      os << " }" << std::endl;
    }

  /* Print final states. */
  a.final_states(states);
  if(states.empty())
    os << "F = {}" << std::endl;
  else
    {
      it_s = states.begin();

      os << "F = { " << *it_s;
      ++it_s;

      for(; it_s != states.end(); ++it_s)
        os << ", " << *it_s;

      os << " }" << std::endl;
    }

  /* Print states and edges. */
  a.states(states);
  if(states.empty())
    {
      os << "E = {}" << std::endl;
      os << "T = {}" << std::endl;
    }
  else
    {
      std::vector<edge_type> transitions;

      typename std::vector<state_type>::const_iterator it_v;

      /* states */
      it_s = states.begin();

      os << "E = { " << *it_s;
      ++it_s;

      for(; it_s != states.end(); ++it_s)
        os << ", " << *it_s;

      os << " }" << std::endl;

      /* edges */
      os << "T = {" << std::endl;

      for(it_s = states.begin(); it_s != states.end(); ++it_s)
        {
          os << *it_s << " -> { ";

          for(it_v = states.begin(); it_v != states.end(); ++it_v)
            {
              a.edges(*it_s, *it_v, transitions);
              for(it_a = transitions.begin(); it_a != transitions.end();
                  ++it_a)
                os << "(" << *it_v << ", " << *it_a << ") ";
            }

          os << "}" << std::endl;
        }
      os << "}" << std::endl;
    }

  return os;
}

/**
 * \brief Read a line describing a part of an automaton.
 * \param is The stream in which we read the attributes.
 * \param a The automaton in which we add the result of the line.
 *
 * The following actions can be done by this function:
 * add an edge:
 *  E <source state> <target state> <symbol>
 *
 * add a state:
 *  S <state>
 *
 * add an initial state:
 *  I <state>
 *
 * add a final state:
 *  F <state>
 *
 * Other lines are ignored.
 */
template <typename Automaton>
void read_line(std::istream& is, Automaton& a)
{
  typename Automaton::state_type src, target;
  typename Automaton::edge_type edge;
  char line_type;

  if(is >> line_type)
    switch(line_type)
      {
      case 'E':
        {
          is >> src >> target >> edge;
          a.add_edge(src, target, edge);
          break;
        }
      case 'S':
        {
          is >> src;
          a.add_state(src);
          break;
        }
      case 'I':
        {
          is >> src;
          a.add_initial_state(src);
          break;
        }
      case 'F':
        {
          is >> src;
          a.add_final_state(src);
          break;
        }
      }
}

/**
 * \brief Load an automation from a file.
 * \param is The stream describing the automaton.
 * \param a (out) The read automaton.
 */
template <typename Automaton>
void load_automaton(std::istream& is, Automaton& a)
{
  std::string line;

  while(std::getline(is, line))
    {
      std::istringstream iss(line);
      read_line(iss, a);
    }
}

/**
 * \brief Tell if a pattern is recognized by an automaton.
 * \param pattern The pattern to check.
 * \param a The automaton.
 */
bool valid_pattern(const std::string& pattern,
                   const claw::automaton<int, char>& a)
{
  return a.match(pattern.begin(), pattern.end());
}

int main(int argc, char* argv[])
{
  if(argc < 3)
    std::cerr << "usage:\n"
              << argv[0] << " automaton_file pattern..." << std::endl;
  else
    {
      std::ifstream f(argv[1]);

      if(!f)
        std::cerr << "Can't open file '" << argv[1] << "'" << std::endl;
      else
        {
          claw::automaton<int, char> a;

          load_automaton(f, a);
          print_automaton(std::cout, a) << std::endl;

          for(int i = 2; i != argc; ++i)
            if(valid_pattern(argv[i], a))
              std::cout << argv[i] << ": valid" << std::endl;
            else
              std::cout << argv[i] << ": not valid" << std::endl;
        }
    }

  return 0;
}
