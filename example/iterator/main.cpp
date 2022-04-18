#include <claw/functional.hpp>
#include <claw/iterator.hpp>
#include <iostream>
#include <list>
#include <utility>
#include <vector>

void bi_test()
{
  typedef std::list<std::pair<int, int> > container;

  container v;

  v.push_back(std::pair<int, int>(1, 1));
  v.push_back(std::pair<int, int>(2, 4));
  v.push_back(std::pair<int, int>(3, 9));
  v.push_back(std::pair<int, int>(4, 61));

  typedef claw::wrapped_iterator<int, container::iterator,
                                 claw::second<int, int> >::iterator_type
      iterator;

  iterator it;
  for(it = v.begin(); it != v.end(); ++it)
    std::cout << *it << ' ';

  std::cout << std::endl;

  do
    {
      --it;
      std::cout << *it << ' ';
    }
  while(it != v.begin());

  std::cout << std::endl;
}

void random_test()
{
  typedef std::vector<std::pair<int, int> > container;

  container v;

  v.push_back(std::pair<int, int>(1, 1));
  v.push_back(std::pair<int, int>(2, 4));
  v.push_back(std::pair<int, int>(3, 9));
  v.push_back(std::pair<int, int>(4, 61));

  typedef claw::wrapped_iterator<int, container::iterator,
                                 claw::second<int, int> >::iterator_type
      iterator;

  iterator it;
  for(it = v.begin(); it != v.end(); ++it)
    std::cout << *it << ' ';

  std::cout << std::endl;

  it = v.begin();
  *(it + 3) = 18;

  for(it = v.begin(); it != v.end(); ++it)
    std::cout << *it << ' ';

  std::cout << std::endl;

  *(it - 1) = 17;

  for(it = v.begin(); it != v.end(); ++it)
    std::cout << *it << ' ';

  std::cout << std::endl;

  it = v.begin();
  it[3] = 16;

  for(it = v.begin(); it != v.end(); ++it)
    std::cout << *it << ' ';

  std::cout << std::endl;
}

void complicated_test()
{
  typedef std::vector<std::pair<int, int>*> container;

  container v;

  v.push_back(new std::pair<int, int>(1, 1));
  v.push_back(new std::pair<int, int>(2, 4));
  v.push_back(new std::pair<int, int>(3, 9));
  v.push_back(new std::pair<int, int>(4, 61));

  typedef claw::wrapped_iterator<
      int, container::iterator,
      claw::unary_compose<claw::second<int, int>,
                          claw::dereference<std::pair<int, int> > > >::
      iterator_type iterator;

  iterator it;
  for(it = v.begin(); it != v.end(); ++it)
    std::cout << *it << ' ';

  std::cout << std::endl;

  it = v.begin();
  *(it + 3) = 18;

  for(it = v.begin(); it != v.end(); ++it)
    std::cout << *it << ' ';

  std::cout << std::endl;

  *(it - 1) = 16;

  for(it = v.begin(); it != v.end(); ++it)
    std::cout << *it << ' ';

  std::cout << std::endl;
}

int main(int argc, char* argv[])
{
  bi_test();
  std::cout << "--\n";
  random_test();
  std::cout << "--\n";
  complicated_test();

  return 0;
}
