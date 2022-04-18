#include <claw/meta/type_list.hpp>
#include <claw/multi_type_map.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

struct custom
{
  int a;
  std::string s;

  custom()
  {}

  custom(int _a, const std::string& _s)
    : a(_a)
    , s(_s)
  {}
}; // class custom

std::ostream& operator<<(std::ostream& os, const custom& c)
{
  return os << "(" << c.a << ", " << c.s << ")";
}

using namespace claw;
using namespace claw::meta;

template <typename Map>
struct map_printer;

template <typename Key>
struct map_printer<multi_type_map<Key, no_type> >
{
  std::ostream& operator()(std::ostream& os,
                           const multi_type_map<Key, no_type>&) const
  {
    return os;
  }
}; // map_printer [no_type]

template <typename Key, typename Head, typename Queue>
struct map_printer<multi_type_map<Key, type_list<Head, Queue> > >
{
  typedef multi_type_map<Key, type_list<Head, Queue> > map_type;

  std::ostream& operator()(std::ostream& os, const map_type& m) const
  {
    typename map_type::template iterator<Head>::const_type it;

    for(it = m.template begin<Head>(); it != m.template end<Head>(); ++it)
      os << "(" << it->first << ", " << it->second << ") ";

    os << std::endl;

    map_printer<multi_type_map<Key, Queue> > rec;
    return rec(os, m);
  }
}; // map_printer [no_type]

template <typename Key, typename Head, typename Queue>
std::ostream& print_map(std::ostream& os,
                        const multi_type_map<Key, type_list<Head, Queue> >& m)
{
  typedef multi_type_map<Key, type_list<Head, Queue> > map_type;
  map_printer<map_type> p;
  return p(os, m);
}

typedef type_list_maker<float, bool, std::string, custom>::result my_type_list;
typedef multi_type_map<int, my_type_list> map_type;

void fill_map(map_type& m)
{
  m.set<bool>(0, false);
  m.set<bool>(1, true);
  m.set<float>(0, 3.14);
  m.set<float>(1, 3.14);
  m.set<float>(3, 3.14);
  m.set<float>(5, 3.14);
  m.set<float>(7, 3.14);
  m.set<float>(9, 3.14);
  m.set<custom>(0, custom(72, "seventy-two"));
  m.set<std::string>(1, "my favorite string");
  m.set<custom>(1, custom(24, "my favorite number"));
}

void test_erase()
{
  map_type m;
  fill_map(m);

  m.erase<bool>(0);
  m.erase<float>(0);
  m.erase<custom>(1);

  map_type::iterator<float>::type it, tmp;
  it = m.begin<float>();
  while(it != m.end<float>())
    {
      tmp = it;
      ++it;
      m.erase<float>(tmp);
    }

  std::cout << "[Test] erase" << std::endl;
  print_map(std::cout, m);
}

void test_get_set()
{
  map_type m;
  fill_map(m);
  std::cout << "[Test] get/set" << std::endl;
  print_map(std::cout, m);
}

void test_set_map()
{
  map_type m;
  fill_map(m);
  map_type m2;

  for(map_type::iterator<float>::type it = m.begin<float>();
      it != m.end<float>(); ++it)
    m2.set<float>(2 * it->first, 2 * it->second);

  m.set(m2);
  std::cout << "[Test] set_map" << std::endl;
  print_map(std::cout, m);
}

void test_size()
{
  map_type m;
  fill_map(m);
  std::cout << "[Test] size" << std::endl;
  std::cout << m.size() << std::endl;
}

int main(int argc, char* argv[])
{
  test_get_set();
  test_size();
  test_set_map();
  test_erase();

  return 0;
}
