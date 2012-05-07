#include "application.hpp"
#include <claw/string_algorithm.hpp>
#include <claw/socket_server.hpp>

/*----------------------------------------------------------------------------*/
ex_sockets::ex_sockets( int& argc, char** &argv )
  : claw::application(argc, argv), m_quit(false), m_delay(10)
{
  check_arguments( argc, argv );
} // ex_sockets::ex_sockets()

/*----------------------------------------------------------------------------*/
int ex_sockets::run()
{
  if (!m_quit)
    {
      if ( !claw::net::socket_server::init() )
        std::cerr << "Socket initialization problem." << std::endl;
      else if (m_server)
        run_server();
      else
        run_client();

      claw::net::socket_server::release();
    }

  return 0;
} // ex_sockets::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check program arguments.
 * \param argc Number of program arguments.
 * \param argv Program arguments.
 */
void ex_sockets::check_arguments( int& argc, char** &argv )
{
  m_arguments.add_long("--ip", "IP to connect to.", true, "address");
  m_arguments.add_long("--port", "Port to listen/connect to.", false, "integer");
  m_arguments.add_long("--server", "Launch a server.", true);
  m_arguments.add_long("--delay", "Timeout (seconds).", true, "integer");
  m_arguments.add("-h", "--help", "Print this message and exit.", true);

  m_arguments.parse( argc, argv );

  if ( m_arguments.get_bool("-h") || !m_arguments.has_value("--port") )
    {
      m_arguments.help();
      m_quit = true;
    }
  else
    {
      m_server = m_arguments.get_bool("--server");

      if ( !m_server )
        {
          if ( m_arguments.has_value("--ip") )
            m_ip = m_arguments.get_string("--ip");
          else
            {
              std::cerr << "You must provide an IP to connect to." << std::endl;
              m_quit = true;
            }
        }

      if ( !m_quit )
        {
          if ( claw::text::is_of_type<unsigned int>
               ( m_arguments.get_string("--port") ) )
            m_port = m_arguments.get_integer("--port");
          else
            {
              std::cerr << "'" << m_arguments.get_string("--port")
                        << "' is not a valid port." << std::endl;
              m_quit = true;
            }
	  
          if ( m_arguments.has_value("--delay") )
            {
              if ( claw::text::is_of_type<unsigned int>
                   ( m_arguments.get_string("--delay") ))
                m_delay = m_arguments.get_integer("--delay");
              else
                {
                  std::cerr << "'" << m_arguments.get_string("--delay")
                            << "' is not a valid delay." << std::endl;
                  m_quit = true;
                }
            }
	  
          for (int i=0; i!=argc; ++i)
            m_message.push_back(argv[i]);
        }
    }
} // ex_sockets::check_arguments()

/*----------------------------------------------------------------------------*/
void ex_sockets::run_server() const
{
  claw::net::socket_server server(m_port);
  claw::net::isocket_stream sock;

  if (server)
    server.accept( sock, m_delay );
  else
    std::cout << "No server" << std::endl;
  
  if (!sock.is_open())
    std::cout << "No socket" << std::endl;
  else
    {
      size_t l;
      std::string msg;
      bool fail=false;

      do
        {
          if (sock >> l)
            std::cout << l << ":";
          else
            {
              fail=true;
              std::cerr << "ERROR: getting l" << std::endl;
            }

          if ( std::getline(sock, msg) )
            {
              std::cout << "'" << msg << "'" << std::endl;
            }
          else
            {
              fail=true;
              std::cerr << "ERROR: getting msg" << std::endl;
            }
        }
      while( (msg != "stop") && !fail );

      sock.close();
    }

  server.close();
} // ex_sockets::run_server()

/*----------------------------------------------------------------------------*/
void ex_sockets::run_client() const
{
  claw::net::osocket_stream sock(m_ip.c_str(), m_port);

  if (!sock)
    std::cerr << "ERREUR: osocket_stream" << std::endl;
  else
    {
      for (unsigned int i=0; i!=m_message.size(); ++i)
        {
          if ( !(sock << m_message[i].length()) )
            std::cerr << "error sending length.\n";
          else if ( !(sock << m_message[i]))
            std::cerr << "error sending message.\n";

          sock << std::endl;
        }
      
      sock.close();
    }
} // ex_sockets::run_client()
