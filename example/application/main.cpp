/**
 * \file main.cpp
 * \brief This program show a basic use of the claw::application class.
 * \author Julien Jorgex
 */
#include <claw/application/application.hpp>
#include <claw/logger/logger.hpp>

#include <iostream>

/**
 * \brief This class represents the application.
 */
class my_app : public claw::application
{
public:
  my_app(int& argc, char**& argv);
  int run();
}; // class my_app

/**
 * \brief Constructor. Parse the command line and initialize the application.
 */
my_app::my_app(int& argc, char**& argv)
  : claw::application(argc, argv)
{
  m_arguments.add_short("-b", "Sample boolean argument.", true);
  m_arguments.add_long("--string", "Print a string.", true, "any_string");
  m_arguments.add("-h", "--help", "Print this help screen.", true);

  m_arguments.parse(argc, argv);

  if(m_arguments.get_bool("-h"))
    m_arguments.help("more_string...");

  std::cout << "Testing error_level:" << std::endl;

  claw::logger << claw::log_error << "error_level:program_name == "
               << m_arguments.get_program_name() << claw::lendl;

  std::cout << "Testing warning_level:" << std::endl;

  for(int i = 0; i < argc; ++i)
    claw::logger << claw::log_warning << "warning_level:argument " << i
                 << " == " << argv[i] << claw::lendl;
}

/**
 * \brief This is the main loop.
 */
int my_app::run()
{
  std::cout << "Testing verbose_level:" << std::endl;

  claw::logger << claw::log_verbose << "verbose_level:m_app is running\n";

  std::cout << "'-b' argument is ";

  if(m_arguments.get_bool("-b"))
    std::cout << "present.";
  else
    std::cout << "missing.";

  std::cout << std::endl;

  std::cout << "'--string' argument value is '";

  if(m_arguments.has_value("--string"))
    std::cout << m_arguments.get_string("--string");

  std::cout << "'" << std::endl;

  return 0;
}

// this macro defines the main function, declare a variable of type my_app and
// call the my_app::run() method.
CLAW_APPLICATION_IMPLEMENT(my_app)
