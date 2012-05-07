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

  contact: julien.jorge@gamned.org
*/
/**
 * \file graph_algorithm.hpp
 * \brief Various algorithms for graph manipulation.
 * \author Julien Jorge
 */
#ifndef __CLAW_GRAPH_ALGORITHM_HPP__
#define __CLAW_GRAPH_ALGORITHM_HPP__

#include <map>

namespace claw
{
  //*************************** graph::scan_events ****************************

    /**
     * \brief Different stages of graph scanning.
     */
  template<class Graph>
  class scan_events
  {
  public:
    typedef typename Graph::vertex_type vertex_type;

  public:
    void init( const Graph& g ) {}
    void start_vertex( const vertex_type& v ) {}
    void visit_edge( const vertex_type& v1, const vertex_type& v2 ) {}
    void end_vertex( const vertex_type& v ) {}
  }; // class scan_events







  //************************** breadth_scan ***********************************





    /**
     * \brief This class performs a depth scan of a graph. Only reachables 
     * vertices from a given vertex are proceeded.
     */
  template<class Graph, class Events = scan_events<Graph> >
  class breadth_scan
  {
  public:
    typedef typename Graph::vertex_type vertex_type;
    typedef typename Graph::vertex_iterator vertex_iterator ;
    /**
     * \brief Colors are :
     * - 0 : never seen.
     * - 1 : seen but not done.
     * - 2 : done.  
     */
    typedef std::map<vertex_type, int,
                     typename Graph::vertex_compare> coloration;

  public:
    breadth_scan( const Graph& g, const vertex_type& source,
                  Events& events );

    void operator()();

  private:
    const Graph& m_g;
    const vertex_type& m_source;
    Events& m_events;
  }; // class breadth_scan







  //**************************** depth_scan ***********************************






    /**
     * \brief This class performs a depth scan of a graph. All nodes are
     * proceeded.
     */
  template<class Graph, class Events = typename Graph::scan_events>
  class depth_scan
  {
  public:
    typedef typename Graph::vertex_type vertex_type;
    typedef typename Graph::vertex_iterator vertex_iterator ;
    /**
     * \brief Colors are :
     * - 0 : never seen.
     * - 1 : seen but not done.
     * - 2 : done.  
     */
    typedef std::map<vertex_type, int,
                     typename Graph::vertex_compare> coloration;

  public:
    depth_scan( const Graph& g, Events& events );

    void operator()();

  private:
    void recursive_scan(const vertex_type& s, coloration& seen_vertices);

  private:
    const Graph& m_g;
    Events& m_events;
  }; // class depth_scan




  //********************** topological_sort ***********************************




    /**
     * \brief Pass this class as the "Envents" template parameter of the
     *        depth scan class to sort the vertices of a graph with the 
     *        topological sort algorithm.
     *
     * When a node process ends, the node is added to a vector. The vector is
     * filled from end to begining.
     */
  template<class Graph>
  class topological_sort : public scan_events<Graph>
  {
  public:
    typedef typename scan_events<Graph>::vertex_type vertex_type;
    typedef std::vector<vertex_type> result_type;
    typedef typename result_type::const_iterator const_iterator;
    typedef topological_sort<Graph> self_type;

  public:
    void init( const Graph& g );
    void end_vertex( const vertex_type& s );

    void operator()( const Graph& g );
    const vertex_type& operator[](unsigned int index) const;
        
    const_iterator begin() const;
    const_iterator end() const;

  private:
    /** \brief The vector we're filling. */
    result_type m_result;
    /** \brief Index of the next item to put in the vector. */
    int m_next_index;
  }; // class topological_sort

} // namespace claw

#include <claw/impl/graph_algorithm.tpp>

#endif // __CLAW_GRAPH_ALGORITHM_HPP__
