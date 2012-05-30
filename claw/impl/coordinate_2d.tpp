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
 * \file coordinate_2d.tpp
 * \brief Implementation of claw::math::coordinate_2d class.
 * \author Julien Jorge
 */
#include <cmath>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<typename T>
claw::math::coordinate_2d<T>::coordinate_2d()
{

} // coordinate_2d::coordinate_2d() [constructor]

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 */
template<typename T>
template<typename U>
claw::math::coordinate_2d<T>::coordinate_2d(const coordinate_2d<U>& that)
  : x(that.x), y(that.y)
{

} // coordinate_2d::coordinate_2d() [copy constructor]

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor with initialization.
 * \param _x x value.
 * \param _y y Value.
 */
template<typename T>
claw::math::coordinate_2d<T>::coordinate_2d
(const value_type& _x, const value_type& _y)
  : x(_x), y(_y)
{

} // coordinate_2d::coordinate_2d() [constructor whit values]

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a copy of the rectangle by converting its members to a given type.
 *
 * Consider the following code:
 *
 * <tt> coordinate_2d<float> a;
 *
 * ...
 *
 * coordinate_2d<int> b(a); </tt>
 *
 * The copy constructor will be called, and your compiler should print some
 * warnings in your console. These warnings have a meaning, so we don't wan't to
 * make them disapear by adding explicit type conversion inside the
 * coordinate_2d class nor adding a cast operator that will be used silently by
 * the compiler.
 *
 * If you really want to convert the type, this method will explicitly cast the
 * member variables.
 */
template<class T>
template<typename U>
claw::math::coordinate_2d<U>
claw::math::coordinate_2d<T>::cast_value_type_to() const
{
  return claw::math::coordinate_2d<U>( (U)x, (U)y );
} // coordinate_2d::cast_value_type_to()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets new values to the coordinate.
 * \param _x New x value.
 * \param _y New y Value.
 */
template<typename T>
void
claw::math::coordinate_2d<T>::set(const value_type& _x, const value_type& _y)
{
  x = _x;
  y = _y; 
} // coordinate_2d::set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the distance separing two coordinates.
 * \param p The second coordinate
 */
template<typename T>
typename claw::math::coordinate_2d<T>::value_type
claw::math::coordinate_2d<T>::distance(const self_type& p) const
{
  return (value_type)sqrt( (p.x - x)*(p.x - x) + (p.y - y)*(p.y - y) );
} // coordinate_2d::distance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Rotate this point around an other point.
 * \param center The other point.
 * \param angle The angle of the rotation.
 */
template<typename T>
void
claw::math::coordinate_2d<T>::rotate( const self_type& center, double angle )
{
  self_type result(center);

  result.x +=
    (x - center.x) * std::cos(angle) - (y - center.y) * std::sin(angle);
  result.y +=
    (x - center.x) * std::sin(angle) + (y - center.y) * std::cos(angle);

  *this = result;
} // coordinate_2d::rotate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the angle of the slope starting from this and ending with an other
 *        coordinate.
 * \param to The other point.
 */
template<typename T>
double claw::math::coordinate_2d<T>::slope_angle( const self_type& to ) const
{
  return std::atan2( to.y - y, to.x - x );
} // coordinate_2d::slope_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality operator.
 * \param that Coordinate to compare to.
 */
template<typename T>
bool claw::math::coordinate_2d<T>::operator==(const self_type& that) const
{
  return (x == that.x) && (y == that.y);
} // coordinate_2d::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Difference operator.
 * \param that Coordinate to compare to.
 */
template<typename T>
bool claw::math::coordinate_2d<T>::operator!=(const self_type& that) const
{
  return !(*this == that);
} // coordinate_2d::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Addition.
 * \param that Coordinate to add.
 */
template<typename T>
claw::math::coordinate_2d<T>
claw::math::coordinate_2d<T>::operator+(const self_type& that) const
{
  return self_type( x + that.x, y + that.y );
} // coordinate_2d::operator+()

/*----------------------------------------------------------------------------*/
/**
 * \brief Subtraction.
 * \param that Coordinate to subtract.
 */
template<typename T>
claw::math::coordinate_2d<T>
claw::math::coordinate_2d<T>::operator-(const self_type& that) const
{
  return self_type( x - that.x, y - that.y );
} // coordinate_2d::operator-()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a coordinate.
 * \param that Coordinate to add.
 */
template<typename T>
claw::math::coordinate_2d<T>&
claw::math::coordinate_2d<T>::operator+=(const self_type& that)
{
  x += that.x;
  y += that.y;

  return *this;
} // coordinate_2d::operator+=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Subtract a coordinate.
 * \param that Coordinate to subtract.
 */
template<typename T>
claw::math::coordinate_2d<T>&
claw::math::coordinate_2d<T>::operator-=(const self_type& that)
{
  x -= that.x;
  y -= that.y;

  return *this;
} // coordinate_2d::operator-=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Multiplication.
 * \param v Factor.
 */
template<typename T>
claw::math::coordinate_2d<T>
claw::math::coordinate_2d<T>::operator*(const value_type& v) const
{
  return self_type( x * v, y * v );
} // coordinate_2d::operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Division.
 * \param v Divider.
 */
template<typename T>
claw::math::coordinate_2d<T>
claw::math::coordinate_2d<T>::operator/(const value_type& v) const
{
  return self_type( x / v, y / v );
} // coordinate_2d::operator/()

/*----------------------------------------------------------------------------*/
/**
 * \brief Multiply the coordinates.
 * \param v Factor.
 */
template<typename T>
claw::math::coordinate_2d<T>&
claw::math::coordinate_2d<T>::operator*=(const value_type& v)
{
  x *= v;
  y *= v;

  return *this;
} // coordinate_2d::operator*=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Divide the coordinates.
 * \param v Divider.
 */
template<typename T>
claw::math::coordinate_2d<T>&
claw::math::coordinate_2d<T>::operator/=(const value_type& v)
{
  x /= v;
  y /= v;

  return *this;
} // coordinate_2d::operator/=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Unary minus.
 * \param that The operand...
 */
template<typename T>
claw::math::coordinate_2d<T>
claw::math::operator-( const claw::math::coordinate_2d<T>& that )
{
  return claw::math::coordinate_2d<T>(-that.x, -that.y);
} // operator-() [coordinate_2d]

/*----------------------------------------------------------------------------*/
/**
 * \brief Multiply coordinates.
 * \param v The multiplicator.
 * \param self The coordinates to multiply.
 */
template<typename T, typename U>
claw::math::coordinate_2d<T>
claw::math::operator*( U v, const coordinate_2d<T>& self )
{
  return self * typename coordinate_2d<T>::value_type(v);
} // operator*() [coordinate_2d]
