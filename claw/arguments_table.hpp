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

  contact: julien.jorge@gamned.org
*/
/**
 * \file arguments_table.hpp
 * \brief A class to manage the arguments of your program, with automatic
 *        management of short/long arguments and help message.
 * \author Julien Jorge
 */
#ifndef __CLAW_ARGUMENTS_TABLE_HPP__
#define __CLAW_ARGUMENTS_TABLE_HPP__

#include <claw/arguments.hpp>

namespace claw
{
  /**
   * \brief A class to manage the arguments of your program, with automatic
   *        management of short/long arguments and help message.
   *
   * \remark None of those methods is allowed to use claw::logger because when
   *         we are processing the arguments, we are at the really begining of
   *         the program and claw::logger is probably not initialised.
   *
   * \author Julien Jorge
   */
  class arguments_table
  {
  private:
    /**
     * \brief This class manage the description of an argument.
     * \author Julien Jorge.
     */
    class argument_attributes
    {
    public:
      argument_attributes( const std::string& name,
                           const std::string& second_name,
                           const std::string& help_message, bool optional,
                           const std::string& value_type );

      bool operator<( const argument_attributes& that ) const;

      std::string format_short_help() const;
      std::string format_long_help( std::size_t arguments_width ) const;
      std::string format_long_help_arguments() const;

      const std::string& get_name() const;
      const std::string& get_second_name() const;

      bool is_optional() const;

    private:
      /** \brief The principal name of the argument. */
      const std::string m_name;

      /** \brief The second name of the argument. */
      const std::string m_second_name;

      /** \brief Message describing the argument. */
      const std::string m_help_message;

      /** \brief Tell if the argument is optional. */
      const bool m_optional;

      /** \brief The type of the value needed by this argument. */
      const std::string m_value_type;

    }; // class argument_attributes

  public:
    explicit arguments_table( const std::string& prog_name );
    arguments_table( int& argc, char** &argv );

    void add( const std::string& short_name, const std::string& long_name,
              const std::string& help_msg = "", bool optional = false,
              const std::string& val_name = "" );
    void add_long( const std::string& long_name,
                   const std::string& help_msg = "", bool optional = false,
                   const std::string& val_name = "" );
    void add_short( const std::string& short_name,
                    const std::string& help_msg = "", bool optional = false,
                    const std::string& val_name = "" );
    
    void parse( int& argc, char** &argv );
    void help( const std::string& free_args = "" ) const;
    
    bool required_fields_are_set() const;
    bool has_value( const std::string& arg_name ) const;
    bool only_integer_values( const std::string& arg_name ) const;
    bool only_real_values( const std::string& arg_name ) const;

    const std::string& get_program_name() const;

    bool get_bool( const std::string& arg_name ) const;
    int get_integer( const std::string& arg_name ) const;
    double get_real( const std::string& arg_name ) const;
    const std::string& get_string( const std::string& arg_name ) const;

    std::list<int> get_all_of_integer( const std::string& arg_name ) const;
    std::list<double> get_all_of_real( const std::string& arg_name ) const;
    std::list<std::string>
    get_all_of_string( const std::string& arg_name ) const;

    void add_argument( const std::string& arg );

  private:
    void get_argument_names( const std::string& arg_name,
                             std::string& short_name,
                             std::string& long_name ) const;

    std::size_t get_maximum_long_help_arguments_width() const;

  private:
    /** \brief The class that will store arguments values. */
    arguments m_arguments;

    /** \brief The arguments with a short version. */
    math::ordered_set<argument_attributes> m_short_arguments;

    /** \brief The arguments with a long version. */
    math::ordered_set<argument_attributes> m_long_arguments;

  }; // class arguments_table
} // namespace claw

#endif // __CLAW_ARGUMENTS_TABLE_HPP__
