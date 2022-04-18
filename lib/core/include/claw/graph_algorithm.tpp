/*
  CLAW - a C++ Library Absolutely Wonderful

  CLAW is a free library without any particular aim but being useful to
  anyone.

  Copyright (C) 2005-2011 Julien Jorge

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  contact: julien.jorge@stuff-o-matic.com
*/
/**
 * \file graph_algorithm.tpp
 * \brief Graph algorithms implementation.
 * \author Julien Jorge
 */
#include <queue>
#include <stack>

/**
 * \brief Constructor.
 * \param g Graph to scan.
 * \param source Start_Vertexing vertex.
 * \param events User's processings.
 */
template <class Graph, class Events>
claw::breadth_scan<Graph, Events>::breadth_scan(const Graph& g,
                                                const vertex_type& source,
                                                Events& events)
  : m_g(g)
  , m_source(source)
  , m_events(events)
{}

/**
 * \brief Performs the scan.
 */
template <class Graph, class Events>
void claw::breadth_scan<Graph, Events>::operator()()
{
  coloration seen_vertices;
  std::queue<vertex_type> pending_vertices;
  vertex_type current_vertex;
  std::vector<vertex_type> neighbourhood;
  typename std::vector<vertex_type>::const_iterator it;

  m_events.init(m_g);

  for(vertex_iterator it_v = m_g.vertex_begin(); it_v != m_g.vertex_end();
      ++it_v)
    seen_vertices[*it_v] = 0;

  seen_vertices[m_source] = 1;
  pending_vertices.push(m_source);

  while(!pending_vertices.empty())
    {
      current_vertex = pending_vertices.front();
      m_events.start_vertex(current_vertex);

      m_g.neighbours(current_vertex, neighbourhood);

      for(it = neighbourhood.begin(); it != neighbourhood.end(); ++it)
        {
          if(seen_vertices[*it] == 0)
            {
              m_events.visit_edge(current_vertex, *it);
              seen_vertices[*it] = 1;
            }
        }

      pending_vertices.pop();
      m_events.end_vertex(current_vertex);
      seen_vertices[current_vertex] = 2;
    }
}

//****************************** depth_scan ***********************************

/**
 * \brief Constructor.
 * \param g Graph to scan.
 * \param events User's processings.
 */
template <class Graph, class Events>
claw::depth_scan<Graph, Events>::depth_scan(const Graph& g, Events& events)
  : m_g(g)
  , m_events(events)
{}

/**
 * \brief Performs the scan.
 */
template <class Graph, class Events>
void claw::depth_scan<Graph, Events>::operator()()
{
  coloration seen_vertices;
  vertex_iterator it;

  m_events.init(m_g);

  for(it = m_g.vertex_begin(); it != m_g.vertex_end(); ++it)
    seen_vertices[*it] = 0;

  for(it = m_g.vertex_begin(); it != m_g.vertex_end(); ++it)
    if(seen_vertices[*it] == 0)
      recursive_scan(*it, seen_vertices);
}

/**
 * \brief Performs the recursive part of the scan.
 */
template <class Graph, class Events>
void claw::depth_scan<Graph, Events>::recursive_scan(const vertex_type& s,
                                                     coloration& seen_vertices)
{
  std::vector<vertex_type> neighbourhood;
  typename std::vector<vertex_type>::const_iterator it;

  m_events.start_vertex(s);
  seen_vertices[s] = 1;

  m_g.neighbours(s, neighbourhood);

  for(it = neighbourhood.begin(); it != neighbourhood.end(); ++it)
    if(seen_vertices[*it] == 0)
      {
        m_events.visit_edge(s, *it);
        recursive_scan(*it, seen_vertices);
      }

  m_events.end_vertex(s);
  seen_vertices[s] = 2;
}

//********************** topological_sort ***********************************

/**
 * \brief Initialize the scan.
 * \param g The graph that will be scanned.
 */
template <class Graph>
void claw::topological_sort<Graph>::init(const Graph& g)
{
  m_result.resize(g.vertices_count());
  m_next_index = (int)g.vertices_count() - 1;
}

template <class Graph>
void claw::topological_sort<Graph>::end_vertex(const vertex_type& s)
{
  m_result[m_next_index] = s;
  --m_next_index;
}

template <class Graph>
void claw::topological_sort<Graph>::operator()(const Graph& g)
{
  claw::depth_scan<Graph, self_type> scan(g, *this);
  scan();
}

template <class Graph>
const typename claw::topological_sort<Graph>::vertex_type&
claw::topological_sort<Graph>::operator[](unsigned int index) const
{
  return m_result[index];
}

template <class Graph>
typename claw::topological_sort<Graph>::const_iterator
claw::topological_sort<Graph>::begin() const
{
  return m_result.begin();
}

template <class Graph>
typename claw::topological_sort<Graph>::const_iterator
claw::topological_sort<Graph>::end() const
{
  return m_result.end();
}
