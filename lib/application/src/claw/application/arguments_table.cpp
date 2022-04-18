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
 * \file arguments_table.cpp
 * \brief Implementation of the claw::arguments_table class.
 * \author Julien Jorge
 */
#include <claw/application/arguments_table.hpp>

#include <claw/assert.hpp>
#include <iomanip>
#include <iostream>

/**
 * \brief Constructor.
 * \param name The principal name of the argument.
 * \param second_name The second name of the argument.
 * \param help_message Message describing the role of the argument.
 * \param optional Tell if this argument is optional.
 * \param value_type The kind of value needed for this argument.
 */
claw::arguments_table::argument_attributes::argument_attributes(
    const std::string& name, const std::string& second_name,
    const std::string& help_message, bool optional,
    const std::string& value_type)
  : m_name(name)
  , m_second_name(second_name)
  , m_help_message(help_message)
  , m_optional(optional)
  , m_value_type(value_type)
{}

/**
 * \brief Tell if the principal name of this argument is alphabetically less
 *        than the name of an other argument.
 */
bool claw::arguments_table::argument_attributes::operator<(
    const argument_attributes& that) const
{
  return m_name < that.m_name;
}

/**
 * \brief Get a short description of the arguement.
 */
std::string
claw::arguments_table::argument_attributes::format_short_help() const
{
  std::string result(m_name);

  if(!m_value_type.empty())
    result += "=" + m_value_type;

  if(m_optional)
    return "[" + result + "]";
  else
    return result;
}

/**
 * \brief Get a long description of the argument.
 * \param arguments_width The width in chararcters occupied by the arguments on
 *        the formatted line.
 */
std::string claw::arguments_table::argument_attributes::format_long_help(
    std::size_t arguments_width) const
{
  std::ostringstream result;

  result << std::left << std::setw(arguments_width)
         << format_long_help_arguments() << ' ' << m_help_message;

  return result.str();
}

/**
 * \brief Returns a string containing the arguments formatted in the same way
 *        than they are in format_long_help.
 */
std::string
claw::arguments_table::argument_attributes::format_long_help_arguments() const
{
  std::string result(m_name);

  if(!m_second_name.empty())
    result += ", " + m_second_name;

  return result;
}

/**
 * \brief Get the principal name of the argument.
 */
const std::string& claw::arguments_table::argument_attributes::get_name() const
{
  return m_name;
}

/**
 * \brief Get the second name of the argument.
 */
const std::string&
claw::arguments_table::argument_attributes::get_second_name() const
{
  return m_second_name;
}

/**
 * \brief Tell if the argument is optional.
 */
bool claw::arguments_table::argument_attributes::is_optional() const
{
  return m_optional;
}

/**
 * \brief Constructor.
 * \param prog_name Force the name of the program.
 */
claw::arguments_table::arguments_table(const std::string& prog_name)
  : m_arguments(prog_name)
{}

/**
 * \brief Constructor.
 * \param argc Number of arguments.
 * \param argv Arguments.
 *
 * All supported arguments will be removed from argv.
 */
claw::arguments_table::arguments_table(int& argc, char**& argv)
  : m_arguments(argc, argv, claw::math::ordered_set<std::string>())
{}

/**
 * \brief Add an argument in the table.
 * \param short_name The short name of the argument.
 * \param long_name The long name of the argument.
 * \param help_msg A description of the argument.
 * \param optional Tell if the argument is optional.
 * \param val_name The type of the value needed for this argument.
 */
void claw::arguments_table::add(const std::string& short_name,
                                const std::string& long_name,
                                const std::string& help_msg, bool optional,
                                const std::string& val_name)
{
  m_short_arguments.insert(argument_attributes(short_name, long_name, help_msg,
                                               optional, val_name));
  m_long_arguments.insert(argument_attributes(long_name, short_name, help_msg,
                                              optional, val_name));
}

/**
 * \brief Add an argument in the table.
 * \param long_name The long name of the argument.
 * \param help_msg A description of the argument.
 * \param optional Tell if the argument is optional.
 * \param val_name The type of the value needed for this argument.
 */
void claw::arguments_table::add_long(const std::string& long_name,
                                     const std::string& help_msg,
                                     bool optional,
                                     const std::string& val_name)
{
  m_long_arguments.insert(
      argument_attributes(long_name, "", help_msg, optional, val_name));
}

/**
 * \brief Add an argument in the table.
 * \param short_name The short name of the argument.
 * \param help_msg A description of the argument.
 * \param optional Tell if the argument is optional.
 * \param val_name The type of the value needed for this argument.
 */
void claw::arguments_table::add_short(const std::string& short_name,
                                      const std::string& help_msg,
                                      bool optional,
                                      const std::string& val_name)
{
  m_short_arguments.insert(
      argument_attributes(short_name, "", help_msg, optional, val_name));
}

/**
 * \brief Parse the command line arguments.
 * \param argc Number of arguments.
 * \param argv Arguments.
 *
 * All supported arguments will be removed from argv.
 */
void claw::arguments_table::parse(int& argc, char**& argv)
{
  math::ordered_set<std::string> allowed;
  math::ordered_set<argument_attributes>::const_iterator it;

  for(it = m_short_arguments.begin(); it != m_short_arguments.end(); ++it)
    allowed.insert(it->get_name());

  for(it = m_long_arguments.begin(); it != m_long_arguments.end(); ++it)
    allowed.insert(it->get_name());

  m_arguments.parse(argc, argv, allowed);
}

/**
 * \brief Print some help about the arguments.
 * \param free_args The arguments of your program that are not managed by
 *        claw::arguments_table.
 *
 * The method prints the name of the program, required arguments, optional
 * arguments and, then, \a free_args. Arguments are printed in short format
 * when available. A line is then skipped and the long description of the
 * arguments is printed.
 */
void claw::arguments_table::help(const std::string& free_args) const
{
  std::cout << m_arguments.get_program_name();

  typedef math::ordered_set<argument_attributes>::const_iterator set_iterator;

  std::list<set_iterator> optional;
  std::list<set_iterator>::const_iterator it_opt;
  set_iterator it;

  for(it = m_short_arguments.begin(); it != m_short_arguments.end(); ++it)
    if(it->is_optional())
      optional.push_back(it);
    else
      std::cout << ' ' << it->format_short_help();

  for(it = m_long_arguments.begin(); it != m_long_arguments.end(); ++it)
    if(it->get_second_name().empty())
      {
        if(it->is_optional())
          optional.push_back(it);
        else
          std::cout << ' ' << it->format_short_help();
      }

  for(it_opt = optional.begin(); it_opt != optional.end(); ++it_opt)
    std::cout << ' ' << (*it_opt)->format_short_help();

  if(!free_args.empty())
    std::cout << ' ' << free_args;

  std::cout << "\n\n";

  const std::size_t description_column(
      get_maximum_long_help_arguments_width());

  for(it = m_short_arguments.begin(); it != m_short_arguments.end(); ++it)
    std::cout << "  " << it->format_long_help(description_column) << '\n';

  for(it = m_long_arguments.begin(); it != m_long_arguments.end(); ++it)
    if(it->get_second_name().empty())
      std::cout << "  " << it->format_long_help(description_column) << '\n';
}

/**
 * \brief Tell if all arguments not marqued as "optional" have been specified
 * in the command line.
 *
 * \remark The method doesn't check the value of the arguments. If, for
 * example, an argument needs to be an integer and the user use it as a
 * boolean, the method will return true.
 */
bool claw::arguments_table::required_fields_are_set() const
{
  bool ok = true;
  math::ordered_set<argument_attributes>::const_iterator it;

  for(it = m_short_arguments.begin(); (it != m_short_arguments.end()) && ok;
      ++it)
    if(!it->is_optional())
      ok = ok && has_value(it->get_name());

  for(it = m_long_arguments.begin(); (it != m_long_arguments.end()) && ok;
      ++it)
    if(!it->is_optional())
      ok = ok && has_value(it->get_name());

  return ok;
}

/**
 * \brief Tell if an argument has a value.
 * \param arg_name The name of the argument to find.
 */
bool claw::arguments_table::has_value(const std::string& arg_name) const
{
  bool result = false;
  std::string short_name, long_name;

  get_argument_names(arg_name, short_name, long_name);

  if(!short_name.empty())
    result = m_arguments.has_value(short_name);

  if(!result)
    if(!long_name.empty())
      result = m_arguments.has_value(long_name);

  return result;
}

/**
 * \brief Tell if only integer values are associated to an argument.
 * \param arg_name The name of the argument to test.
 */
bool claw::arguments_table::only_integer_values(
    const std::string& arg_name) const
{
  bool result = true;
  std::string short_name, long_name;

  get_argument_names(arg_name, short_name, long_name);

  if(short_name.empty() && long_name.empty())
    result = false;
  else
    {
      if(!short_name.empty())
        result = m_arguments.only_integer_values(short_name);

      if(!long_name.empty())
        result = result && m_arguments.only_integer_values(long_name);
    }

  return result;
}

/**
 * \brief Tell if only real values are associated to an argument.
 * \param arg_name The name of the argument to test.
 */
bool claw::arguments_table::only_real_values(const std::string& arg_name) const
{
  bool result = true;
  std::string short_name, long_name;

  get_argument_names(arg_name, short_name, long_name);

  if(short_name.empty() && long_name.empty())
    result = false;
  else
    {
      if(!short_name.empty())
        result = m_arguments.only_real_values(short_name);

      if(!long_name.empty())
        result = result && m_arguments.only_real_values(long_name);
    }

  return result;
}

/**
 * \brief Get the name of the program.
 */
const std::string& claw::arguments_table::get_program_name() const
{
  return m_arguments.get_program_name();
}

/**
 * \brief Get the boolean state of an argument.
 * \param arg_name The name of the argument to find.
 */
bool claw::arguments_table::get_bool(const std::string& arg_name) const
{
  std::string short_name, long_name;

  get_argument_names(arg_name, short_name, long_name);

  return m_arguments.get_bool(short_name) || m_arguments.get_bool(long_name);
}

/**
 * \brief Get the integer value of an argument.
 * \param arg_name The name of the argument to find.
 * \pre has_value(arg_name)
 */
int claw::arguments_table::get_integer(const std::string& arg_name) const
{
  CLAW_PRECOND(has_value(arg_name));

  std::string short_name, long_name;

  get_argument_names(arg_name, short_name, long_name);

  if(m_arguments.has_value(short_name))
    return m_arguments.get_integer(short_name);
  else
    return m_arguments.get_integer(long_name);
}

/**
 * \brief Get the real value of an argument.
 * \param arg_name The name of the argument to find.
 * \pre has_value(arg_name)
 */
double claw::arguments_table::get_real(const std::string& arg_name) const
{
  CLAW_PRECOND(has_value(arg_name));

  std::string short_name, long_name;

  get_argument_names(arg_name, short_name, long_name);

  if(m_arguments.has_value(short_name))
    return m_arguments.get_real(short_name);
  else
    return m_arguments.get_real(long_name);
}

/**
 * \brief Get the string value of an argument.
 * \param arg_name The name of the argument to find.
 * \pre has_value(arg_name)
 */
const std::string&
claw::arguments_table::get_string(const std::string& arg_name) const
{
  CLAW_PRECOND(has_value(arg_name));

  std::string short_name, long_name;

  get_argument_names(arg_name, short_name, long_name);

  if(m_arguments.has_value(short_name))
    return m_arguments.get_string(short_name);
  else
    return m_arguments.get_string(long_name);
}

/**
 * \brief Get all integer values of an argument.
 * \param arg_name The name of the argument to get.
 */
std::list<int>
claw::arguments_table::get_all_of_integer(const std::string& arg_name) const
{
  std::list<int> result;
  std::string short_name, long_name;

  get_argument_names(arg_name, short_name, long_name);

  if(!short_name.empty())
    result = m_arguments.get_all_of_integer(short_name);

  if(!long_name.empty())
    {
      const std::list<int> p(m_arguments.get_all_of_integer(long_name));
      result.insert(result.end(), p.begin(), p.end());
    }

  return result;
}

/**
 * \brief Get all real values of an argument.
 * \param arg_name The name of the argument to get.
 */
std::list<double>
claw::arguments_table::get_all_of_real(const std::string& arg_name) const
{
  std::list<double> result;
  std::string short_name, long_name;

  get_argument_names(arg_name, short_name, long_name);

  if(!short_name.empty())
    result = m_arguments.get_all_of_real(short_name);

  if(!long_name.empty())
    {
      const std::list<double> p(m_arguments.get_all_of_real(long_name));
      result.insert(result.end(), p.begin(), p.end());
    }

  return result;
}

/**
 * \brief Get all string values of an argument.
 * \param arg_name The name of the argument to get.
 */
std::list<std::string>
claw::arguments_table::get_all_of_string(const std::string& arg_name) const
{
  std::list<std::string> result;
  std::string short_name, long_name;

  get_argument_names(arg_name, short_name, long_name);

  if(!short_name.empty())
    result = m_arguments.get_all_of_string(short_name);

  if(!long_name.empty())
    {
      const std::list<std::string> p(m_arguments.get_all_of_string(long_name));
      result.insert(result.end(), p.begin(), p.end());
    }

  return result;
}

/**
 * \brief Add an argument in our list.
 *
 * You can use this method to set default values to the parameters of your
 * program, before calling parse.
 *
 * \param arg The argument to add.
 * \pre (arg != "--") && (arg[0] == '-')
 */
void claw::arguments_table::add_argument(const std::string& arg)
{
  m_arguments.add_argument(arg);
}

/**
 * \brief Get the principal name and the second name of an argument.
 * \param arg_name The name of the argument to find.
 * \param short_name The short name of the argument.
 * \param long_name The long name of the argument.
 */
void claw::arguments_table::get_argument_names(const std::string& arg_name,
                                               std::string& short_name,
                                               std::string& long_name) const
{
  argument_attributes attr(arg_name, "", "", false, "");
  math::ordered_set<argument_attributes>::const_iterator it;

  // if arg_name is short, try to find the long version
  it = m_short_arguments.find(attr);

  if(it != m_short_arguments.end())
    {
      short_name = arg_name;
      long_name = it->get_second_name();
    }
  else
    {
      // if arg_name is long, try to find the short version
      it = m_long_arguments.find(attr);

      if(it != m_long_arguments.end())
        {
          short_name = it->get_second_name();
          long_name = arg_name;
        }
    }
}

/**
 * \brief Returns the length of the longest formatted argument names in the
 * long help display.
 */
std::size_t
claw::arguments_table::get_maximum_long_help_arguments_width() const
{
  math::ordered_set<argument_attributes>::const_iterator it;
  std::size_t result(0);

  for(it = m_short_arguments.begin(); it != m_short_arguments.end(); ++it)
    result = std::max(result, it->format_long_help_arguments().size());

  for(it = m_long_arguments.begin(); it != m_long_arguments.end(); ++it)
    if(it->get_second_name().empty())
      result = std::max(result, it->format_long_help_arguments().size());

  return result;
}
