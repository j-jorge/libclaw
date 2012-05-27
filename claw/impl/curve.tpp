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

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<typename C, typename Traits>
claw::math::curve::control_point::control_point()
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
claw::math::curve<C, Traits>::section::coordinate_type
claw::math::curve<C, Traits>::section::get_point_at( double t ) const
{
  if ( m_origin == m_end )
    return m_origin->get_position();

  const value_type x
    ( evaluate
      ( t, traits::get_x(m_origin->get_position()),
        traits::get_x(m_origin->get_output_direction()),
        traits::get_x(m_end->get_input_direction()),
        traits::get_x(m_end->get_position()) ) );
  const value_type y
    ( evaluate
      ( t, traits::get_y(m_origin->get_position()),
        traits::get_y(m_origin->get_output_direction()),
        traits::get_y(m_end->get_input_direction()),
        traits::get_y(m_end->get_position()) ) );

  return traits::make_coordinate( x, y );
} // curve::section::get_point_at()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the direction of the tangent at a given date.
 * \param t The date for which we want the tangent.
 */
template<typename C, typename Traits>
claw::math::curve<C, Traits>::section::value_type
claw::math::curve<C, Traits>::section::get_tangent_at( double t ) const
{
  return 0;
} // curve::section::get_tangent_at()

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
claw::math::curve<C, Traits>::section::value_type
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
 * \brief Get the y-coordinate of the points of the curve having the given
 *        x-coordinate.
 * \param x The x-coordinate for which we want the points.
 */
template<typename C, typename Traits>
std::vector<claw::math::curve<C, Traits>::coordinate_type>
claw::math::curve<C, Traits>::get_y_at( value_type x ) const
{

} // curve::get_y_at()

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
  return m_points.end()
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
