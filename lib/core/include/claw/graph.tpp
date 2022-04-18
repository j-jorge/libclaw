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
 * \file graph.tpp
 * \brief Implementation of the claw::graph class.
 * \author Julien Jorge
 */
#include <algorithm>
#include <cassert>

#include <claw/functional.hpp>

/**
 * \brief Constructor of the graph_vertex_iterator class.
 */
template <class S, class A, class Comp>
claw::graph<S, A, Comp>::graph_vertex_iterator::graph_vertex_iterator()
{}

/**
 * \brief Preincrement.
 * \pre Iterator is not at the end of the container.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::graph_vertex_iterator&
claw::graph<S, A, Comp>::graph_vertex_iterator::operator++()
{
  ++m_iterator;
  return *this;
}

/**
 * \brief Postincrement.
 * \pre Iterator is not at the end of the container.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::graph_vertex_iterator
claw::graph<S, A, Comp>::graph_vertex_iterator::operator++(int)
{
  graph_vertex_iterator it_tmp(*this);
  m_iterator++;
  return *this;
}

/**
 * \brief Predecrement.
 * \pre Iterator is not at the begining of the container.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::graph_vertex_iterator&
claw::graph<S, A, Comp>::graph_vertex_iterator::operator--()
{
  --m_iterator;
  return *this;
}

/**
 * \brief Postdecrement.
 * \pre Iterator is not at the begining of the container.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::graph_vertex_iterator
claw::graph<S, A, Comp>::graph_vertex_iterator::operator--(int)
{
  graph_vertex_iterator it_tmp(*this);
  m_iterator--;
  return it_tmp;
}

/**
 * \brief Dereference.
 * \pre Iterator is not at the end of the container.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::graph_vertex_iterator::reference
claw::graph<S, A, Comp>::graph_vertex_iterator::operator*() const
{
  return m_iterator->first;
}

/**
 * \brief Reference.
 * \pre Iterator is not at the end of the container.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::graph_vertex_iterator::pointer
claw::graph<S, A, Comp>::graph_vertex_iterator::operator->() const
{
  return &(m_iterator->first);
}

/**
 * \brief Equality.
 * \param it Iterator to compare to.
 * \pre Iterator and it are not at the end of their respective  containers.
 */
template <class S, class A, class Comp>
bool claw::graph<S, A, Comp>::graph_vertex_iterator::operator==(
    const graph_vertex_iterator& it) const
{
  return m_iterator == it.m_iterator;
}

/**
 * \brief Difference.
 * \param it Iterator to compare to.
 * \pre Iterator and it are not at the end of their respective  containers.
 */
template <class S, class A, class Comp>
bool claw::graph<S, A, Comp>::graph_vertex_iterator::operator!=(
    const graph_vertex_iterator& it) const
{
  return m_iterator != it.m_iterator;
}

/**
 * \brief Constructor with an iterator on graph class data.
 * \param it Iterator where scan starts.
 */
template <class S, class A, class Comp>
claw::graph<S, A, Comp>::graph_vertex_iterator::graph_vertex_iterator(
    typename graph_content::const_iterator it)
  : m_iterator(it)
{}

/**
 * \brief Constructor.
 */
template <class S, class A, class Comp>
claw::graph<S, A, Comp>::graph_edge_iterator::edge::edge()
  : m_label(NULL)
  , m_source(NULL)
  , m_target(NULL)
{}

/**
 * \brief Gets edge's label.
 */
template <class S, class A, class Comp>
const typename claw::graph<S, A, Comp>::edge_type&
claw::graph<S, A, Comp>::graph_edge_iterator::edge::label() const
{
  assert(m_label != NULL);
  return *m_label;
}

/**
 * \brief Gets edge's source.
 */
template <class S, class A, class Comp>
const typename claw::graph<S, A, Comp>::vertex_type&
claw::graph<S, A, Comp>::graph_edge_iterator::edge::source() const
{
  assert(m_source != NULL);
  return *m_source;
}

/**
 * \brief Gets edge's target.
 */
template <class S, class A, class Comp>
const typename claw::graph<S, A, Comp>::vertex_type&
claw::graph<S, A, Comp>::graph_edge_iterator::edge::target() const
{
  assert(m_target != NULL);
  return *m_target;
}

/**
 * \brief Sets label, source and taget.
 */
template <class S, class A, class Comp>
void claw::graph<S, A, Comp>::graph_edge_iterator::edge::set(
    const edge_type& l, const vertex_type& s, const vertex_type& t)
{
  m_label = &l;
  m_source = &s;
  m_target = &t;
}

/**
 * \brief Constructor of the graph_edge_iterator class.
 */
template <class S, class A, class Comp>
claw::graph<S, A, Comp>::graph_edge_iterator::graph_edge_iterator()
{}

/**
 * \brief Preincrement.
 * \pre Iterator is not at the end of the container.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::graph_edge_iterator&
claw::graph<S, A, Comp>::graph_edge_iterator::operator++()
{
  bool ok = true;
  ++m_neighbours_iterator;

  // end of a neighbourhood
  if(m_neighbours_iterator == m_vertex_iterator->second.end())
    {
      // find next edge or end.
      ok = false;
      ++m_vertex_iterator;

      while((m_vertex_iterator != m_vertex_end) && !ok)
        if(!m_vertex_iterator->second.empty())
          {
            ok = true;
            m_neighbours_iterator = m_vertex_iterator->second.begin();
          }
        else
          ++m_vertex_iterator;
    }

  if(ok)
    m_edge.set(m_neighbours_iterator->second, m_vertex_iterator->first,
               m_neighbours_iterator->first);

  return *this;
}

/**
 * \brief Postincrement.
 * \pre Iterator is not at the end of the container.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::graph_edge_iterator
claw::graph<S, A, Comp>::graph_edge_iterator::operator++(int)
{
  graph_edge_iterator it_tmp(*this);
  ++(*this);
  return it_tmp;
}

/**
 * \brief Predecrement.
 * \pre Iterator is not at the begining of the container.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::graph_edge_iterator&
claw::graph<S, A, Comp>::graph_edge_iterator::operator--()
{
  bool ok = true;

  if(m_vertex_iterator == m_vertex_end)
    {
      --m_vertex_iterator;
      m_neighbours_iterator = m_vertex_iterator->second.end();
    }

  // begining of a neighbourhood
  if(m_neighbours_iterator == m_vertex_iterator->second.begin())
    {
      ok = false;
      // find previous edge or begining.
      while((m_vertex_iterator != m_vertex_begin) && !ok)
        {
          --m_vertex_iterator;
          if(!m_vertex_iterator->second.empty())
            {
              ok = true;
              m_neighbours_iterator = --m_vertex_iterator->second.end();
            }
        }
    }
  else
    --m_neighbours_iterator;

  if(!ok)
    m_vertex_iterator == m_vertex_end;
  else
    m_edge.set(m_neighbours_iterator->second, m_vertex_iterator->first,
               m_neighbours_iterator->first);

  return *this;
}

/**
 * \brief postdecrement.
 * \pre Iterator is not at the begining of the container.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::graph_edge_iterator
claw::graph<S, A, Comp>::graph_edge_iterator::operator--(int)
{
  graph_edge_iterator it_tmp(*this);
  --(*this);
  return it_tmp;
}

/**
 * \brief Reference.
 * \pre Iterator is not at the end of the container.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::graph_edge_iterator::reference
claw::graph<S, A, Comp>::graph_edge_iterator::operator*() const
{
  return m_edge;
}

/**
 * \brief Pointer.
 * \pre Iterator is not at the end of the container.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::graph_edge_iterator::pointer
claw::graph<S, A, Comp>::graph_edge_iterator::operator->() const
{
  return &m_edge;
}

/**
 * \brief Equality.
 * \param it Iterator to compare to.
 * \pre Iterator and it are not at the end of their respective  containers.
 */
template <class S, class A, class Comp>
bool claw::graph<S, A, Comp>::graph_edge_iterator::operator==(
    const graph_edge_iterator& it) const
{
  // both are empty
  if(m_vertex_begin == m_vertex_end)
    return (it.m_vertex_begin == it.m_vertex_end)
           && (m_vertex_begin == it.m_vertex_begin);
  else if(it.m_vertex_begin == it.m_vertex_end) // -it- is empty
    return false;
  else
    // none is empty, perheaps at the end ?
    if(m_vertex_iterator == m_vertex_end)
      return (it.m_vertex_iterator == it.m_vertex_end)
             && (m_vertex_begin == it.m_vertex_begin);
    else if(it.m_vertex_iterator == it.m_vertex_end)
      return false;
    else
      return m_neighbours_iterator == it.m_neighbours_iterator;
}

/**
 * \brief Difference.
 * \param it Iterator to compare to.
 * \pre Iterator and it are not at the end of their respective  containers.
 */
template <class S, class A, class Comp>
bool claw::graph<S, A, Comp>::graph_edge_iterator::operator!=(
    const graph_edge_iterator& it) const
{
  return !(*this == it);
}

/**
 * \brief Constructor with an iterator on graph class data.
 * \param it_begin Iterator on the first node.
 * \param it_end Iterator on the last node.
 * \param it_s Iterator on current edge's source.
 * \param it_d Iterator where scan starts.
 */
template <class S, class A, class Comp>
claw::graph<S, A, Comp>::graph_edge_iterator::graph_edge_iterator(
    typename graph_content::const_iterator it_begin,
    typename graph_content::const_iterator it_end,
    typename graph_content::const_iterator it_s,
    typename neighbours_list::const_iterator it_d)
  : m_vertex_begin(it_begin)
  , m_vertex_end(it_end)
  , m_vertex_iterator(it_s)
  , m_neighbours_iterator(it_d)
{
  if(m_vertex_begin != m_vertex_end)
    m_edge.set(m_neighbours_iterator->second, m_vertex_iterator->first,
               m_neighbours_iterator->first);
}

/**
 * \brief Constructor.
 */
template <class S, class A, class Comp>
claw::graph<S, A, Comp>::graph()
  : m_edges_count(0)
{}

/**
 * \brief Add an edge in the graph.
 * \param s1 Tail of the edge.
 * \param s2 Head of the edgre.
 * \param e The label on the edge.
 */
template <class S, class A, class Comp>
void claw::graph<S, A, Comp>::add_edge(const vertex_type& s1,
                                       const vertex_type& s2,
                                       const edge_type& e)
{
  if(!edge_exists(s1, s2))
    {
      // s2 is not a neighbor of s1
      ++m_edges_count;

      add_vertex(s1);
      add_vertex(s2);

      // in all cases, s2 as one more inner edge
      ++m_inner_degrees[s2];
    }

  m_edges[s1][s2] = e;
}

/**
 * \brief Add a vertex.
 * \param s The vertex to add.
 */
template <class S, class A, class Comp>
void claw::graph<S, A, Comp>::add_vertex(const vertex_type& s)
{
  std::pair<S, neighbours_list> p;

  if(m_edges.find(s) == m_edges.end())
    {
      // Add the vertex in the adjacency list.
      p.first = s;
      m_edges.insert(p);
      m_inner_degrees[s] = 0;
    }
}

/**
 * \brief Check if there is an edge linking to vertices.
 * \param s Vertex at the tail of the edge.
 * \param r Vertex at the head of the edge.
 */
template <class S, class A, class Comp>
bool claw::graph<S, A, Comp>::edge_exists(const vertex_type& s,
                                          const vertex_type& r) const
{
  typename graph_content::const_iterator it = m_edges.find(s);

  if(it == m_edges.end())
    return false;
  else
    return it->second.find(r) != it->second.end();
}

/**
 * \brief Get the neighbors of a vertex.
 * \param s The vertex.
 * \param v (out) The neighbors.
 */
template <class S, class A, class Comp>
void claw::graph<S, A, Comp>::neighbours(const vertex_type& s,
                                         std::vector<vertex_type>& v) const
{
  typename graph_content::const_iterator it_s = m_edges.find(s);
  v.clear();

  if(it_s != m_edges.end())
    {
      v.resize(it_s->second.size());
      std::transform(it_s->second.begin(), it_s->second.end(), v.begin(),
                     const_first<S, A>());
    }
}

/**
 * \brief Get all the vertices.
 * \param v (out) The vertices.
 */
template <class S, class A, class Comp>
void claw::graph<S, A, Comp>::vertices(std::vector<vertex_type>& v) const
{
  v.clear();
  v.resize(m_edges.size());

  std::transform(m_edges.begin(), m_edges.end(), v.begin(),
                 const_first<S, neighbours_list>());
}

/**
 * \brief Get a node iterator on the first node.
 * \remark Returns vertex_end() if graph is empty.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::vertex_iterator
claw::graph<S, A, Comp>::vertex_begin() const
{
  return vertex_iterator(m_edges.begin());
}

/**
 * \brief Get a node iterator past the last node.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::vertex_iterator
claw::graph<S, A, Comp>::vertex_end() const
{
  return vertex_iterator(m_edges.end());
}

/**
 * \brief Get a node iterator on a particular node.
 * \remark Returns vertex_end() if S is not found.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::vertex_iterator
claw::graph<S, A, Comp>::vertex_begin(const vertex_type& s) const
{
  return vertex_iterator(m_edges.find(s));
}

/**
 * \brief Get a reverse node iterator on the first node.
 * \remark Returns vertex_rend() if graph is empty.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::reverse_vertex_iterator
claw::graph<S, A, Comp>::vertex_rbegin() const
{
  return reverse_vertex_iterator(vertex_end());
}

/**
 * \brief Get a reverse node iterator past the last node.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::reverse_vertex_iterator
claw::graph<S, A, Comp>::vertex_rend() const
{
  return reverse_vertex_iterator(vertex_begin());
}

/**
 * \brief Get a reverse node iterator just after a particular node.
 * \remark Returns vertex_rend() if s is not found.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::reverse_vertex_iterator
claw::graph<S, A, Comp>::vertex_rbegin(const vertex_type& s) const
{
  vertex_iterator it = vertex_begin(s);

  if(it != vertex_end())
    ++it;

  return reverse_vertex_iterator(it);
}

/**
 * \brief Get an edge iterator on the first edge.
 * \remark Returns edge_end() if there's no edge in the graph.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::edge_iterator
claw::graph<S, A, Comp>::edge_begin() const
{
  bool ok = false;
  typename graph_content::const_iterator it_s;
  it_s = m_edges.begin();

  while((it_s != m_edges.end()) && !ok)
    if(it_s->second.empty())
      ++it_s;
    else
      ok = true;

  if(ok)
    return edge_iterator(m_edges.begin(), m_edges.end(), it_s,
                         it_s->second.begin());
  else
    return edge_end();
}

/**
 * \brief Get an edge iterator after the last edge.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::edge_iterator
claw::graph<S, A, Comp>::edge_end() const
{
  return edge_iterator(m_edges.begin(), m_edges.end(), m_edges.end(),
                       typename neighbours_list::const_iterator());
}

/**
 * \brief Get en iterator on a particular edge .
 * \remark Returns edge_end() if edge (s1,s2) is not found.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::edge_iterator
claw::graph<S, A, Comp>::edge_begin(const vertex_type& s1,
                                    const vertex_type& s2) const
{
  if(edge_exists(s1, s2))
    {
      typename graph_content::const_iterator it_s1;

      it_s1 = m_edges.find(s1);
      return edge_iterator(m_edges.begin(), m_edges.end(), it_s1,
                           it_s1->second.find(s2));
    }
  else
    return edge_end();
}

/**
 * \brief Get a reverse edge iterator on the first edge.
 * \remark Returns redge_end() if there's no edge in the graph.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::reverse_edge_iterator
claw::graph<S, A, Comp>::edge_rbegin() const
{
  return reverse_edge_iterator(edge_end());
}

/**
 * \brief Get a reverse edge iterator after the last edge.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::reverse_edge_iterator
claw::graph<S, A, Comp>::edge_rend() const
{
  return reverse_edge_iterator(edge_begin());
}

/**
 * \brief Get a reverse edge iterator on a particular edge.
 * \remark Returns redge_end() if edge (s1,s2) is not found.
 */
template <class S, class A, class Comp>
typename claw::graph<S, A, Comp>::reverse_edge_iterator
claw::graph<S, A, Comp>::edge_rbegin(const vertex_type& s1,
                                     const vertex_type& s2) const
{
  reverse_edge_iterator it = edge_begin(s1, s2);

  if(it != edge_end())
    ++it;

  return reverse_edge_iterator(it);
}

/**
 * \brief Get the label of an edge.
 * \param s The vertex at the tail of the edge.
 * \param r The vertex at the head of the edge.
 */
template <class S, class A, class Comp>
const typename claw::graph<S, A, Comp>::edge_type&
claw::graph<S, A, Comp>::label(const vertex_type& s,
                               const vertex_type& r) const
{
  typename graph_content::const_iterator it_s = m_edges.find(s);

  if(it_s == m_edges.end())
    throw graph_exception("claw::graph::label(): unkonwn source vertex.");
  else
    {
      typename neighbours_list::const_iterator it_r = it_s->second.find(r);

      if(it_r == it_s->second.end())
        throw graph_exception(
            "claw::graph::label(): destination is not a neighbor.");
      else
        return it_r->second;
    }
}

/**
 * \brief Get the outter degree of a vertex.
 * \param s The vertex.
 */
template <class S, class A, class Comp>
std::size_t claw::graph<S, A, Comp>::outer_degree(const vertex_type& s) const
{
  typename graph_content::const_iterator it = m_edges.find(s);

  if(it == m_edges.end())
    throw graph_exception("claw::graph::outer_degree(): unknown vertex.");
  else
    return it->second.size();
}

/**
 * \brief Get the inner degree of a vertex.
 * \param s The vertex
 */
template <class S, class A, class Comp>
std::size_t claw::graph<S, A, Comp>::inner_degree(const vertex_type& s) const
{
  typename std::map<S, std::size_t, Comp>::const_iterator it;
  it = m_inner_degrees.find(s);

  if(it == m_inner_degrees.end())
    throw graph_exception("claw::graph::inner_degree(): unkown vertex.");
  else
    return it->second;
}

/**
 * \brief Get the number of vertices.
 */
template <class S, class A, class Comp>
std::size_t claw::graph<S, A, Comp>::vertices_count() const
{
  return m_edges.size();
}

/**
 * \brief Get the number of edges.
 */
template <class S, class A, class Comp>
std::size_t claw::graph<S, A, Comp>::edges_count() const
{
  return m_edges_count;
}
