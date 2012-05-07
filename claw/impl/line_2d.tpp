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
 * \file line_2d.tpp
 * \brief Implementation of claw::math::line_2d class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<class T>
claw::math::line_2d<T>::line_2d()
{

} // line_2d::line_2d() [constructor]

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param that Line to copy from.
 */
template<class T>
template<class U>
claw::math::line_2d<T>::line_2d( const line_2d<U>& that )
  : origin(that.origin), direction(that.direction)
{

} // line_2d::line_2d() [copy constructor]

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor with initializations.
 * \param _origin A point on the line.
 * \param _direction The direction of the line.
 */
template<class T>
claw::math::line_2d<T>::line_2d
( const point_type& _origin, const direction_type& _direction )
  : origin(_origin), direction(_direction)
{

} // line_2d::line_2d() [constructor with values]

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor with initializations.
 * \param ox X-coordinate of the origin.
 * \param oy Y-coordinate of the origin.
 * \param dx X direction of the line.
 * \param dy Y direction of the line.
 */
template<class T>
claw::math::line_2d<T>::line_2d( const value_type& ox, const value_type& oy,
                                 const value_type& dx, const value_type& dy )
  : origin(ox, oy), direction(dx, dy)
{

} // line_2d::line_2d() [constructor with detailed origin & direction]

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two lines are parallels.
 * \param that The other line.
 */
template<class T>
bool claw::math::line_2d<T>::parallel( const self_type& that ) const
{
  return !( (direction.x * that.direction.y)
            - (that.direction.x * direction.y) );
} // line_2d::parallel()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two lines are orthogonal.
 * \param that The other line.
 */
template<class T>
bool claw::math::line_2d<T>::orthogonal( const self_type& that ) const
{
  return !( direction.dot_product( that.direction ) );
} // line_2d::orthogonal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the point at the intersection of two lines.
 * \param that The other line.
 * \remark The result if unknow if the two lines are parallel.
 */
template<class T>
typename claw::math::line_2d<T>::point_type
claw::math::line_2d<T>::intersection( const self_type& that ) const
{
  point_type result;

  if ( ! parallel( that ) )
    {
      point_type delta( that.origin - origin );
      value_type n, m;
      
      n = direction.x * delta.y - direction.y * delta.x;
      m = that.direction.x * direction.y - direction.x * that.direction.y;

      result.x = that.origin.x + (n * that.direction.x) / m;
      result.y = that.origin.y + (n * that.direction.y) / m;
    }

  return result;
} // line_2d::intersection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the y value of the point of the line at position \a x.
 * \param x The X-coordinate for which we want the Y-coordinate.
 */
template<class T>
typename claw::math::line_2d<T>::value_type
claw::math::line_2d<T>::y_value( const value_type& x ) const
{
  return (direction.y * (x - origin.x) + direction.x * origin.y) / direction.x;
} // line_2d::y_value()
