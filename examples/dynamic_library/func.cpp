/*
  Example library for the claw::dynamic_library class.
*/

#include <string>
#include <iostream>

extern "C"
{
  void first_function( const std::string& s )
  {
    std::cout << "first : " << s << std::endl;
  }

  void second_function( const std::string& s )
  {
    std::cout << "second : " << s << std::endl;
  }
}

