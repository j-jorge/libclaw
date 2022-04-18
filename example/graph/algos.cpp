#include "claw/graph.hpp"
#include "claw/graph_algorithm.hpp"
#include <fstream>
#include <iostream>

/**
 * \brief A vertex of the graph.
 * \author Julien Jorge
 */
class vertex
{
public:
  int val;
  bool operator<(const vertex& s) const
  {
    return val < s.val;
  }
  bool operator==(const vertex& s) const
  {
    return val == s.val;
  }

  friend std::ostream& operator<<(std::ostream& os, const vertex& s)
  {
    return os << s.val;
  }
}; // class vertex

/**
 * \brief An edge of the graph
 * \author Julien Jorge
 */
class edge
{
public:
  char symbol;
  int weight;

  bool operator<(const edge& a) const
  {
    return weight < a.weight;
  }
  bool operator==(const edge& a) const
  {
    return (symbol == a.symbol) && (weight == a.weight);
  }

  friend std::ostream& operator<<(std::ostream& os, const edge& a)
  {
    return os << "[" << a.symbol << ", " << a.weight << "]";
  }
}; // class edge

class pl_scan_events : public claw::scan_events<claw::graph<vertex, edge> >
{
public:
  pl_scan_events(const vertex& s)
  {
    distances[s] = 0;
  }

  void visit_edge(const vertex& source, const vertex& dest)
  {
    parents[dest] = source;
    distances[dest] = distances[source] + 1;
  }

  std::map<vertex, unsigned int> distances;
  std::map<vertex, vertex> parents;
}; // class pl_scan_events

class pp_scan_events : public claw::scan_events<claw::graph<vertex, edge> >
{
public:
  pp_scan_events()
  {
    m_date = 0;
  }

  void start(const vertex& s)
  {
    ++m_date;
    dates[s].first = m_date;
  }

  void visit_edge(const vertex& source, const vertex& dest)
  {
    parents[dest] = source;
  }

  void end(const vertex& s)
  {
    ++m_date;
    dates[s].second = m_date;
  }

  std::map<vertex, std::pair<unsigned int, unsigned int> > dates;
  std::map<vertex, vertex> parents;

private:
  unsigned int m_date;
}; // class pp_scan_events

/**
 * \brief Output a text representation of a graph.
 */
template <class S, class A, class Comp>
std::ostream& operator<<(std::ostream& os, const claw::graph<S, A, Comp>& g)
{
  typename claw::graph<S, A, Comp>::vertex_iterator it_s;
  typename claw::graph<S, A, Comp>::edge_iterator it_e;

  // vertices
  os << "S = {";
  for(it_s = g.vertex_begin(); it_s != g.vertex_end(); ++it_s)
    {
      os << " " << *it_s;
    }
  os << " }" << std::endl;

  // edges
  os << "A = {" << std::endl;
  for(it_e = g.edge_begin(); it_e != g.edge_end(); ++it_e)
    os << " [" << it_e->label() << ", " << it_e->source() << ", "
       << it_e->target() << "]" << std::endl;
  os << " }" << std::endl;

  return os;
}

void test_pl(const claw::graph<vertex, edge>& g)
{
  vertex s;
  std::cout << "Distances, computed from vertex : " << std::endl;
  std::cin >> s.val;
  pl_scan_events events(s);
  claw::breadth_scan<claw::graph<vertex, edge>, pl_scan_events> scan(g, s,
                                                                     events);

  scan();

  std::map<vertex, unsigned int>::const_iterator it_d;
  for(it_d = events.distances.begin(); it_d != events.distances.end(); ++it_d)
    std::cout << it_d->first.val << " to " << it_d->second << std::endl;

  std::map<vertex, vertex>::const_iterator it_s;
  for(it_s = events.parents.begin(); it_s != events.parents.end(); ++it_s)
    std::cout << it_s->second.val << " parent of " << it_s->first.val
              << std::endl;
}

void test_pp(const claw::graph<vertex, edge>& g)
{
  pp_scan_events events;
  claw::depth_scan<claw::graph<vertex, edge>, pp_scan_events> scan(g, events);

  scan();

  std::map<vertex, std::pair<unsigned int, unsigned int> >::const_iterator
      it_d;
  for(it_d = events.dates.begin(); it_d != events.dates.end(); ++it_d)
    std::cout << it_d->first.val << " : " << it_d->second.first << ","
              << it_d->second.second << std::endl;

  std::map<vertex, vertex>::const_iterator it_s;
  for(it_s = events.parents.begin(); it_s != events.parents.end(); ++it_s)
    std::cout << it_s->second.val << " parent of " << it_s->first.val
              << std::endl;
}

void test_topological_sort(const claw::graph<vertex, edge>& g)
{
  claw::topological_sort<claw::graph<vertex, edge> > sort;
  claw::topological_sort<claw::graph<vertex, edge> >::const_iterator it;

  sort(g);

  for(it = sort.begin(); it != sort.end(); ++it)
    std::cout << *it << std::endl;
}

bool test_graphs(const std::string& filename)
{
  claw::graph<vertex, edge> g;
  vertex s1, s2;
  edge a;
  std::ifstream f(filename.c_str());

  if(f)
    {
      while(f >> s1.val)
        {
          f >> s2.val;

          if(s2.val > 0)
            {
              f >> a.symbol >> a.weight;
              g.add_edge(s1, s2, a);
            }
          else
            {
              std::cout << s1 << " : isolé" << std::endl;
              g.add_vertex(s1);
            }
        }

      f.close();

      std::cout << "        -------------------------------------- display"
                << std::endl;
      std::cout << g << std::endl;

      std::cout << "        ------------------- breadth scan" << std::endl;

      test_pl(g);

      std::cout << "        ------------------- deep scan" << std::endl;

      test_pp(g);

      std::cout << "        ------------------- topological order"
                << std::endl;

      test_topological_sort(g);

      return true;
    }
  else
    return false;
}

int main(int argc, char* argv[])
{
  if(argc != 2)
    {
      std::cout << argv[0] << " test_file" << std::endl;
      return 1;
    }
  else
    {
      try
        {
          if(!test_graphs(argv[1]))
            std::cerr << "Can't process file '" << argv[1] << '\''
                      << std::endl;
        }
      catch(claw::graph_exception& e)
        {
          std::cerr << e.what() << std::endl;
        }

      return 0;
    }
}
