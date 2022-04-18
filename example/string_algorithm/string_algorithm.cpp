#include <claw/string_algorithm.hpp>
#include <iostream>
#include <string>

int main()
{
  std::string ui("24");

  std::string s("\t  word aaa bbb abab aaa  \t ");
  std::string s1(s);
  std::string s2(s);
  std::string s3(s);
  claw::text::trim_left(s1, " \t");
  claw::text::trim_right(s2, " \t");
  claw::text::trim(s3, " \t");

  std::cout << ui << " is of type uinsigned int : "
            << claw::text::is_of_type<unsigned int>(ui) << std::endl;

  std::cout << "trim_left '" << s << "' = '" << s1 << "'" << std::endl;
  std::cout << "trim_right '" << s << "' = '" << s2 << "'" << std::endl;
  std::cout << "trim '" << s << "' = '" << s3 << "'" << std::endl;

  std::string s4(s3);

  claw::text::squeeze(s4, "ab");
  std::cout << "squeeze '" << s3 << "' = '" << s4 << "'" << std::endl;

  s4 = s;
  claw::text::replace(s4, std::string("ab"), std::string("C"));
  std::cout << "replace '" << s << "' = '" << s4 << "'" << std::endl;

  return 0;
}
