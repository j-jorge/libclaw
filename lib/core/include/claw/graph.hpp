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
 * \file graph.hpp
 * \brief A class to represent a graph.
 * \author Julien Jorge
 */
#ifndef __CLAW_GRAPH_HPP__
#define __CLAW_GRAPH_HPP__

#include <claw/exception.hpp>
#include <claw/meta/no_type.hpp>

#include <exception>
#include <iterator>
#include <map>
#include <queue>
#include <utility>
#include <vector>

#include <cstddef>

namespace claw
{

  /**
   * \brief The exceptions thrown by the graphs.
   * \author Julien Jorge
   */
  typedef claw::exception graph_exception;

  /**
   * \brief A class to represent a graph.
   *
   * <b>Constraints on the template parameters:</b>
   *  - S is LessThanComparable,
   *  - A is any Assignable and Default Constructible,
   *  - Comp is a binary predicate such that Comp(S a, S b) == true if and
   *    only if a < b.
   *
   * \author Julien Jorge
   */
  template <class S, class A = meta::no_type, class Comp = std::less<S> >
  class graph
  {
  public:
    /** \brief Type of the vertices. */
    typedef S vertex_type;

    /** \brief Type of the edges. */
    typedef A edge_type;

    /** \brief Binary predicate to compare vertices. */
    typedef Comp vertex_compare;

    /** \brief The adjacency list of a vertex.
        vertex_type is the target vertex,
        edge_type is the label on the edge. */
    typedef std::map<vertex_type, edge_type, vertex_compare> neighbours_list;

    /** \brief The whole graph: an adjacency list for each vertex. */
    typedef std::map<vertex_type, neighbours_list, vertex_compare>
        graph_content;

    /** \brief Type of the current structure. */
    typedef claw::graph<vertex_type, edge_type, vertex_compare> self_type;

    /**
     * \brief Iterator on the graph's vertices.
     */
    class graph_vertex_iterator
    {
      friend class graph<vertex_type, edge_type, vertex_compare>;

    public:
      typedef const vertex_type value_type;
      typedef const vertex_type& reference;
      typedef const vertex_type* const pointer;
      typedef ptrdiff_t difference_type;

      typedef std::bidirectional_iterator_tag iterator_category;

    public:
      graph_vertex_iterator();

      graph_vertex_iterator& operator++();
      graph_vertex_iterator operator++(int);
      graph_vertex_iterator& operator--();
      graph_vertex_iterator operator--(int);
      reference operator*() const;
      pointer operator->() const;
      bool operator==(const graph_vertex_iterator& it) const;
      bool operator!=(const graph_vertex_iterator& it) const;

    private:
      explicit graph_vertex_iterator(
          typename graph_content::const_iterator it);

    private:
      /** \brief Iterator on the list of vertex. */
      typename graph_content::const_iterator m_iterator;

    }; // class graph_vertex_iterator

    /**
     * \brief Iterator on the graph's edges.
     */
    class graph_edge_iterator
    {
      friend class graph<vertex_type, edge_type, vertex_compare>;

    public:
      /**
       * \brief Value pointed by the iterator.
       */
      class edge
      {
        friend class graph_edge_iterator;

      public:
        edge();
        const edge_type& label() const;
        const vertex_type& source() const;
        const vertex_type& target() const;

      private:
        void set(const edge_type& l, const vertex_type& s,
                 const vertex_type& t);

      private:
        edge_type const* m_label;
        vertex_type const* m_source;
        vertex_type const* m_target;
      }; // class edge

    public:
      typedef const edge value_type;
      typedef const edge& reference;
      typedef const edge* const pointer;
      typedef ptrdiff_t difference_type;

      typedef std::bidirectional_iterator_tag iterator_category;

    public:
      graph_edge_iterator();

      graph_edge_iterator& operator++();
      graph_edge_iterator operator++(int);
      graph_edge_iterator& operator--();
      graph_edge_iterator operator--(int);
      reference operator*() const;
      pointer operator->() const;
      bool operator==(const graph_edge_iterator& it) const;
      bool operator!=(const graph_edge_iterator& it) const;

    private:
      explicit graph_edge_iterator(
          typename graph_content::const_iterator it_begin,
          typename graph_content::const_iterator it_end,
          typename graph_content::const_iterator it_s,
          typename neighbours_list::const_iterator it_d);

    private:
      /** \brief Iterator of the first node. */
      typename graph_content::const_iterator m_vertex_begin;

      /** \brief Iterator of the last node. */
      typename graph_content::const_iterator m_vertex_end;

      /** \brief Iterator on the list of vertex. */
      typename graph_content::const_iterator m_vertex_iterator;

      /** \brief Iterator on the list of edges. */
      typename neighbours_list::const_iterator m_neighbours_iterator;

      /** \brief Current edge. */
      edge m_edge;
    }; // class graph_edge_iterator

  public:
    typedef graph_vertex_iterator vertex_iterator;
    typedef graph_edge_iterator edge_iterator;
    typedef std::reverse_iterator<vertex_iterator> reverse_vertex_iterator;
    typedef std::reverse_iterator<edge_iterator> reverse_edge_iterator;

  public:
    graph();

    void add_edge(const vertex_type& s1, const vertex_type& s2,
                  const edge_type& e = edge_type());
    void add_vertex(const vertex_type& s);

    bool edge_exists(const vertex_type& s, const vertex_type& r) const;
    void neighbours(const vertex_type& s, std::vector<vertex_type>& v) const;
    void vertices(std::vector<vertex_type>& v) const;

    vertex_iterator vertex_begin() const;
    vertex_iterator vertex_end() const;
    vertex_iterator vertex_begin(const vertex_type& s) const;

    reverse_vertex_iterator vertex_rbegin() const;
    reverse_vertex_iterator vertex_rend() const;
    reverse_vertex_iterator vertex_rbegin(const vertex_type& s) const;

    edge_iterator edge_begin() const;
    edge_iterator edge_end() const;
    edge_iterator edge_begin(const vertex_type& s1,
                             const vertex_type& s2) const;

    reverse_edge_iterator edge_rbegin() const;
    reverse_edge_iterator edge_rend() const;
    reverse_edge_iterator edge_rbegin(const vertex_type& s1,
                                      const vertex_type& s2) const;

    const edge_type& label(const vertex_type& s, const vertex_type& r) const;

    std::size_t outer_degree(const vertex_type& s) const;
    std::size_t inner_degree(const vertex_type& s) const;
    std::size_t vertices_count() const;
    std::size_t edges_count() const;

  private:
    /** \brief The content of the graph (edges and vertices. */
    graph_content m_edges;

    /** \brief Inner degree of the vertices. */
    std::map<vertex_type, std::size_t, vertex_compare> m_inner_degrees;

    /** \brief Number of edges. */
    std::size_t m_edges_count;

  }; // class graph

}

#include <claw/graph.tpp>

#endif // __CLAW_GRAPH_HPP__
