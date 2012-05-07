/**
 * \file main.cpp
 * \brief Sample program presenting the use of the claw::memory namespace.
 * \author Julien Jorge
 */
#include <claw/smart_ptr.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <list>

/*----------------------------------------------------------------------------*/
/**
 * \brief Simple class, containing a string message. To illustrate the use of
 *        claw::memory.
 */
class message
{
public:
  message( const std::string& msg )
    : m_msg(msg)
  { }

  ~message()
  {
    std::cout << "deleting " << m_msg << std::endl;
  }

  const std::string& get_message() const { return m_msg; }

private:
  std::string m_msg;

}; // class message

/*----------------------------------------------------------------------------*/
/**
 * \brief Chained structure to show the problems occuring with the use of
 *        references count.
 */
struct item
{
  claw::memory::smart_ptr<item> next;
}; // struct item

/*----------------------------------------------------------------------------*/
/**
 * \brief Print a \a message stored in a smart_ptr. The parameter is passed by
 *        copy.
 */
void print_copy( claw::memory::smart_ptr<message> p )
{
  std::cout << "print_copy" << std::endl;
  std::cout << "  (->) " << p->get_message() << std::endl;
  std::cout << "  (*) " << (*p).get_message() << std::endl;
} // print_copy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Print a \a message stored in a smart_ptr. The parameter is passed by
 *        reference.
 */
void print_ref( claw::memory::smart_ptr<message>& p )
{
  std::cout << "print_ref" << std::endl;
  std::cout << "  (->) " << p->get_message() << std::endl;
  std::cout << "  (*) " << (*p).get_message() << std::endl;
} // print_ref()

/*----------------------------------------------------------------------------*/
/**
 * \brief Print a \a message stored in a smart_ptr. The parameter is passed by
 *        reference (to test the constant methods of smart_ptr).
 */
void print_const( const claw::memory::smart_ptr<message>& p )
{
  std::cout << "print_const" << std::endl;
  std::cout << "  (->) " << p->get_message() << std::endl;
  std::cout << "  (*) " << (*p).get_message() << std::endl;
} // print_const()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create some smart_ptr, call print_*() methods and do some operations.
 */
void basic_test()
{
  std::cout << "----------------- basic_test" << std::endl;

  claw::memory::smart_ptr<message> p = new message( "basic_1" );
  claw::memory::smart_ptr<message> q = new message( "basic_2" );
  claw::memory::smart_ptr<message> p2(p);
  claw::memory::smart_ptr<message> q2;
  claw::memory::smart_ptr<message> r( NULL );

  print_copy(p);
  print_ref(p);
  print_const(p);

  std::cout << "instruction: p = p = p" << std::endl;
  p = p = p;

  std::cout << "instruction: q2 = q" << std::endl;
  q2 = q;
} // basic_test()

/*----------------------------------------------------------------------------*/
/**
 * \brief Use a smart_ptr with a class from the STL.
 */
void stl_test()
{
  std::cout << "----------------- stl_test" << std::endl;
  std::list< claw::memory::smart_ptr<message> > list;
  std::list< claw::memory::smart_ptr<message> >::const_iterator it;

  for ( unsigned int i=0; i!=10; ++i )
    {
      std::ostringstream oss;
      oss << i;

      list.push_back( new message(oss.str()) );
    }

  for ( it=list.begin(); it!=list.end(); ++it )
    std::cout << (*it)->get_message();

  std::cout << std::endl;
} // stl_test()

/*----------------------------------------------------------------------------*/
/**
 * \brief Example of the problems occuring with the use of references count.
 */
void loop_test()
{
  std::cout << "----------------- loop_test" << std::endl;
  claw::memory::smart_ptr<item> i1 = new item;
  claw::memory::smart_ptr<item> i2 = new item;

  i2->next = i1;
  i1->next = i2;
} // test_boucle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Main program. Call test methods.
 */
int main()
{
  basic_test();
  stl_test();
  loop_test();

  return 0;
} // main()
