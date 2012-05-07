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
 * \file arguments.cpp
 * \brief Implementation of the  claw::arguments class.
 * \author Julien Jorge
 */
#include <claw/arguments.hpp>

#include <sstream>
#include <claw/assert.hpp>
#include <claw/string_algorithm.hpp>
#include <claw/claw_gettext.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
claw::arguments::arguments()
  : m_program_name( claw_gettext("<unknow>") )
{

} // arguments::arguments()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param prog_name Force the name of the program.
 */
claw::arguments::arguments( const std::string& prog_name )
  : m_program_name(prog_name)
{

} // arguments::arguments()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param argc Number of arguments.
 * \param argv Arguments.
 *
 * You should construct an instance with the parameters given to your function
 * main(). The constructor will remove all supported arguments from argv.
 */
claw::arguments::arguments( int& argc, char** &argv )
{
  parse(argc, argv);
} // arguments::arguments()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param argc Number of arguments.
 * \param argv Arguments.
 * \param allowed The set of allowed arguments.
 *
 * You should construct an instance with the parameters given to your function
 * main(). The constructor will remove all supported arguments from argv.
 */
claw::arguments::arguments(int& argc, char** &argv,
                           const claw::math::ordered_set<std::string>& allowed )

{
  parse(argc, argv, allowed);
} // arguments::arguments()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse arguments.
 * \param argc Number of arguments.
 * \param argv Arguments.
 *
 * All supported arguments will be removed from argv.
 */
void claw::arguments::parse( int& argc, char** &argv )
{
  parse( argc, argv, true, claw::math::ordered_set<std::string>() );
} // arguments::parse()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse arguments.
 * \param argc Number of arguments.
 * \param argv Arguments.
 * \param allowed The set of allowed arguments.
 *
 * All supported arguments will be removed from argv.
 */
void claw::arguments::parse
( int& argc, char** &argv,
  const claw::math::ordered_set<std::string>& allowed )
{
  parse( argc, argv, false, allowed );
} // arguments::parse()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a value is associated to an argument.
 * \param arg_name The name of the argument to test.
 */
bool claw::arguments::has_value( const std::string& arg_name ) const
{
  return m_pairs.find( arg_name ) != m_pairs.end();
} // arguments::has_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if only integer values are associated to an argument.
 * \param arg_name The name of the argument to test.
 */
bool claw::arguments::only_integer_values( const std::string& arg_name ) const
{
  const valued_arguments_map::const_iterator itk(m_pairs.find(arg_name));
  bool result = true;

  if ( itk == m_pairs.end() )
    result = false;
  else
    {
      std::list<std::string>::const_iterator it;
      for( it=itk->second.begin(); result && (it!=itk->second.end()); ++it )
        result = result && text::is_of_type<int>(*it);
    }

  return result;
} // arguments::only_integer_values()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if only real values are associated to an argument.
 * \param arg_name The name of the argument to test.
 */
bool claw::arguments::only_real_values( const std::string& arg_name ) const
{
  const valued_arguments_map::const_iterator itk(m_pairs.find(arg_name));
  bool result = true;

  if ( itk == m_pairs.end() )
    result = false;
  else
    {
      std::list<std::string>::const_iterator it;
      for( it=itk->second.begin(); result && (it!=itk->second.end()); ++it )
        result = result && text::is_of_type<double>(*it);
    }

  return result;
} // arguments::only_real_values()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the program.
 */
const std::string& claw::arguments::get_program_name() const
{
  return m_program_name;
} // arguments::get_program_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the boolean state of an argument.
 * \param arg_name The name of the argument to get.
 */
bool claw::arguments::get_bool( const std::string& arg_name ) const
{
  return m_flags.find( arg_name ) != m_flags.end();
} // arguments::get_bool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the integer value of an argument.
 * \param arg_name The name of the argument to get.
 * \pre has_value(arg_name)
 */
int claw::arguments::get_integer( const std::string& arg_name ) const
{
  CLAW_ASSERT( has_value(arg_name),
               "arguments::get_integer(): argument is not set." );

  std::istringstream iss( m_pairs.find( arg_name )->second.back() );
  int val;
  iss >> val;

  return val;
} // arguments::get_integer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the real value of an argument.
 * \param arg_name The name of the argument to get.
 * \pre has_value(arg_name)
 */
double claw::arguments::get_real( const std::string& arg_name ) const
{
  CLAW_ASSERT( has_value(arg_name),
               "arguments::get_real(): argument is not set." );

  std::istringstream iss( m_pairs.find( arg_name )->second.back() );
  double val;
  iss >> val;

  return val;
} // arguments::get_real()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the string value of an argument.
 * \param arg_name The name of the argument to get.
 * \pre has_value(arg_name)
 */
const std::string&
claw::arguments::get_string( const std::string& arg_name ) const
{
  CLAW_ASSERT( has_value(arg_name),
               "arguments::get_string(): argument is not set." );

  return m_pairs.find( arg_name )->second.back();
} // arguments::get_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get all integer values of an argument.
 * \param arg_name The name of the argument to get.
 */
std::list<int>
claw::arguments::get_all_of_integer( const std::string& arg_name ) const
{
  std::list<int> result;
  const valued_arguments_map::const_iterator itk(m_pairs.find(arg_name));

  if ( itk != m_pairs.end() )
    {
      std::list<std::string>::const_iterator it;

      for( it=itk->second.begin(); it!=itk->second.end(); ++it )
        if ( text::is_of_type<int>(*it) )
          {
            std::istringstream iss(*it);
            int val;
            iss >> val;
            result.push_back(val);
          }
    }

  return result;
} // arguments::get_all_of_integer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get all real values of an argument.
 * \param arg_name The name of the argument to get.
 */
std::list<double>
claw::arguments::get_all_of_real( const std::string& arg_name ) const
{
  std::list<double> result;
  const valued_arguments_map::const_iterator itk(m_pairs.find(arg_name));

  if ( itk != m_pairs.end() )
    {
      std::list<std::string>::const_iterator it;

      for( it=itk->second.begin(); it!=itk->second.end(); ++it )
        if ( text::is_of_type<double>(*it) )
          {
            std::istringstream iss(*it);
            double val;
            iss >> val;
            result.push_back(val);
          }
    }

  return result;
} // arguments::get_all_of_real()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get all string values of an argument.
 * \param arg_name The name of the argument to get.
 */
std::list<std::string>
claw::arguments::get_all_of_string( const std::string& arg_name ) const
{
  std::list<std::string> result;
  const valued_arguments_map::const_iterator itk(m_pairs.find(arg_name));

  if ( itk != m_pairs.end() )
    result = itk->second;

  return result;
} // arguments::get_all_of_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an argument in our list.
 *
 * You can use this method to set default values to the parameters of your
 * program, before calling parse_arguments.
 *
 * \param arg The argument to add.
 * \pre (arg != "--") && (arg[0] == '-')
 */
void claw::arguments::add_argument( const std::string& arg )
{
  CLAW_ASSERT( arg != "--", "arguments::add_argument(): arg can't be '--'" );
  CLAW_ASSERT( arg[0] == '-',
               "arguments::add_argument(): arg must begin by '-'" );
  
  std::string name, value;
  const bool has_value = split_argument(arg, name, value);

  if ( !has_value )
    m_flags.insert( arg );
  else
    m_pairs[name].push_back(value);
} // arguments::add_argument()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse arguments.
 * \param argc Number of arguments.
 * \param argv Arguments.
 * \param always_allowed If true, \a allowed is never used and all arguments
 *        with a valid format are accepted.
 * \param allowed The set of allowed arguments.
 * \return true if the argument has a value associated with him.
 *
 * All supported arguments will be removed from argv.
 */
void claw::arguments::parse
( int& argc, char** &argv, bool always_allowed,
  const claw::math::ordered_set<std::string>& allowed )
{
  bool stop = false;
  int base = 0;

  if (m_program_name.empty() && (argc!=0))
    {
      m_program_name = argv[0];
      argv[0] = NULL;
      base = 1;
    }

  for (int argi=base; (argi!=argc) && !stop; ++argi)
    {
      std::string arg(argv[argi]);

      if ( !arg.empty() )
        if ( (arg[0] == '-') && (arg.length() > 1) )
          {
            if (arg == "--")
              stop = true;
            else
              {
                std::string name, value;
                const bool has_value = split_argument( arg, name, value );

                if ( !has_value )
                  process_boolean( argv[argi], always_allowed, allowed );
                else if ( always_allowed
                          || (allowed.find( name ) != allowed.end()) )
                  {
                    add_argument( arg );
                    argv[argi] = NULL;
                  }
              }
          }
    }

  remove_null_arguments( argc, argv );
} // arguments::parse()

/*----------------------------------------------------------------------------*/
/**
 * \brief Split an argument to get its name and its value.
 * \param arg The argument to split.
 * \param name (out) The name of the argument.
 * \param value (out) The value of the argument.
 * \return true if the argument has a value associated with him.
 * \pre (arg != "--") && (arg[0] == '-')
 */
bool claw::arguments::split_argument
( const std::string& arg, std::string& name, std::string& value ) const
{
  CLAW_ASSERT( arg != "--", "arguments::split_argument(): arg can't be '--'" );
  CLAW_ASSERT( arg[0] == '-',
               "arguments::split_argument(): arg must begin by '-'" );
  
  std::string::size_type pos = arg.find_first_of('=');
  bool result(false);

  if ( pos == std::string::npos )
    {
      name = arg;
      value.clear();
    }
  else
    {
      name = arg.substr(0, pos);
      value = arg.substr(pos+1, arg.length() - pos - 1);
      result = true;
    }

  return result;
} // arguments::split_argument()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all NULL arguments from argv and update argc.
 * \param argc The number of arguments.
 * \param argv The arguments.
 */
void claw::arguments::remove_null_arguments( int& argc, char** &argv ) const
{
  unsigned int c=0; // number of non-NULL arguments
  
  for (int i=0; i!=argc; ++i)
    if ( argv[i] != NULL )
      ++c;
    else
      {
        bool ok = false;
        int j=i;
        
        while ( (j!=argc) && !ok )
          if ( argv[j] == NULL )
            ++j;
          else
            ok = true;

        if (ok)
          {
            argv[i] = argv[j];
            argv[j] = NULL;
            ++c;
          }
      }

  if ( c > 0 )
    if ( (std::string(argv[c-1]) == "--") )
      --c;
    
  argc=c;
} // arguments::remove_null_arguments()

/*----------------------------------------------------------------------------*/
/**
 * \brief Process a boolean option.
 * \param arg (in) The argument to process, (out) the argument cleaned of all
 *        valid values.
 * \param always_allowed If true, \a allowed is never used and all arguments
 *        with a valid format are accepted.
 * \param allowed The set of allowed arguments.
 *
 */
void claw::arguments::process_boolean
( char* &arg, bool always_allowed,
  const claw::math::ordered_set<std::string>& allowed )
{
  CLAW_ASSERT( std::string(arg) != "--", "arg can't be '--'" );
  CLAW_ASSERT( std::string(arg).length() > 1,
               "arg must be at least two characters long" );
  CLAW_ASSERT( arg[0] == '-', "arg must begin by '-'" );

  if ( arg[1] == '-' ) // long boolean
    {
      if ( always_allowed || (allowed.find(arg) != allowed.end()) )
        {
          add_argument(arg);
          arg = NULL;
        }
    }
  else // short boolean(s)
    {
      int i(1);
      std::string s("-?"); // equivalent single character argument

      while ( arg[i] != '\0' )
        {
          s[1] = arg[i];

          if ( always_allowed || (allowed.find(s) != allowed.end()) )
            {
              add_argument(s);

              // shift remaining arguments
              for ( int j=i; arg[j]!='\0'; ++j )
                arg[j] = arg[j+1];
            }
          else
            ++i;
        }

      if ( i==1 ) // all arguments have been accepted
        arg = NULL;
    }
} // arguments::process_boolean()
