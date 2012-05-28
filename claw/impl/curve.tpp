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
 * \file claw/impl/curve.tpp
 * \brief Implementation of claw::math::curve.
 * \author Julien Jorge
 */
#include <boost/math/special_functions/cbrt.hpp>
#include <boost/math/constants/constants.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<typename C, typename Traits>
claw::math::curve<C, Traits>::control_point::control_point()
{

} // curve::control_point::control_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The position of this control point. It will be assigned to the input
 *        and output directions too.
 */
template<typename C, typename Traits>
claw::math::curve<C, Traits>::control_point::control_point
( const coordinate_type& p )
  : m_position(p), m_input_direction(p), m_output_direction(p)
{

} // curve::control_point::control_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The position of this control point.
 * \param input_direction The point in the direction of which the curve enters
 *        \a p.
 * \param output_direction The point in the direction of which the curve leaves
 *        \a p.
 */
template<typename C, typename Traits>
claw::math::curve<C, Traits>::control_point::control_point
( const coordinate_type& p, const coordinate_type& input_direction,
  const coordinate_type& output_direction )
  : m_position(p), m_input_direction(input_direction),
    m_output_direction(output_direction)
{

} // curve::control_point::control_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of this control point.
 */
template<typename C, typename Traits>
const typename claw::math::curve<C, Traits>::control_point::coordinate_type&
claw::math::curve<C, Traits>::control_point::get_position() const
{
  return m_position;
} // curve::control_point::get_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the point in the direction of which the curve enters this
 *        position.
 */
template<typename C, typename Traits>
const typename claw::math::curve<C, Traits>::control_point::coordinate_type&
claw::math::curve<C, Traits>::control_point::get_input_direction() const
{
  return m_input_direction;
} // curve::control_point::get_input_direction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the point in the direction of which the curve leaves this
 *        position.
 */
template<typename C, typename Traits>
const typename claw::math::curve<C, Traits>::control_point::coordinate_type&
claw::math::curve<C, Traits>::control_point::get_output_direction() const
{
  return m_output_direction;
} // curve::control_point::get_output_direction()





/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param position The position of the point.
 * \param s The section on which the point has been found.
 * \param t The date of the point on the section.
 */
template<typename C, typename Traits>
claw::math::curve<C, Traits>::section::resolved_point::resolved_point
( const coordinate_type& position, const section& s, const double t )
  : m_position(position), m_section(s), m_date(t)
{

} // curve::section::resolved_point::resolved_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get The position of the point.
 */
template<typename C, typename Traits>
const
typename claw::math::curve<C, Traits>::section::resolved_point::coordinate_type&
claw::math::curve<C, Traits>::section::resolved_point::get_position() const
{
  return m_position;
} // curve::section::resolved_point::get_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the section on which the point has been found.
 */
template<typename C, typename Traits>
const typename claw::math::curve<C, Traits>::section&
claw::math::curve<C, Traits>::section::resolved_point::get_section() const
{
  return m_section;
} // curve::section::::resolved_point::get_section()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the date of the point on the section.
 */
template<typename C, typename Traits>
double
claw::math::curve<C, Traits>::section::resolved_point::get_date() const
{
  return m_date;
} // curve::section::resolved_point::get_date()






/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param origin The point at the beginning of the section.
 * \param end The point at the end of the section.
 */
template<typename C, typename Traits>
claw::math::curve<C, Traits>::section::section
( const iterator_type& origin, const iterator_type& end )
  : m_origin(origin), m_end(end)
{

} // curve::section::section()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the point of this section at a given date.
 * \param t The date at which the point is computed.
 */
template<typename C, typename Traits>
typename claw::math::curve<C, Traits>::section::coordinate_type
claw::math::curve<C, Traits>::section::get_point_at( double t ) const
{
  if ( m_origin == m_end )
    return m_origin->get_position();

  const value_type x
    ( evaluate
      ( t, traits_type::get_x(m_origin->get_position()),
        traits_type::get_x(m_origin->get_output_direction()),
        traits_type::get_x(m_end->get_input_direction()),
        traits_type::get_x(m_end->get_position()) ) );
  const value_type y
    ( evaluate
      ( t, traits_type::get_y(m_origin->get_position()),
        traits_type::get_y(m_origin->get_output_direction()),
        traits_type::get_y(m_end->get_input_direction()),
        traits_type::get_y(m_end->get_position()) ) );

  return traits_type::make_coordinate( x, y );
} // curve::section::get_point_at()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the direction of the tangent at a given date.
 * \param t The date for which we want the tangent.
 */
template<typename C, typename Traits>
typename claw::math::curve<C, Traits>::section::coordinate_type
claw::math::curve<C, Traits>::section::get_tangent_at( double t ) const
{
  const value_type dx = evaluate_derived
    ( t, traits_type::get_x(m_origin->get_position()),
      traits_type::get_x(m_origin->get_output_direction()),
      traits_type::get_x(m_end->get_input_direction()),
      traits_type::get_x(m_end->get_position()) );

  const value_type dy = evaluate_derived
    ( t, traits_type::get_y(m_origin->get_position()),
      traits_type::get_y(m_origin->get_output_direction()),
      traits_type::get_y(m_end->get_input_direction()),
      traits_type::get_y(m_end->get_position()) );

  if ( dx == 0 )
    return traits_type::make_coordinate( 0, (dy < 0) ? -1 : 1 );
  else
    return traits_type::make_coordinate( 1, dy / dx );
} // curve::section::get_tangent_at()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the points having the given x-coordinate on this section.
 * \param x The coordinate for which we want the points.
 */
template<typename C, typename Traits>
std::vector<typename claw::math::curve<C, Traits>::section::resolved_point>
claw::math::curve<C, Traits>::section::get_point_at_x( value_type x ) const
{
  std::vector<resolved_point> result;

  if ( empty() )
    return result;
  
  if ( x == m_origin->get_position().x )
    result.push_back
      ( resolved_point( m_origin->get_position(), *this, 0 ) );
  else if ( x == m_end->get_position().x )
    result.push_back
      ( resolved_point( m_end->get_position(), *this, 1 ) );
  else
    {        
      const std::vector<double> roots
        ( get_roots
          ( x, traits_type::get_x(m_origin->get_position()),
            traits_type::get_x(m_origin->get_output_direction()),
            traits_type::get_x(m_end->get_input_direction()),
            traits_type::get_x(m_end->get_position() ) ) );
  
      for ( std::size_t i=0; i!=roots.size(); ++i )
        result.push_back
          ( resolved_point( get_point_at( roots[i] ), *this, roots[i] ) );
    }

  return result;
} // curve::section::get_point_at_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the control point at the origin of the section in
 *        the curve from which it was created.
 */
template<typename C, typename Traits>
const typename claw::math::curve<C, Traits>::section::iterator_type&
claw::math::curve<C, Traits>::section::get_origin() const
{
  return m_origin;
} // curve::section::get_origin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is no points on this section.
 */
template<typename C, typename Traits>
bool claw::math::curve<C, Traits>::section::empty() const
{
  return m_origin == m_end;
} // curve::section::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the length of this section.
 */
template<typename C, typename Traits>
typename claw::math::curve<C, Traits>::section::value_type
claw::math::curve<C, Traits>::section::get_length() const
{
  return get_length(1);
} // curve::section::get_length()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the length of the part of this section between the origin and the
 *        point at the given date.
 * \param t The date up to which the length is measured.
 */
template<typename C, typename Traits>
typename claw::math::curve<C, Traits>::section::value_type
claw::math::curve<C, Traits>::section::get_length( double t ) const
{
  if ( empty() )
    return 0;

  const value_type dx_0 = evaluate_derived
    ( 0, traits_type::get_x(m_origin->get_position()),
      traits_type::get_x(m_origin->get_output_direction()),
      traits_type::get_x(m_end->get_input_direction()),
      traits_type::get_x(m_end->get_position()) );

  const value_type dy_0 = evaluate_derived
    ( 0, traits_type::get_y(m_origin->get_position()),
      traits_type::get_y(m_origin->get_output_direction()),
      traits_type::get_y(m_end->get_input_direction()),
      traits_type::get_y(m_end->get_position()) );

  const value_type dx_t = evaluate_derived
    ( t, traits_type::get_x(m_origin->get_position()),
      traits_type::get_x(m_origin->get_output_direction()),
      traits_type::get_x(m_end->get_input_direction()),
      traits_type::get_x(m_end->get_position()) );

  const value_type dy_t = evaluate_derived
    ( t, traits_type::get_y(m_origin->get_position()),
      traits_type::get_y(m_origin->get_output_direction()),
      traits_type::get_y(m_end->get_input_direction()),
      traits_type::get_y(m_end->get_position()) );

  const value_type d_0( dx_0 * dx_0 + dy_0 * dy_0 );
  const value_type d_t( dx_t * dx_t + dy_t * dy_t );

  return ( d_t * std::sqrt(d_t) - d_0 * std::sqrt(d_0) ) * 2 / 3;
} // curve::section::get_length()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of the curve's equation on one dimension, at a given
 *        date.
 * \param t The date at which the value us computed.
 * \param origin The value on the computed dimension of the first point of the
 *        section of the curve.
 * \param output_direction The value on the computed dimension of the point in
 *        the direction of which the curve leaves \a origin.
 * \param input_direction The value on the computed dimension of the point in
 *        the direction of which the curve enters \a end.
 * \param origin The value on the computed dimension of the last point of the
 *        section of the curve.
 */
template<typename C, typename Traits>
typename claw::math::curve<C, Traits>::section::value_type
claw::math::curve<C, Traits>::section::evaluate
( double t, value_type origin, value_type output_direction,
  value_type input_direction, value_type end ) const
{
  const double dt(1 - t);
 
  return origin * dt * dt * dt
    + 3 * output_direction * t * dt * dt
    + 3 * input_direction * t * t * dt
    + end * t * t * t;
} // curve::section::evaluate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value at a given date of the curve's derived equation on one
 *        dimension.
 * \param t The date at which the value us computed.
 * \param origin The value on the computed dimension of the first point of the
 *        section of the curve.
 * \param output_direction The value on the computed dimension of the point in
 *        the direction of which the curve leaves \a origin.
 * \param input_direction The value on the computed dimension of the point in
 *        the direction of which the curve enters \a end.
 * \param origin The value on the computed dimension of the last point of the
 *        section of the curve.
 */
template<typename C, typename Traits>
typename claw::math::curve<C, Traits>::section::value_type
claw::math::curve<C, Traits>::section::evaluate_derived
( double t, value_type origin, value_type output_direction,
  value_type input_direction, value_type end ) const
{
  return - 3 * origin + 3 * output_direction
    + (6 * origin - 12 * output_direction + 6 * input_direction) * t
    + (-3 * origin + 9 * output_direction - 9 * input_direction + 3 * end)
    * t * t;
} // curve::section::evaluate_derived()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the dates at which the curve passes at a given coordinate, on a
          given dimension.
 * \param x The coordinate for which we want the dates.
 * \param origin The value on the computed dimension of the first point of the
 *        section of the curve.
 * \param output_direction The value on the computed dimension of the point in
 *        the direction of which the curve leaves \a origin.
 * \param input_direction The value on the computed dimension of the point in
 *        the direction of which the curve enters \a end.
 * \param origin The value on the computed dimension of the last point of the
 *        section of the curve.
 */
template<typename C, typename Traits>
std::vector<double>
claw::math::curve<C, Traits>::section::get_roots
( value_type x, value_type origin, value_type output_direction,
  value_type input_direction, value_type end ) const
{
  const value_type a
    (-origin + 3 * output_direction - 3 * input_direction + end );
  const value_type b( 3 * origin - 6 * output_direction + 3 * input_direction );
  const value_type c( -3 * origin + 3 * output_direction );
  const value_type d( origin - x );

  if ( a == 0 )
    return get_roots_degree_2(b, c, d);
  else
    return get_roots_degree_3(a, b, c, d);
} // curve::section::get_roots()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the dates at which the curve passes at a given coordinate, in the
 *        case where the equation is a reduced to a polynom of degree 2.
 * \param a The coefficient of the square part of the equation.
 * \param b The coefficient of the linear part of the equation.
 * \param c The constant of the equation.
 */
template<typename C, typename Traits>
std::vector<double>
claw::math::curve<C, Traits>::section::get_roots_degree_2
( value_type a, value_type b, value_type c ) const
{
  const value_type delta( b * b - 4 * a * c );

  std::vector<double> result;

  if ( delta == 0 )
    result.push_back( - b / ( 2 * a ) );
  else if ( delta > 0 )
    {
      result.push_back( (-b - std::sqrt(delta)) / (2 * a) );
      result.push_back( (-b + std::sqrt(delta)) / (2 * a) );
    }

  return result;
} // curve::section::get_roots_degree_2()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the dates at which the curve passes at a given coordinate, in the
 *        case where the equation is a reduced to a polynom of degree 3.
 * \param a The coefficient of the cubic part of the equation.
 * \param b The coefficient of the square part of the equation.
 * \param c The coefficient of the linear part of the equation.
 * \param d The constant of the equation.
 */
template<typename C, typename Traits>
std::vector<double>
claw::math::curve<C, Traits>::section::get_roots_degree_3
( value_type a, value_type b, value_type c, value_type d ) const
{
  // The following is the application of the method of Cardan

  const value_type p( -(b * b) / (3.0 * a * a) + c / a );
  const value_type q
    ( ( b / (27.0 * a) )
      * ( (2.0 * b * b) / (a * a)
          - 9.0 * c / a )
      + d / a );

  const value_type delta( q * q + 4.0 * p * p * p / 27.0 );

  std::vector<double> result;

  if ( delta == 0 )
    {
      if ( p == 0 )
        result.push_back(0);
      else
        {
          result.push_back( 3.0 * q / p );
          result.push_back( - 3.0 * q / (2.0 * p) );
        }
    }
  else if ( delta > 0 )
    {
    result.push_back
      ( boost::math::cbrt
        ( (-q + std::sqrt(delta)) / 2.0 )
        + boost::math::cbrt
        ( (-q - std::sqrt(delta)) / 2.0 ) );
    }
  else
    for ( std::size_t i=0; i!=3; ++i )
      result.push_back
        ( 2.0 * std::sqrt( -p / 3.0 )
          * std::cos
          ( std::acos( std::sqrt(27.0 / (- p * p * p)) * - q / 2.0 ) / 3.0
            + 2.0 * i * boost::math::constants::pi<double>() / 3.0 ) );

  return result;
} // curve::section::get_roots_degree_3()






/*----------------------------------------------------------------------------*/
/**
 * \brief Add a point at the end of the curve.
 * \param p The point to add.
 */
template<typename C, typename Traits>
void claw::math::curve<C, Traits>::push_back( const control_point& p )
{
  m_points.push_back(p);
} // curve::push_back()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a point at the beginning of the curve.
 * \param p The point to add.
 */
template<typename C, typename Traits>
void claw::math::curve<C, Traits>::push_front( const control_point& p )
{
  m_points.push_front(p);
} // curve::push_front()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a point before an other point of the curve.
 * \param pos An iterator on the point before which the control point is added.
 * \param p The point to add.
 */
template<typename C, typename Traits>
void claw::math::curve<C, Traits>::insert
( const iterator& pos, const control_point& p )
{
  m_points.insert( pos, p );
} // curve::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the section of the curve starting at a given control point.
 * \param pos An iterator of the control point at which the returned section
 *        begins.
 */
template<typename C, typename Traits>
typename claw::math::curve<C, Traits>::section
claw::math::curve<C, Traits>::get_section( const const_iterator& pos ) const
{
  const_iterator it(pos);
  ++it;

  if ( it == end() )
    return section( pos, pos );
  else
    return section( pos, it );
} // curve::get_section()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the points having the given x-coordinate on the curve.
 * \param x The coordinate for which we want the points.
 */
template<typename C, typename Traits>
std::vector< typename claw::math::curve<C, Traits>::section::resolved_point >
claw::math::curve<C, Traits>::get_point_at_x( value_type x ) const
{
  typedef std::vector<typename section::resolved_point> result_type;
  result_type result;

  for ( const_iterator it=begin(); it!=end(); ++it )
    {
      const section s( get_section(it) );

      if ( !s.empty() )
        {
          const result_type new_points( s.get_point_at_x(x) );

          result.insert( result.end(), new_points.begin(), new_points.end() );
        }
    }

  return result;
} // curve::get_point_at_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the length of the curve up between the first control point and a
 *        given control point.
 * \param pos An iterator on the control point up to which the length is
 *        computed.
 */
template<typename C, typename Traits>
typename claw::math::curve<C, Traits>::value_type
claw::math::curve<C, Traits>::get_length( const const_iterator& pos ) const
{
  value_type result(0);

  for ( const_iterator it=begin(); it!=end(); ++it )
    {
      const section s( get_section(it) );
      result += s.get_length();
    }

  return result;
} // curve::get_length()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first control point.
 */
template<typename C, typename Traits>
typename claw::math::curve<C, Traits>::iterator
claw::math::curve<C, Traits>::begin()
{
  return m_points.begin();
} // curve::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the last control point.
 */
template<typename C, typename Traits>
typename claw::math::curve<C, Traits>::iterator
claw::math::curve<C, Traits>::end()
{
  return m_points.end();
} // curve::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first control point.
 */
template<typename C, typename Traits>
typename claw::math::curve<C, Traits>::const_iterator
claw::math::curve<C, Traits>::begin() const
{
  return m_points.begin();
} // curve::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the last control point.
 */
template<typename C, typename Traits>
typename claw::math::curve<C, Traits>::const_iterator
claw::math::curve<C, Traits>::end() const
{
  return m_points.end();
} // curve::end()