/*
  CLAW - a C++ Library Absolutely Wonderful

  CLAW is a free library without any particular aim but being useful to
  anyone.

  Copyright (C) 2005-2011 Julien Jorge

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  contact: julien.jorge@stuff-o-matic.com
*/
/**
 * \file arguments.hpp
 * \brief A class to manage the arguments of your program.
 * \author Julien Jorge
 */
#ifndef __CLAW_ARGUMENTS_HPP__
#define __CLAW_ARGUMENTS_HPP__

#include <claw/ordered_set.hpp>

#include <map>
#include <string>

namespace claw
{
  /**
   * \brief A class to manage the arguments of your program.
   *
   * This class will handle all arguments of type -l[=val] or --long[=val].
   *
   * \remark None of those methods is allowed to use claw::logger because when
   *         we are processing the arguments, we are at the really begining of
   *         the program and claw::logger is probably not initialised.
   *
   * \author Julien Jorge
   */
  class arguments
  {
  private:
    typedef std::map<std::string, std::list<std::string> >
        valued_arguments_map;

  public:
    arguments();
    explicit arguments(const std::string& prog_name);
    arguments(int& argc, char**& argv);
    arguments(int& argc, char**& argv,
              const claw::math::ordered_set<std::string>& allowed);

    void parse(int& argc, char**& argv);
    void parse(int& argc, char**& argv,
               const claw::math::ordered_set<std::string>& allowed);

    bool has_value(const std::string& arg_name) const;
    bool only_integer_values(const std::string& arg_name) const;
    bool only_real_values(const std::string& arg_name) const;

    const std::string& get_program_name() const;

    bool get_bool(const std::string& arg_name) const;
    int get_integer(const std::string& arg_name) const;
    double get_real(const std::string& arg_name) const;
    const std::string& get_string(const std::string& arg_name) const;

    std::list<int> get_all_of_integer(const std::string& arg_name) const;
    std::list<double> get_all_of_real(const std::string& arg_name) const;
    std::list<std::string>
    get_all_of_string(const std::string& arg_name) const;

    void add_argument(const std::string& arg);

  private:
    void parse(int& argc, char**& argv, bool always_allowed,
               const claw::math::ordered_set<std::string>& allowed);
    bool split_argument(const std::string& arg, std::string& name,
                        std::string& value) const;

    void remove_null_arguments(int& argc, char**& argv) const;

    void process_boolean(char*& arg, bool always_allowed,
                         const claw::math::ordered_set<std::string>& allowed);

  private:
    /** \brief The name of the program. */
    std::string m_program_name;

    /** \brief yes/no arguments. */
    claw::math::ordered_set<std::string> m_flags;

    /** \brief Arguments of type key=value. */
    valued_arguments_map m_pairs;

  }; // class arguments
}

#endif // __CLAW_ARGUMENTS_HPP__
