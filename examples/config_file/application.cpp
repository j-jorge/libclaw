#include "application.hpp"
#include <fstream>

/*----------------------------------------------------------------------------*/
ex_config::ex_config( int& argc, char** &argv )
  : claw::application(argc, argv), m_quit(false), m_noname_section(true)
{
  check_arguments( argc, argv );
} // ex_config::ex_config()

/*----------------------------------------------------------------------------*/
int ex_config::run()
{
  if (m_quit)
    return 0;

  std::ifstream f( m_filename.c_str() );

  if (!f)
    std::cerr << "can't open file '" << m_filename << "'" << std::endl;
  else if ( !m_config.open(f) )
    std::cerr << "syntax error in '" << m_filename << "'" << std::endl;
  else
    {
      std::cout << "field value is '";

      if ( m_noname_section )
        std::cout << m_config(m_field);
      else
        std::cout << m_config(m_section, m_field);
	  
      std::cout << "'" << std::endl;

      std::cout << "all values are:\n";
      claw::configuration_file::const_field_iterator it, last;

      if ( m_noname_section )
        {
          it = m_config.field_begin(m_field);
          last = m_config.field_end(m_field);
        }
      else
        {
          it = m_config.field_begin(m_section, m_field);
          last = m_config.field_end(m_section, m_field);
        }

      for ( ; it!=last; ++it )
        std::cout << "\t'" << *it << "'\n";

      std::cout << "The sections of the file are:\n";

      claw::configuration_file::const_file_iterator itf;
      for (itf=m_config.file_begin(); itf!=m_config.file_end(); ++itf)
        std::cout << '\'' << *itf << "'\n";

      std::cout.flush();
    }

  return 0;
} // ex_config::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check program arguments.
 * \param argc Number of program arguments.
 * \param argv Program arguments.
 */
void ex_config::check_arguments( int& argc, char** &argv )
{
  m_arguments.add_long("--config", "Configuration file to open", false, "file");
  m_arguments.add_long
    ("--field", "The name of the field to find.", false, "string");
  m_arguments.add_long
    ("--section", "section in which the field is.", true, "string");
  m_arguments.add("-h", "--help", "Print this message and exit.", true);

  m_arguments.parse( argc, argv );

  if ( m_arguments.get_bool("-h") || !m_arguments.required_fields_are_set() )
    {
      std::cout << m_arguments.required_fields_are_set() << std::endl;
      m_arguments.help();
      m_quit = true;
    }
  else
    {
      m_noname_section = !m_arguments.has_value("--section");

      if ( !m_noname_section )
	m_section = m_arguments.get_string("--section");

      m_field = m_arguments.get_string("--field");
      m_filename = m_arguments.get_string("--config");
    }
} // ex_config::check_arguments()
