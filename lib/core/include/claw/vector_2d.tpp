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
 * \file vector_2d.tpp
 * \brief Implementation of claw::math::vector_2d class.
 * \author Julien Jorge
 */
#include <cmath>

/**
 * \brief Constructor.
 */
template <class T>
claw::math::vector_2d<T>::vector_2d()
{}

/**
 * \brief Copy constructor.
 */
template <class T>
template <class U>
claw::math::vector_2d<T>::vector_2d(const coordinate_2d<U>& that)
  : super(that)
{}

/**
 * \brief Construct a vector from two points.
 */
template <class T>
template <class U>
claw::math::vector_2d<T>::vector_2d(const coordinate_2d<U>& p1,
                                    const coordinate_2d<U>& p2)
  : super(p2.x - p1.x, p2.y - p1.y)
{}

/**
 * \brief Constructor with initialization.
 * \param _x x value.
 * \param _y y Value.
 */
template <class T>
claw::math::vector_2d<T>::vector_2d(const value_type& _x, const value_type& _y)
  : super(_x, _y)
{}

/**
 * \brief Gets vector length.
 */
template <class T>
typename claw::math::vector_2d<T>::value_type
claw::math::vector_2d<T>::length() const
{
  return std::sqrt((this->x * this->x) + (this->y * this->y));
}

/**
 * \brief Normalize the vector.
 */
template <class T>
void claw::math::vector_2d<T>::normalize()
{
  value_type l = length();

  if(l != 0)
    {
      this->x /= l;
      this->y /= l;
    }
}

/**
 * \brief Get a vector orthonormal to this vector.
 */
template <class T>
claw::math::vector_2d<T>
claw::math::vector_2d<T>::get_orthonormal_clockwise() const
{
  return self_type(this->y, -this->x);
}

/**
 * \brief Get a vector orthonormal to this vector.
 */
template <class T>
claw::math::vector_2d<T>
claw::math::vector_2d<T>::get_orthonormal_anticlockwise() const
{
  return self_type(-this->y, this->x);
}

/**
 * \brief Dot product.
 * \param that The other operand.
 */
template <class T>
typename claw::math::vector_2d<T>::value_type
claw::math::vector_2d<T>::dot_product(const self_type& that) const
{
  return this->x * that.x + this->y * that.y;
}
