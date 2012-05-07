#include <claw/basic_singleton.hpp>
#include <string>
#include <iostream>

class resource_manager : public claw::concept::basic_singleton<resource_manager>
{
public:
  void print( const std::string& s ) const
  {
    std::cout << s << std::endl;
  }
}; // class resource_manager

int main()
{
  resource_manager::get_instance().print( "The instance is just allocated" );
  resource_manager::get_instance().print( "One more sentence" );
  resource_manager::get_instance().print( "The instance will be deleted at the "
                                          "end of the program" );

  return 0;
} // main()
