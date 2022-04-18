#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__
#include <claw/application/application.hpp>
#include <vector>

class ex_sockets : public claw::application
{
public:
  ex_sockets(int& argc, char**& argv);
  int run();

private:
  void check_arguments(int& argc, char**& argv);

  void run_server() const;
  void run_client() const;

private:
  bool m_quit;

  bool m_server;

  std::string m_ip;

  unsigned int m_port;

  unsigned int m_delay;

  std::vector<std::string> m_message;

}; // class ex_sockets

#endif // __APPLICATION_HPP__
