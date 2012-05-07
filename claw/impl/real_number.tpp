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
 * \file claw/impl/real_number.tpp
 * \brief Implementation of the claw::real_number class.
 * \author Julien Jorge
 */
#include <cmath>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructuor.
 */
template<typename T>
claw::real_number<T>::real_number()
  : m_value(0), m_epsilon( make_epsilon<value_type>::value(m_value) )
{

} // real_number::real_number()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param v The value of the number.
 */
template<typename T>
claw::real_number<T>::real_number( const value_type& v )
  : m_value(v), m_epsilon( make_epsilon<T>::value(m_value) )
{

} // real_number::real_number()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
template<typename T>
claw::real_number<T>::real_number( const self_type& that )
  : m_value(that.m_value), m_epsilon(that.m_epsilon)
{

} // real_number::real_number()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the absolute value of the number.
 */
template<typename T>
typename claw::real_number<T>::self_type claw::real_number<T>::abs() const
{
  return self_type( std::abs(m_value) );
} // real_number::abs()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this number is stricty lower than an other number.
 * \param that The other number.
 */
template<typename T>
bool claw::real_number<T>::operator<( const self_type& that ) const
{
  if ( that.m_value == std::numeric_limits<value_type>::infinity() )
    return m_value != std::numeric_limits<value_type>::infinity();
  else if ( that.m_value == -std::numeric_limits<value_type>::infinity() )
    return false;
  else if ( m_value == std::numeric_limits<value_type>::infinity() )
    return false;
  else if ( m_value == -std::numeric_limits<value_type>::infinity() )
    return that.m_value != -std::numeric_limits<value_type>::infinity();
  else
    return m_value < (that.m_value - std::max(m_epsilon, that.m_epsilon));
} // real_number::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this number is lower or equal to an other number.
 * \param that The other number.
 */
template<typename T>
bool claw::real_number<T>::operator<=( const self_type& that ) const
{
  return !(that < *this);
} // real_number::operator<=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this number is stricty greater than an other number.
 * \param that The other number.
 */
template<typename T>
bool claw::real_number<T>::operator>( const self_type& that ) const
{
  return that < *this;
} // real_number::operator>()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this number is greater or equal to an other number.
 * \param that The other number.
 */
template<typename T>
bool claw::real_number<T>::operator>=( const self_type& that ) const
{
  return that <= *this;
} // real_number::operator>=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this number is equal to an other number.
 * \param that The other number.
 */
template<typename T>
bool claw::real_number<T>::operator==( const self_type& that ) const
{
  if ( that.m_value == std::numeric_limits<value_type>::infinity() )
    return m_value == std::numeric_limits<value_type>::infinity();
  else if ( that.m_value == -std::numeric_limits<value_type>::infinity() )
    return m_value == -std::numeric_limits<value_type>::infinity();
  else if ( m_value == that.m_value )
    return true;
  else
    return std::abs(m_value - that.m_value)
      <= std::max(m_epsilon, that.m_epsilon);
} // real_number::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this number is not equal to an other number.
 * \param that The other number.
 */
template<typename T>
bool claw::real_number<T>::operator!=( const self_type& that ) const
{
  return !((*this) == that);
} // real_number::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sum two numbers.
 * \param that The other number.
 */
template<typename T>
typename claw::real_number<T>::self_type
claw::real_number<T>::operator+( const self_type& that ) const
{
  return self_type(m_value + that.m_value);
} // real_number::operator+()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the difference of two numbers.
 * \param that The other number.
 */
template<typename T>
typename claw::real_number<T>::self_type
claw::real_number<T>::operator-( const self_type& that ) const
{
  return self_type(m_value - that.m_value);
} // real_number::operator-()

/*----------------------------------------------------------------------------*/
/**
 * \brief Multiply two numbers.
 * \param that The other number.
 */
template<typename T>
typename claw::real_number<T>::self_type
claw::real_number<T>::operator*( const self_type& that ) const
{
  return self_type(m_value * that.m_value);
} // real_number::operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Divide by an other number.
 * \param that The other number.
 */
template<typename T>
typename claw::real_number<T>::self_type
claw::real_number<T>::operator/( const self_type& that ) const
{
  return self_type(m_value / that.m_value);
} // real_number::operator/()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an other number.
 * \param that The other number.
 */
template<typename T>
typename claw::real_number<T>::self_type&
claw::real_number<T>::operator+=( const self_type& that )
{
  m_value += that.m_value;
  m_epsilon = make_epsilon<value_type>::value(m_value);
  return *this;
} // real_number::operator+=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Subtract an other number.
 * \param that The other number.
 */
template<typename T>
typename claw::real_number<T>::self_type&
claw::real_number<T>::operator-=( const self_type& that )
{
  m_value -= that.m_value;
  m_epsilon = make_epsilon<value_type>::value(m_value);
  return *this;
} // real_number::operator-=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Multiply by an other number.
 * \param that The other number.
 */
template<typename T>
typename claw::real_number<T>::self_type&
claw::real_number<T>::operator*=( const self_type& that )
{
  m_value *= that.m_value;
  m_epsilon = make_epsilon<value_type>::value(m_value);
  return *this;
} // real_number::operator*=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Divide by an other number.
 * \param that The other number.
 */
template<typename T>
typename claw::real_number<T>::self_type&
claw::real_number<T>::operator/=( const self_type& that )
{
  m_value /= that.m_value;
  m_epsilon = make_epsilon<value_type>::value(m_value);
  return *this;
} // real_number::operator/=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output the value in a stream.
 * \param os The stream in which the value is written.
 */
template<typename T>
std::ostream& claw::real_number<T>::output( std::ostream& os ) const
{
  return os << m_value;
} // real_number::output()

/*----------------------------------------------------------------------------*/
/**
 * \brief Cast the value.
 */
template<typename T>
template<typename U>
claw::real_number<T>::operator U() const
{
  return (U)m_value;
} // real_number::operator U()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the absolute value of a number.
 * \param v The number.
 */
template<typename T>
claw::real_number<T> std::abs( const claw::real_number<T>& v )
{
  return v.abs();
} // abs()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the opposite value of a number.
 * \param self The number.
 */
template<typename T>
claw::real_number<T> operator-( const claw::real_number<T>& self )
{
  return claw::real_number<T>(0) - self;
} // operator-()

/*----------------------------------------------------------------------------*/
/**
 * \brief Subtract a number to an other value.
 * \param v The other value.
 * \param self The number.
 */
template<typename T>
claw::real_number<T> operator-( T v, const claw::real_number<T>& self )
{
  return claw::real_number<T>(v) - self;
} // operator-()

/*----------------------------------------------------------------------------*/
/**
 * \brief Output a number in a stream.
 * \param os The stream in which the value is written.
 * \param self The number.
 */
template<typename T>
std::ostream& operator<<( std::ostream& os, const claw::real_number<T>& self )
{
  return self.output(os);
} // operator<<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a number from a stream.
 * \param is The stream from which the value is read.
 * \param self The number.
 */
template<typename T>
std::istream& operator>>( std::istream& is, claw::real_number<T>& self )
{
  return is >> self.m_value;
} // operator>>()
