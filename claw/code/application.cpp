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
 * \file application.cpp
 * \brief Implementation of the claw::application class.
 * \author Julien Jorge
 */
#include <claw/application.hpp>

#include <claw/logger.hpp>
#include <claw/log_stream_uniq.hpp>
#include <claw/log_stream_concise.hpp>
#include <claw/claw_gettext.hpp>

#define CLAW_MK_STR_(e) #e
#define CLAW_MK_STR(e) CLAW_MK_STR_(e)

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param argc Number of arguments to the program.
 * \param argv The arguments of the program.
 * 
 * The constructor removes from argv all supported arguments, and updates the
 * value of argc.
 */
claw::application::application( int& argc, char** &argv )
  : m_arguments( argc, argv )
{
  setlocale( LC_ALL, "" );
#ifdef CLAW_TEXT_DOMAIN_PATH
  bindtextdomain( "libclaw", CLAW_MK_STR(CLAW_TEXT_DOMAIN_PATH) );
#endif
  bind_textdomain_codeset( "libclaw", "UTF-8" );
  textdomain("libclaw");

  m_arguments.add_long
    ("--log-file", claw_gettext("The file to use to store log informations."),
     true, claw_gettext("file") );
  m_arguments.add_long
    ("--log-level",
     claw_gettext("Level of log informations:\n"
		  "\t\terror: error messages,\n"
		  "\t\twarning: warning and error messages,\n"
		  "\t\tverbose: all messages."), true, claw_gettext("string") );
  m_arguments.add_long
    ("--log-uniq",
     claw_gettext
     ("Use a logger that does not output successively the same message."),
     true );
  m_arguments.add_long
    ("--log-concise",
     claw_gettext
     ("Use a logger that does not output messages that have been recently"
      " output."), true, claw_gettext("integer") );

  m_arguments.parse( argc, argv );
  
  log_stream* log;

  if ( m_arguments.has_value("--log-file") )
    log = new file_logger( m_arguments.get_string("--log-file") );
  else
    log = new console_logger;

  if ( m_arguments.get_bool("--log-uniq") )
    log = new log_stream_uniq(log);
  else if ( m_arguments.has_value("--log-concise")
	    && m_arguments.only_integer_values("--log-concise")
	    && m_arguments.get_integer("--log-concise") > 0 )
    log = new log_stream_concise(log, m_arguments.get_integer("--log-concise"));
  else if ( m_arguments.get_bool("--log-concise") )
    log = new log_stream_concise(log);
 
  logger.set( log );

  if ( m_arguments.has_value( "--log-level" ) )
    {
      std::string level = m_arguments.get_string("--log-level");

      if ( (level == "error") || (level == claw_gettext("error")) )
	logger.set_level( log_error );
      else if ( (level == "warning") || (level == claw_gettext("warning")) )
	logger.set_level( log_warning );
      else if ( (level == "verbose") || (level == claw_gettext("verbose")) )
	logger.set_level( log_verbose );
      else
	logger.set_level( m_arguments.get_integer("--log-level") );
    }

} // application::application()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
claw::application::~application()
{
  logger.clear();
} // application::~application()
