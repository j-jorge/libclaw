/*
  Example program for the claw::dynamic_library class.
*/

#include <iostream>
#include <claw/dynamic_library.hpp>

#ifdef _WIN32
# define DLLEXPORT __declspec(dllexport)
#else
# define DLLEXPORT
#endif

typedef void (*function_type)( const std::string& s );

extern "C"
DLLEXPORT void in_program( const std::string& s )
{
  std::cout << "in_program : " << s << std::endl;
}

int main( int argc, char** argv )
{
  if ( argc < 2 )
    {
      std::cout << argv[0] << " method_name...\n";
      std::cout << "\tThe first method_name is searched in the current "
                << "program\n";
      std::cout << "\tThe next arguments are searched in the ./libfunc.so "
                << "shared library." << std::endl;
    }
  else
    {
      /* Load the library. */
#ifdef _WIN32
      claw::dynamic_library dl( "./libfunc.dll" );
#else
      claw::dynamic_library dl( "./libfunc.so" );
#endif

      /* Load the current program. */
      claw::dynamic_library current_prog( argv[0], true );

      function_type f;

      /* Search in the current program. */
      if ( current_prog.have_symbol( argv[1] ) )
        {
          f = current_prog.get_symbol<function_type>( argv[1] );
          f( "The string passed to a method in the current program" );
        }
      else
        std::cout << "Symbol '" << argv[1]
                  << "' not found in the current program." << std::endl;

      /* We try to find the next symbols passed as arguments */
      for (int i=2; i<argc; ++i)
        if ( dl.have_symbol( argv[i] ) )
          {
            f = dl.get_symbol<function_type>( argv[i] );

            f( "The string passed to the method" );
          }
        else
          std::cout << "Symbol '" << argv[i] << "' not found." << std::endl;
    }

  return 0;
}
