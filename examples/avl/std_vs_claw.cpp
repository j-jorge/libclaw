/**
 * \file main.cpp
 * \brief Example program to show the use of the the claw::avl class.
 *
 * The program take avl values from a file and do some operations.
 *
 * \author Julien Jorge
 */
#include <claw/avl.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <cmath>
#include <set>
#include <vector>
#include <sys/time.h>

/*----------------------------------------------------------------------------*/
template<typename SetType>
unsigned int time_insert( SetType& s, std::vector<int> data )
{
  timeval beg, end;
  const unsigned int n = data.size();
  s.clear();

  gettimeofday(&beg, NULL);

  for (unsigned int i=0; i!=n; ++i)
    s.insert(data[i]);

  gettimeofday(&end, NULL);

  SetType c(s);

  return
    ( (end.tv_sec * 1000000 + end.tv_usec)
      - (beg.tv_sec * 1000000 + beg.tv_usec) ) / 1000;
} // time_insert()

/*----------------------------------------------------------------------------*/
template<typename SetType>
unsigned int time_insert_it( SetType& s, std::vector<int> data )
{
  timeval beg, end;
  s.clear();

  gettimeofday(&beg, NULL);

  s.insert(data.begin(), data.end());

  gettimeofday(&end, NULL);

  return
    ( (end.tv_sec * 1000000 + end.tv_usec)
      - (beg.tv_sec * 1000000 + beg.tv_usec) ) / 1000;
} // time_insert_it()

/*----------------------------------------------------------------------------*/
template<typename SetType>
unsigned int time_iterator( SetType& s )
{
  timeval beg, end;
  unsigned int v(0);

  gettimeofday(&beg, NULL);

  typename SetType::const_iterator it;
  const typename SetType::const_iterator eit = s.end();

  for (it=s.begin(); it!=eit; ++it )
    v += *it;

  gettimeofday(&end, NULL);

  return
    ( (end.tv_sec * 1000000 + end.tv_usec)
      - (beg.tv_sec * 1000000 + beg.tv_usec) ) / 1000;
} // time_iterator()

/*----------------------------------------------------------------------------*/
template<typename SetType>
unsigned int time_copy( SetType& s )
{
  timeval beg, end;
  SetType c;

  gettimeofday(&beg, NULL);

  c = s;

  gettimeofday(&end, NULL);

  return
    ( (end.tv_sec * 1000000 + end.tv_usec)
      - (beg.tv_sec * 1000000 + beg.tv_usec) ) / 1000;
} // time_copy()

/*----------------------------------------------------------------------------*/
template<typename SetType>
unsigned int time_find( SetType& s, std::vector<int> data )
{
  timeval beg, end;
  unsigned int v(0);
  const unsigned int n = data.size();
  const typename SetType::const_iterator eit = s.end();

  gettimeofday(&beg, NULL);

  for (unsigned int i=0; i!=n; ++i)
    if ( s.find(data[i]) != eit )
      v += i;
    else if ( s.find(i) != eit )
      v += i;
  
  gettimeofday(&end, NULL);

  return
    ( (end.tv_sec * 1000000 + end.tv_usec)
      - (beg.tv_sec * 1000000 + beg.tv_usec) ) / 1000;
} // time_find()

/*----------------------------------------------------------------------------*/
template<typename SetType>
unsigned int time_clear( SetType& s )
{
  timeval beg, end;

  gettimeofday(&beg, NULL);

  s.clear();

  gettimeofday(&end, NULL);

  return
    ( (end.tv_sec * 1000000 + end.tv_usec)
      - (beg.tv_sec * 1000000 + beg.tv_usec) ) / 1000;
} // time_clear()

/*----------------------------------------------------------------------------*/
void do_bench( const std::vector<int>& data )
{
  std::set<int> s;
  claw::avl<int> a;

  std::cout << data.size() << '\t'
            << time_insert(a, data) << ' ' << time_insert(s, data) << '\t'
            << time_insert_it(a, data) << ' ' << time_insert_it(s, data) << '\t'
            << time_iterator(a) << ' ' << time_iterator(s) << '\t'
            << time_copy(a) << ' ' << time_copy(s) << '\t'
            << time_find(a, data) << ' ' << time_find(s, data) << '\t'
            << time_clear(a) << ' ' << time_clear(s) << std::endl;
} // do_bench()

/*----------------------------------------------------------------------------*/
void benchmark( std::vector<int>& data )
{
  std::cout << "# milliseconds (avl, std::set)\n";
  std::cout << "# n insert insert(it) iterator copy find clear\n";
  do_bench(data);

  std::cout << "# sorted inc\n";
  std::sort( data.begin(), data.end() );
  do_bench(data);

  std::cout << "# sorted dec\n";
  std::sort( data.begin(), data.end(), std::greater<int>() );
  do_bench(data);
} // benchmark()

int main( int argc, char* argv[] )
{
  srand(time(NULL));

  if (argc != 2)
    {
      std::cout << argv[0] << " count" << std::endl;
      return 1;
    }
  else
    {
      std::istringstream iss( argv[1] );
      unsigned int n;

      if ( !(iss >> n) )
        {
          std::cerr << "not a number " << argv[1] << std::endl;
          return 1;
        }
      else
        {
          std::vector<int> data;
          data.reserve(n);

          for (unsigned int i=0; i!=n; ++i)
            data.push_back( rand() );
          
          benchmark(data);
        }
    }

  return 0;
} // main()
