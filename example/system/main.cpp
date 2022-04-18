#include <claw/system_info.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
  std::cout << "your name: '" << claw::system_info::get_user_name() << "'\n";
  std::cout << "your directory: '" << claw::system_info::get_user_directory()
            << "'\n";
  std::cout << "host name: '" << claw::system_info::get_host_name() << "'\n";
  std::cout << "temp directory: '"
            << claw::system_info::get_temporary_directory() << "'\n";

  for(int i = 1; i < argc; ++i)
    std::cout << "\t'" << argv[i] << "': '"
              << claw::system_info::get_environment(argv[i]) << "'\n";

  std::cout.flush();

  return 0;
}
