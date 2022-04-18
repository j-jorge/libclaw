/**
 * \file main.cpp
 * \brief Test program for the claw::math::ordered_set class.
 * \author Julien Jorge
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "claw/ordered_set.hpp"

typedef claw::math::ordered_set<unsigned int> set_type;

enum operation
{
  UNION,
  INTERSECTION,
  DIFFERENCE,
  SYMETRIC_DIFFERENCE,
  CONTAINS,
  STRICT_CONTAINS,
  CONTAINED,
  STRICT_CONTAINED
}; // enum operation

void result(bool b)
{
  if(b)
    std::cout << "yes" << std::endl;
  else
    std::cout << "no" << std::endl;
}

void result(const set_type& s)
{
  set_type::const_iterator it;

  std::cout << "{ ";

  for(it = s.begin(); it != s.end(); ++it)
    std::cout << *it << ", ";

  std::cout << "}" << std::endl;
}

void help()
{
  std::cerr << "ordered_set value... op value..." << std::endl;
  std::cerr << "with :" << std::endl;
  std::cerr << "value : an unsigned integers." << std::endl;
  std::cerr << "op : one string in { union, intersection, difference, "
               "symetric_difference, contains, strictly_contains, contained, "
               "strictly_contained }."
            << std::endl;
}

void apply_op(operation op, set_type& s1, const set_type& s2)
{
  switch(op)
    {
    case UNION:
      std::cout << "union:";
      result(s1 += s2);
      break;
    case INTERSECTION:
      std::cout << "intersection:";
      result(s1 *= s2);
      break;
    case DIFFERENCE:
      std::cout << "difference:";
      result(s1 -= s2);
      break;
    case SYMETRIC_DIFFERENCE:
      std::cout << "symetric_difference:";
      result(s1 /= s2);
      break;
    case CONTAINS:
      std::cout << "contains:";
      result(s1 >= s2);
      break;
    case STRICT_CONTAINS:
      std::cout << "strictly_contains:";
      result(s1 > s2);
      break;
    case CONTAINED:
      std::cout << "contained:";
      result(s1 <= s2);
      break;
    case STRICT_CONTAINED:
      std::cout << "strictly_contained:";
      result(s1 < s2);
      break;
    }
}

/**
 * \brief Main procedure.
 * \param argc Parameters count. Should be 3.
 * \param argv Parameters. argv[1] is the pattern, argv[2] is the text.
 */
int main(int argc, char* argv[])
{
  if(argc < 3)
    {
      help();
      return EXIT_FAILURE;
    }
  else
    {
      set_type left, right;
      unsigned int current_set = 0;
      operation op = UNION;

      for(int argi = 1; argi != argc; ++argi)
        if((argv[argi][0] >= '0') && (argv[argi][0] <= '9'))
          {
            std::istringstream iss(argv[argi]);
            unsigned int val;
            iss >> val;

            current_set == 0 ? left.insert(val) : right.insert(val);
          }
        else if(current_set != 0)
          {
            std::cerr << argv[argi] << std::endl;
            help();
            exit(EXIT_FAILURE);
          }
        else
          {
            std::string arg(argv[argi]);
            current_set = 1;

            if(arg == "union")
              op = UNION;
            else if(arg == "intersection")
              op = INTERSECTION;
            else if(arg == "difference")
              op = DIFFERENCE;
            else if(arg == "symetric_difference")
              op = SYMETRIC_DIFFERENCE;
            else if(arg == "contains")
              op = CONTAINS;
            else if(arg == "strictly_contains")
              op = STRICT_CONTAINS;
            else if(arg == "contained")
              op = CONTAINED;
            else if(arg == "strictly_contained")
              op = STRICT_CONTAINED;
            else
              {
                std::cerr << arg << std::endl;
                help();
                exit(EXIT_FAILURE);
              }
          }

      result(left);
      result(right);
      apply_op(op, left, right);
    }

  return EXIT_SUCCESS;
}
