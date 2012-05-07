#include <claw/application.hpp>
#include <claw/configuration_file.hpp>

class ex_config : public claw::application
{
public:
  ex_config( int& argc, char** &argv );
  int run();

private:
  void check_arguments( int& argc, char** &argv );

private:
  bool m_quit;

  claw::configuration_file m_config;

  std::string m_filename;

  bool m_noname_section;

  std::string m_section;

  std::string m_field;

}; // class ex_config
