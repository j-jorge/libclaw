/**
 * \file main.cpp
 * \brief This program shows basic use of the claw::graph class.
 * \author Julien Jorge
 */
#include <algorithm>
#include <claw/graph.hpp>
#include <fstream>
#include <iostream>
#include <list>

/**
 * \brief This class is a vertex in the graph.
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
 * \brief An edge in the graph.
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

/**
 * \brief Write a graph in a stream.
 */
template <class S, class A, class Comp>
std::ostream& operator<<(std::ostream& os, const claw::graph<S, A, Comp>& g)
{
  typename claw::graph<S, A, Comp>::vertex_iterator it_s;
  typename claw::graph<S, A, Comp>::edge_iterator it_e;

  // vertices
  os << "V = {";
  for(it_s = g.vertex_begin(); it_s != g.vertex_end(); ++it_s)
    {
      os << " " << *it_s;
    }
  os << " }" << std::endl;

  // edges
  os << "E = {" << std::endl;
  for(it_e = g.edge_begin(); it_e != g.edge_end(); ++it_e)
    os << " [" << it_e->label() << ", " << it_e->source() << ", "
       << it_e->target() << "]" << std::endl;
  os << " }" << std::endl;

  return os;
}

/**
 * \brief Write some informations about a graph.
 */
void graph_infos(const claw::graph<vertex, edge>& g)
{
  std::vector<vertex> v;
  g.vertices(v);

  std::cout << g.vertices_count() << " vertices" << std::endl;
  std::cout << g.edges_count() << " edges" << std::endl;

  for(unsigned int i = 0; i != v.size(); ++i)
    std::cout << "vertex: " << v[i].val
              << " outer degree: " << g.outer_degree(v[i])
              << " inner degree: " << g.inner_degree(v[i]) << std::endl;

  std::cout << "adjacency list" << std::endl;

  for(unsigned int i = 0; i != v.size(); ++i)
    {
      std::vector<vertex> d;
      g.neighbours(v[i], d);

      std::cout << v[i].val << " : ";

      for(unsigned int j = 0; j != d.size(); ++j)
        std::cout << d[j].val << " (" << g.label(v[i], d[j]).symbol << "); ";

      std::cout << std::endl;
    }
}

/**
 * \brief Test the reverse iterator on vertices.
 */
void graph_rit_vertices(const claw::graph<vertex, edge>& g)
{
  std::cout << ":: reverse vertex iterator: " << std::endl;

  claw::graph<vertex, edge>::reverse_vertex_iterator it_s;

  std::cout << "=========== increment ++ (end to begin)" << std::endl;
  for(it_s = g.vertex_rbegin(); it_s != g.vertex_rend(); ++it_s)
    std::cout << *it_s << ", ";
  std::cout << std::endl;
  std::cout << "=========== decrement -- (begin to end)" << std::endl;
  it_s = g.vertex_rend();
  do
    {
      --it_s;
      std::cout << *it_s << ", ";
    }
  while(it_s != g.vertex_rbegin());
  std::cout << std::endl;
}

/**
 * \brief Test the iterator on vertices.
 */
void graph_it_vertices(const claw::graph<vertex, edge>& g)
{
  std::cout << ":: vertex iterator: " << std::endl;

  claw::graph<vertex, edge>::vertex_iterator it_s;
  std::list<vertex> v1, v2;

  std::cout << "=========== forward" << std::endl;
  for(it_s = g.vertex_begin(); it_s != g.vertex_end(); ++it_s)
    {
      std::cout << *it_s << ", ";
      v1.push_back(*it_s);
    }
  std::cout << std::endl;

  std::cout << "=========== then come back to first" << std::endl;
  do
    {
      --it_s;
      std::cout << *it_s << ", ";
      v2.push_front(*it_s);
    }
  while(it_s != g.vertex_begin());
  std::cout << std::endl;

  if(std::equal(v1.begin(), v1.end(), v2.begin()))
    std::cout << "OK" << std::endl;
  else
    std::cout << "Problem" << std::endl;

  v1.clear();
  v2.clear();

  std::cout << "=========== backward" << std::endl;
  it_s = g.vertex_end();
  do
    {
      --it_s;
      std::cout << *it_s << ", ";
      v1.push_front(*it_s);
    }
  while(it_s != g.vertex_begin());
  std::cout << std::endl;

  std::cout << "=========== then come back to the end" << std::endl;
  for(; it_s != g.vertex_end(); ++it_s)
    {
      std::cout << *it_s << ", ";
      v2.push_back(*it_s);
    }
  std::cout << std::endl;

  if(std::equal(v1.begin(), v1.end(), v2.begin()))
    std::cout << "OK" << std::endl;
  else
    std::cout << "Problem" << std::endl;

  vertex s;
  std::cout << "scan from vertex=";
  std::cin >> s.val;

  std::cout << "=========== to the end" << std::endl;
  for(it_s = g.vertex_begin(s); it_s != g.vertex_end(); ++it_s)
    std::cout << *it_s << ", ";
  std::cout << std::endl;

  std::cout << "=========== to the beginning" << std::endl;
  it_s = g.vertex_begin(s);
  if(it_s != g.vertex_begin())
    do
      {
        --it_s;
        std::cout << *it_s << ", ";
      }
    while(it_s != g.vertex_begin());
  std::cout << std::endl;
}

/**
 * \brief Test the reverse iterator on edges.
 */
void graph_rit_edges(const claw::graph<vertex, edge>& g)
{
  std::cout << ":: reverse edge iterator: " << std::endl;

  claw::graph<vertex, edge>::reverse_edge_iterator it_a;

  std::cout << "=========== increment ++ (end to begin)" << std::endl;
  for(it_a = g.edge_rbegin(); it_a != g.edge_rend(); ++it_a)
    std::cout << "<" << it_a->source() << "; " << it_a->label()
              << it_a->target() << ">, ";
  std::cout << std::endl;
  std::cout << "=========== decrement -- (begin to end)" << std::endl;
  it_a = g.edge_rend();
  do
    {
      --it_a;
      std::cout << "<" << it_a->source() << "; " << it_a->label()
                << it_a->target() << ">, ";
    }
  while(it_a != g.edge_rbegin());
  std::cout << std::endl;
}

/**
 * \brief Test the iterator on edges.
 */
void graph_it_edges(const claw::graph<vertex, edge>& g)
{
  std::cout << ":: edge iterator: " << std::endl;

  claw::graph<vertex, edge>::edge_iterator it_a;
  std::list<edge> v1, v2;

  std::cout << "=========== forward" << std::endl;
  for(it_a = g.edge_begin(); it_a != g.edge_end(); ++it_a)
    {
      std::cout << "<" << it_a->source() << "; " << it_a->label()
                << it_a->target() << ">, ";
      v1.push_back(it_a->label());
    }
  std::cout << std::endl;

  std::cout << "=========== then come back to the first" << std::endl;
  do
    {
      --it_a;
      std::cout << "<" << it_a->source() << "; " << it_a->label()
                << it_a->target() << ">, ";
      v2.push_front(it_a->label());
    }
  while(it_a != g.edge_begin());
  std::cout << std::endl;

  if(std::equal(v1.begin(), v1.end(), v2.begin()))
    std::cout << "OK" << std::endl;
  else
    std::cout << "Problem" << std::endl;

  v1.clear();
  v2.clear();

  std::cout << "=========== backward" << std::endl;
  it_a = g.edge_end();
  do
    {
      --it_a;
      std::cout << "<" << it_a->source() << "; " << it_a->label()
                << it_a->target() << ">, ";
      v1.push_front(it_a->label());
    }
  while(it_a != g.edge_begin());
  std::cout << std::endl;

  std::cout << "=========== then come back to the end" << std::endl;
  for(; it_a != g.edge_end(); ++it_a)
    {
      std::cout << "<" << it_a->source() << "; " << it_a->label()
                << it_a->target() << ">, ";
      v2.push_back(it_a->label());
    }
  std::cout << std::endl;

  if(std::equal(v1.begin(), v1.end(), v2.begin()))
    std::cout << "OK" << std::endl;
  else
    std::cout << "Problem" << std::endl;

  vertex s1, s2;
  std::cout << "scan from the edge starting at vertex=";
  std::cin >> s1.val;
  std::cout << "and ending at vertex=";
  std::cin >> s2.val;

  std::cout << "=========== to the end" << std::endl;
  for(it_a = g.edge_begin(s1, s2); it_a != g.edge_end(); ++it_a)
    std::cout << "<" << it_a->source() << "; " << it_a->label()
              << it_a->target() << ">, ";
  std::cout << std::endl;

  std::cout << "=========== to the begining" << std::endl;
  it_a = g.edge_begin(s1, s2);
  if(it_a != g.edge_begin())
    do
      {
        --it_a;
        std::cout << "<" << it_a->source() << "; " << it_a->label()
                  << it_a->target() << ">, ";
      }
    while(it_a != g.edge_begin());
  std::cout << std::endl;
}

/**
 * \brief Load a graph and call the test methods.
 */
void test_graphs(const std::string& filename)
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
              std::cout << s1 << ": isolated" << std::endl;
              g.add_vertex(s1);
            }
        }

      f.close();

      std::cout << "        ----------------------------------- informations"
                << std::endl;
      graph_infos(g);

      std::cout << "        ------------------------------------------ print"
                << std::endl;
      std::cout << g << std::endl;

      std::cout << "        ------------------------------ iterator (states)"
                << std::endl;

      graph_it_vertices(g);

      std::cout << "        ------------------------------- iterator (edges)"
                << std::endl;

      graph_it_edges(g);

      std::cout << "        ---------------------- reverse iterator (states)"
                << std::endl;

      graph_rit_vertices(g);

      std::cout << "        ----------------------- reverse iterator (edges)"
                << std::endl;

      graph_rit_edges(g);
    }
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
          test_graphs(argv[1]);
        }
      catch(claw::graph_exception& e)
        {
          std::cerr << e.what() << std::endl;
        }

      return 0;
    }
}
