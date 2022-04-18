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
 * \file rectangle.tpp
 * \brief Implementation of claw::math::rectangle class.
 * \author Julien Jorge
 */
#include <algorithm>

/**
 * \brief Constructor.
 */
template <class T>
claw::math::rectangle<T>::rectangle()
{}

/**
 * \brief Copy constructor.
 * \param that Rectangle to copy from.
 */
template <class T>
template <class U>
claw::math::rectangle<T>::rectangle(const rectangle<U>& that)
  : position(that.position)
  , width(that.width)
  , height(that.height)
{}

/**
 * \brief Constructor from a box.
 * \param that The box to copy from.
 */
template <class T>
template <class U>
claw::math::rectangle<T>::rectangle(const box_2d<U>& that)
  : position(that.left(), that.top())
  , width(that.width())
  , height(that.height())
{}

/**
 * \brief Constructor with initialization.
 * \param _x Rectangle's X-coordinate.
 * \param _y Rectangle's Y-coordinate.
 * \param _width Rectangle's width.
 * \param _height Rectangle's height.
 */
template <class T>
claw::math::rectangle<T>::rectangle(const value_type& _x, const value_type& _y,
                                    const value_type& _width,
                                    const value_type& _height)
  : position(_x, _y)
  , width(_width)
  , height(_height)
{}

/**
 * \brief Constructor with initialization.
 * \param pos The position of the rectangle.
 * \param _width Rectangle's width.
 * \param _height Rectangle's height.
 */
template <class T>
template <typename U>
claw::math::rectangle<T>::rectangle(const coordinate_2d<U>& pos,
                                    const value_type& _width,
                                    const value_type& _height)
  : position(pos)
  , width(_width)
  , height(_height)
{}

/**
 * \brief Constructor with initialization.
 * \param pos The position of the rectangle.
 * \param size The size of the rectangle.
 */
template <class T>
template <typename U>
claw::math::rectangle<T>::rectangle(const coordinate_2d<U>& pos,
                                    const coordinate_2d<U>& size)
  : position(pos)
  , width(size.x)
  , height(size.y)
{}

/**
 * \brief Get a copy of the rectangle by converting its members to a given
 * type.
 *
 * Consider the following code:
 *
 * <tt> rectangle<float> a;
 *
 * ...
 *
 * rectangle<int> b(a); </tt>
 *
 * The copy constructor will be called, and your compiler should print some
 * warnings in your console. These warnings have a meaning, so we don't want to
 * make them disapear by adding explicit type conversion inside the rectangle
 * class nor adding a cast operator that will be used silently by the compiler.
 *
 * If you really want to convert the type, this method will explicitly cast the
 * member variables.
 */
template <class T>
template <typename U>
claw::math::rectangle<U> claw::math::rectangle<T>::cast_value_type_to() const
{
  return claw::math::rectangle<U>(position.template cast_value_type_to<U>(),
                                  (U)width, (U)height);
}

/**
 * \brief Tell if this rectangle equals an other rectangle.
 * \param that The rectangle to compare to.
 */
template <class T>
bool claw::math::rectangle<T>::operator==(const self_type& that) const
{
  return (position == that.position) && (width == that.width)
         && (height == that.height);
}

/**
 * \brief Tell if this rectangle equals an other rectangle.
 * \param that The rectangle to compare to.
 */
template <class T>
bool claw::math::rectangle<T>::operator!=(const self_type& that) const
{
  return !(*this == that);
}

/**
 * \brief Calculate the rectangle's area.
 */
template <class T>
typename claw::math::rectangle<T>::value_type
claw::math::rectangle<T>::area() const
{
  return width * height;
}

/**
 * \brief Tell if a point is in a rectangle.
 * \param p The supposed included point.
 */
template <class T>
bool claw::math::rectangle<T>::includes(
    const coordinate_2d<value_type>& p) const
{
  return (position.x <= p.x) && (right() >= p.x) && (position.y <= p.y)
         && (bottom() >= p.y);
}

/**
 * \brief Tell if a rectangle is in a rectangle.
 * \param r The supposed included rectangle.
 */
template <class T>
bool claw::math::rectangle<T>::includes(const self_type& r) const
{
  box_2d<value_type> his_box(r);

  return includes(his_box.first_point) && includes(his_box.second_point);
}

/**
 * \brief Tell if there is an intersection of two rectangles.
 * \param r The supposed intersecting rectangle.
 */
template <class T>
bool claw::math::rectangle<T>::intersects(const self_type& r) const
{
  return (right() >= r.position.x) && (r.right() >= position.x)
         && (bottom() >= r.position.y) && (r.bottom() >= position.y);
}

/**
 * \brief Intersection of two rectangles.
 * \param r The supposed intersecting rectangle.
 */
template <class T>
claw::math::rectangle<T>
claw::math::rectangle<T>::intersection(const self_type& r) const
{
  self_type result;

  if(intersects(r))
    {
      x_intersection(r, result);
      y_intersection(r, result);
    }

  return result;
}

/**
 * \brief Get the smallest rectangle bounding both this rectangle and another
 *        one.
 * \param r The other rectangle.
 */
template <class T>
claw::math::rectangle<T>
claw::math::rectangle<T>::join(const self_type& r) const
{
  const T result_left = std::min(left(), r.left());
  const T result_top = std::min(top(), r.top());
  const T result_bottom = std::max(bottom(), r.bottom());
  const T result_right = std::max(right(), r.right());

  return self_type(result_left, result_top, result_right - result_left,
                   result_bottom - result_top);
}

/**
 * \brief set new position and size to the rectangle.
 * \param new_x New x-coordinate.
 * \param new_y New y-coordinate.
 * \param new_width New width.
 * \param new_height New height.
 */
template <class T>
void claw::math::rectangle<T>::set(const value_type& new_x,
                                   const value_type& new_y,
                                   const value_type& new_width,
                                   const value_type& new_height)
{
  position.x = new_x;
  position.y = new_y;
  width = new_width;
  height = new_height;
}

/**
 * \brief Get the x-coordinate of the left edge.
 */
template <class T>
typename claw::math::rectangle<T>::value_type
claw::math::rectangle<T>::left() const
{
  return position.x;
}

/**
 * \brief Get the x-coordinate of the right edge.
 */
template <class T>
typename claw::math::rectangle<T>::value_type
claw::math::rectangle<T>::right() const
{
  return position.x + width;
}

/**
 * \brief Get the y-coordinate of the bottom edge.
 */
template <class T>
typename claw::math::rectangle<T>::value_type
claw::math::rectangle<T>::bottom() const
{
  return position.y + height;
}

/**
 * \brief Get the y-coordinate of the top edge.
 */
template <class T>
typename claw::math::rectangle<T>::value_type
claw::math::rectangle<T>::top() const
{
  return position.y;
}

/**
 * \brief Get the size of the rectangle.
 */
template <class T>
claw::math::coordinate_2d<typename claw::math::rectangle<T>::value_type>
claw::math::rectangle<T>::size() const
{
  return claw::math::coordinate_2d<value_type>(width, height);
}

/**
 * \brief X-intersection of two rectangles.
 * \pre There is an intersection between this and r.
 * \post result's x and width fields are filled.
 */
template <class T>
void claw::math::rectangle<T>::x_intersection(const self_type& r,
                                              self_type& result) const
{
  if(position.x <= r.position.x)
    {
      result.position.x = r.position.x;

      if(right() >= r.right())
        result.width = r.width;
      else
        result.width = right() - r.position.x;
    }
  else
    r.x_intersection(*this, result);
}

/**
 * \brief Y-intersection of two rectangles.
 * \pre There is an intersection between this and r.
 * \post result's y and height fields are filled.
 */
template <class T>
void claw::math::rectangle<T>::y_intersection(const self_type& r,
                                              self_type& result) const
{
  if(position.y <= r.position.y)
    {
      result.position.y = r.position.y;

      if(bottom() >= r.bottom())
        result.height = r.height;
      else
        result.height = bottom() - r.position.y;
    }
  else
    r.y_intersection(*this, result);
}
