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
 * \file box_2d.tpp
 * \brief Implementation of claw::math::box_2d class.
 * \author Julien Jorge
 */
#include <claw/rectangle.hpp>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<class T>
claw::math::box_2d<T>::box_2d()
{

} // box_2d::box_2d() [constructor]

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that Box to copy from.
 */
template<class T>
claw::math::box_2d<T>::box_2d( const self_type& that )
  : first_point(that.first_point), second_point(that.second_point)
{

} // box_2d::box_2d() [copy constructor]

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor from a rectangle.
 * \param that Rectangle to copy from.
 */
template<class T>
claw::math::box_2d<T>::box_2d( const rectangle<value_type>& that )
  : first_point(that.position),
    second_point(that.right(), that.bottom())
{

} // box_2d::box_2d() [constructor from rectangle]

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor from two points.
 * \param p1 The first point.
 * \param p2 The second point.
 */
template<class T>
claw::math::box_2d<T>::box_2d( const point_type& p1, const point_type& p2 )
  : first_point(p1), second_point(p2)
{

} // box_2d::box_2d() [constructor from coordinates]

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor with initialization.
 * \param x1 X-coordinate of the first point.
 * \param y1 Y-coordinate of the first point.
 * \param x2 X-coordinate of the second point.
 * \param y2 Y-coordinate of the second point.
 */
template<class T>
claw::math::box_2d<T>::box_2d( const value_type& x1, const value_type& y1,
                               const value_type& x2, const value_type& y2 )
  : first_point(x1, y1), second_point(x2, y2)
{

} // box_2d::box_2d() [constructor with values]

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the coordinates of the two points.
 * \param x1 X-coordinate of the first point.
 * \param y1 Y-coordinate of the first point.
 * \param x2 X-coordinate of the second point.
 * \param y2 Y-coordinate of the second point.
 */
template<class T>
void claw::math::box_2d<T>::set
( const value_type& x1, const value_type& y1, const value_type& x2,
  const value_type& y2 )
{
  first_point.set(x1, y1);
  second_point.set(x2, y2);
} // box_2d::set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a copy of the box by converting its members to a given type.
 *
 * Consider the following code:
 *
 * <tt> box_2d<float> a;
 *
 * ...
 *
 * box_2d<int> b(a); </tt>
 *
 * The copy constructor will be called, and your compiler should print some
 * warnings in your console. These warnings have a meaning, so we don't wan't to
 * make them disapear by adding explicit type conversion inside the box_2d
 * class nor adding a cast operator that will be used silently by the compiler.
 *
 * If you really want to convert the type, this method will explicitly cast the
 * member variables.
 */
template<class T>
template<typename U>
claw::math::box_2d<U> claw::math::box_2d<T>::cast_value_type_to() const
{
  return claw::math::box_2d<U>
    ( first_point.cast_value_type_to<U>(),
      second_point.cast_value_type_to<U>() );
} // box_2d::cast_value_type_to()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the box's area.
 */
template<class T>
typename claw::math::box_2d<T>::value_type claw::math::box_2d<T>::area() const
{
  return width() * height();
} // box_2d::area()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a point is in a box.
 * \param p The supposed included point.
 */
template<class T>
bool
claw::math::box_2d<T>::includes( const coordinate_2d<value_type>& p ) const
{
  return (left() <= p.x) && (right() >= p.x)
    && (bottom() <= p.y) && (top() >= p.y);
} // box_2d::includes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a box_2d is in a box_2d.
 * \param r The supposed included box_2d.
 */
template<class T>
bool claw::math::box_2d<T>::includes( const self_type& r ) const
{
  return includes(r.first_point) && includes(r.second_point);
} // box_2d::includes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is an intersection of two boxes.
 * \param r The supposed intersecting box.
 */
template<class T>
bool claw::math::box_2d<T>::intersects( const self_type& r ) const
{
  return (right() >= r.left()) && (r.right() >= left()) 
    && (top() >= r.bottom()) && (r.top() >= bottom());
} // box_2d::intersects()

/*----------------------------------------------------------------------------*/
/**
 * \brief Intersection of two box_2ds.
 * \param r The supposed intersecting box_2d.
 */
template<class T>
claw::math::box_2d<T>
claw::math::box_2d<T>::intersection( const self_type& r ) const
{
  CLAW_PRECOND( intersects(r) );

  self_type result;

  if ( intersects(r) )
    {
      x_intersection(r, result);
      y_intersection(r, result);
    }

  return result;
} // box_2d::intersection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Join two box_2ds.
 * \param r The box to join with.
 * \returns a box containing \a *this and \a r.
 */
template<class T>
claw::math::box_2d<T> claw::math::box_2d<T>::join( const self_type& r ) const
{
  return self_type
    ( std::min(r.left(), left()), std::min(r.bottom(), bottom()),
      std::max(r.right(), right()), std::max(r.top(), top()) );
} // box_2d::join()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the box has a dimension equal to zero.
 */
template<class T>
bool claw::math::box_2d<T>::empty() const
{
  return (width() == 0) || (height() == 0);
} // box_2d::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the y-coordinate of the top edge.
 */
template<class T>
typename claw::math::box_2d<T>::value_type claw::math::box_2d<T>::top() const
{
  return (first_point.y > second_point.y) ? first_point.y : second_point.y;
} // box_2d::top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the y-coordinate of the bottom edge.
 */
template<class T>
typename claw::math::box_2d<T>::value_type claw::math::box_2d<T>::bottom() const
{
  return (first_point.y < second_point.y) ? first_point.y : second_point.y;
} // box_2d::bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the x-coordinate of the left edge.
 */
template<class T>
typename claw::math::box_2d<T>::value_type claw::math::box_2d<T>::left() const
{
  return (first_point.x < second_point.x) ? first_point.x : second_point.x;
} // box_2d::left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the x-coordinate of the right edge.
 */
template<class T>
typename claw::math::box_2d<T>::value_type claw::math::box_2d<T>::right() const
{
  return (first_point.x > second_point.x) ? first_point.x : second_point.x;
} // box_2d::right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the coordinate of the top-left corner.
 */
template<class T>
typename claw::math::box_2d<T>::point_type
claw::math::box_2d<T>::top_left() const
{
  return point_type(left(), top());
} // box_2d::top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the coordinate of the top-right corner.
 */
template<class T>
typename claw::math::box_2d<T>::point_type
claw::math::box_2d<T>::top_right() const
{
  return point_type(right(), top());
} // box_2d::top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the coordinate of the bottom-left corner.
 */
template<class T>
typename claw::math::box_2d<T>::point_type
claw::math::box_2d<T>::bottom_left() const
{
  return point_type(left(), bottom());
} // box_2d::bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the coordinate of the bottom-right corner.
 */
template<class T>
typename claw::math::box_2d<T>::point_type
claw::math::box_2d<T>::bottom_right() const
{
  return point_type(right(), bottom());
} // box_2d::bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the top edge at a given position.
 * \param p The position.
 */
template<class T>
void claw::math::box_2d<T>::top( const value_type& p )
{
  shift_y(p - top());
} // box_2d::top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the bottom edge at a given position.
 * \param p The position.
 */
template<class T>
void claw::math::box_2d<T>::bottom( const value_type& p )
{
  shift_y(p - bottom());
} // box_2d::bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the left edge at a given position.
 * \param p The position.
 */
template<class T>
void claw::math::box_2d<T>::left( const value_type& p )
{
  shift_x(p - left());
} // box_2d::left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the right edge at a given position.
 * \param p The position.
 */
template<class T>
void claw::math::box_2d<T>::right( const value_type& p )
{
  shift_x(p - right());
} // box_2d::right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the top-left corner at a given position.
 * \param p The position.
 */
template<class T>
void
claw::math::box_2d<T>::top_left( const coordinate_2d<value_type>& p )
{
  top(p.y);
  left(p.x);
} // box_2d::top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the top-right corner at a given position.
 * \param p The position.
 */
template<class T>
void
claw::math::box_2d<T>::top_right( const coordinate_2d<value_type>& p )
{
  top(p.y);
  right(p.x);
} // box_2d::top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the bottom-left corner at a given position.
 * \param p The position.
 */
template<class T>
void
claw::math::box_2d<T>::bottom_left( const coordinate_2d<value_type>& p )
{
  bottom(p.y);
  left(p.x);
} // box_2d::bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the bottom-right corner at a given position.
 * \param p The position.
 */
template<class T>
void claw::math::box_2d<T>::bottom_right
( const coordinate_2d<value_type>& p )
{
  bottom(p.y);
  right(p.x);
} // box_2d::bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Shift the position of the box on the x-axis.
 * \param d The movement length.
 */
template<class T>
void claw::math::box_2d<T>::shift_x( const value_type& d )
{
  first_point.x += d;
  second_point.x += d;
} // box_2d::shift_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Shift the position of the box on the y-axis.
 * \param d The movement length.
 */
template<class T>
void claw::math::box_2d<T>::shift_y( const value_type& d )
{
  first_point.y += d;
  second_point.y += d;
} // box_2d::shift_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the box_2d.
 */
template<class T>
claw::math::coordinate_2d< typename claw::math::box_2d<T>::value_type >
claw::math::box_2d<T>::size() const
{
  return claw::math::coordinate_2d<value_type>(width(), height());
} // box_2d::size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality operator
 * \param that Box to compare to.
 */
template<class T>
bool claw::math::box_2d<T>::operator==(const self_type& that) const
{
  return (left() == that.left()) && (right() == that.right())
    && (top() == that.top()) && (bottom() == that.bottom());
} // box_2d::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Difference operator.
 * \param that Box to compare to.
 */
template<class T>
bool claw::math::box_2d<T>::operator!=(const self_type& that) const
{
  return !( *this == that );
} // box_2d::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Translation.
 * \param vect The vector to add to points.
 */
template<class T>
claw::math::box_2d<T>
claw::math::box_2d<T>::operator+(const point_type& vect) const
{
  return self_type( first_point + vect, second_point + vect );
} // box_2d::operator+()

/*----------------------------------------------------------------------------*/
/**
 * \brief Translation.
 * \param vect The vector to substract to points.
 */
template<class T>
claw::math::box_2d<T>
claw::math::box_2d<T>::operator-(const point_type& vect) const
{
  return self_type( first_point - vect, second_point - vect );
} // box_2d::operator-()

/*----------------------------------------------------------------------------*/
/**
 * \brief Translation.
 * \param vect The vector to add to points.
 */
template<class T>
claw::math::box_2d<T>&
claw::math::box_2d<T>::operator+=(const point_type& vect)
{
  first_point += vect;
  second_point += vect;
} // box_2d::operator+=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Translation.
 * \param vect The vector to substract to points.
 */
template<class T>
claw::math::box_2d<T>&
claw::math::box_2d<T>::operator-=(const point_type& vect)
{
  first_point -= vect;
  second_point -= vect;
} // box_2d::operator-=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return box' width.
 */
template<class T>
typename claw::math::box_2d<T>::value_type
claw::math::box_2d<T>::width() const
{
  if (first_point.x > second_point.x)
    return first_point.x - second_point.x;
  else
    return second_point.x - first_point.x;
} // box_2d::width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return box' height.
 */
template<class T>
typename claw::math::box_2d<T>::value_type
claw::math::box_2d<T>::height() const
{
  if (first_point.y > second_point.y)
    return first_point.y - second_point.y;
  else
    return second_point.y - first_point.y;
} // box_2d::height()

/*----------------------------------------------------------------------------*/
/**
 * \brief X-intersection of two box_2ds.
 * \pre There is an intersection between this and r.
 * \post result's x and width fields are filled.
 */
template<class T>
void claw::math::box_2d<T>::x_intersection
( const self_type& r, self_type& result ) const
{
  result.first_point.x = std::max(left(), r.left());
  result.second_point.x = std::min(right(), r.right());
} // box_2d::x_intersection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Y-intersection of two box_2ds.
 * \pre There is an intersection between this and r.
 * \post result's y and height fields are filled.
 */
template<class T>
void claw::math::box_2d<T>::y_intersection
( const self_type& r, self_type& result ) const
{
  result.first_point.y = std::max(bottom(), r.bottom());
  result.second_point.y = std::min(top(), r.top());
} // box_2d::y_intersection()
