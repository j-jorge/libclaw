/*
  CLAW - a C++ Library Absolutely Wonderful

  CLAW is a free library without any particular aim but being useful to 
  anyone.

  Copyright (C) 2005-2009 Julien Jorge

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

  contact: julien_jorge@yahoo.fr
*/
/**
 * \file image.ipp
 * \brief Inline methods for the claw::graphic::image class.
 * \author Julien Jorge
 */
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a pixel from the line.
 * \param i The index of the pixel.
 */
claw::graphic::image::scanline::reference
inline claw::graphic::image::scanline::operator[](unsigned int i)
{
  return super::operator[](i);
} // image::scanline::operator[]()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a pixel from the line.
 * \param i The index of the pixel.
 */
claw::graphic::image::scanline::const_reference
inline claw::graphic::image::scanline::operator[](unsigned int i) const
{
  return super::operator[](i);
} // image::scanline::operator[]()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<typename Image, typename Pixel>
inline claw::graphic::image::base_iterator<Image, Pixel>::base_iterator()
  : m_owner(NULL), m_pos(0, 0)
{
  CLAW_POSTCOND(is_final());
} // image::base_iterator::base_iterator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor, from an image.
 * \param owner The image we will iterate through.
 * \param x X-coordinate of the pointed pixel.
 * \param y Y-coordinate of the pointed pixel.
 */
template<typename Image, typename Pixel>
inline claw::graphic::image::base_iterator<Image, Pixel>::base_iterator
( image_type& owner, unsigned int x, unsigned int y )
  : m_owner(&owner), m_pos(x, y)
{

} // image::base_iterator::base_iterator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two iterator point to the same address.
 * \param that The other operand.
 */
template<typename Image, typename Pixel>
inline bool
claw::graphic::image::base_iterator<Image, Pixel>::operator==
( const self_type& that ) const
{
  if ( is_final() && that.is_final() )
    return true;
  else if ( m_owner == that.m_owner )
    return m_pos == that.m_pos;
  else
    return false;
} // image::base_iterator::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two iterator points to different addresses.
 * \param that The other operand.
 */
template<typename Image, typename Pixel>
inline bool
claw::graphic::image::base_iterator<Image, Pixel>::operator!=
( const self_type& that ) const
{
  return !(*this == that);
} // image::base_iterator::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the current iterator is before an other.
 * \param that The other operand.
 */
template<typename Image, typename Pixel>
inline bool
claw::graphic::image::base_iterator<Image, Pixel>::operator<
  ( const self_type& that ) const
{
  if ( this->m_pos.y == that.m_pos.y)
    return this->m_pos.x < that.m_pos.x;
  else
    return this->m_pos.y < that.m_pos.y;
} // image::base_iterator::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the current iterator is after an other.
 * \param that The other operand.
 */
template<typename Image, typename Pixel>
inline bool
claw::graphic::image::base_iterator<Image, Pixel>::operator>
( const self_type& that ) const
{
  return that < *this;
} // image::base_iterator::operator>()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the current iterator is before an other, or on the same
 *        address.
 * \param that The other operand.
 */
template<typename Image, typename Pixel>
inline bool
claw::graphic::image::base_iterator<Image, Pixel>::operator<=
( const self_type& that ) const
{
  return !(*this > that);
} // image::base_iterator::operator<=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the current iterator is after an other, or on the same
 *        address.
 * \param that The other operand.
 */
template<typename Image, typename Pixel>
inline bool
claw::graphic::image::base_iterator<Image, Pixel>::operator>=
( const self_type& that ) const
{
  return !(*this < that);
} // image::base_iterator::operator>=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the iterator.
 * \param n Number of steps of the move.
 */
template<typename Image, typename Pixel>
inline typename claw::graphic::image::base_iterator<Image, Pixel>::self_type&
claw::graphic::image::base_iterator<Image, Pixel>::operator+=( int n )
{
  if (n < 0)
    return *this -= -n;
  else
    {
      CLAW_PRECOND( !is_final() );

      unsigned int n_y = n / m_owner->width();
      unsigned int n_x = n % m_owner->width();

      m_pos.x += n_x;
      m_pos.y += n_y;

      return *this;
    }
} // image::base_iterator::operator+=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the iterator.
 * \param n Number of steps of the move.
 */
template<typename Image, typename Pixel>
inline typename claw::graphic::image::base_iterator<Image, Pixel>::self_type&
claw::graphic::image::base_iterator<Image, Pixel>::operator-=( int n )
{
  if (n < 0)
    return *this += -n;
  else
    {
      CLAW_PRECOND( m_owner );

      unsigned int n_y = n / m_owner->width();
      unsigned int n_x = n % m_owner->width();

      CLAW_PRECOND( m_pos.x >= n_x );
      CLAW_PRECOND( m_pos.y >= n_y );

      m_pos.x -= n_x;
      m_pos.y -= n_y;

      return *this;
    }
} // image::base_iterator::operator-=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator at a specific distance of the current iterator.
 * \param n The distance of the wanted iterator.
 */
template<typename Image, typename Pixel>
inline typename claw::graphic::image::base_iterator<Image, Pixel>::self_type
claw::graphic::image::base_iterator<Image, Pixel>::operator+( int n ) const
{
  self_type that(*this);

  return that += n;
} // image::base_iterator::operator+()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator at a specific distance of the current iterator.
 * \param n The distance of the wanted iterator.
 */
template<typename Image, typename Pixel>
inline typename claw::graphic::image::base_iterator<Image, Pixel>::self_type
claw::graphic::image::base_iterator<Image, Pixel>::operator-( int n ) const
{
  self_type that(*this);

  return that -= n;
} // image::base_iterator::operator-()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator at a specific distance of the current iterator.
 * \param n The distance of the wanted iterator.
 * \param self The reference iterator.
 */
template<typename ImageT, typename PixelT>
inline typename claw::graphic::image::base_iterator<ImageT, PixelT>::self_type
operator+
( int n,
  const typename 
  claw::graphic::image::base_iterator<ImageT, PixelT>::self_type& self )
{
  return self + n;
} // image::base_iterator::operator+()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the distance between two iterators.
 * \param that The other operand.
 */
template<typename Image, typename Pixel>
inline
typename claw::graphic::image::base_iterator<Image, Pixel>::difference_type
claw::graphic::image::base_iterator<Image, Pixel>::operator-
( const self_type& that ) const
{
  CLAW_PRECOND( is_final() || that.is_final() || (m_owner == that.m_owner) );

  if ( that.is_final() )
    {
      if ( is_final() )
        return 0;
      else
        return -(m_owner->height() - m_pos.y) * m_owner->width() - m_pos.x;
    }
  else if ( is_final() )
    return (that.m_owner->height() - that.m_pos.y) * that.m_owner->width()
      + that.m_pos.x;
  else
    return m_pos.y * m_owner->width() + m_pos.x
      - that.m_pos.y * that.m_owner->width() + that.m_pos.x;
} // image::base_iterator::operator-()

/*----------------------------------------------------------------------------*/
/**
 * \brief Preincrement.
 */
template<typename Image, typename Pixel>
inline typename claw::graphic::image::base_iterator<Image, Pixel>::self_type&
claw::graphic::image::base_iterator<Image, Pixel>::operator++()
{
  CLAW_PRECOND( !is_final() );

  ++m_pos.x;

  if ( m_pos.x == m_owner->width() )
    {
      m_pos.x = 0;
      ++m_pos.y;
    }

  return *this;
} // image::base_iterator::operator++()

/*----------------------------------------------------------------------------*/
/**
 * \brief Postincrement.
 */
template<typename Image, typename Pixel>
inline typename claw::graphic::image::base_iterator<Image, Pixel>::self_type
claw::graphic::image::base_iterator<Image, Pixel>::operator++(int)
{
  self_type that(*this);
  ++(*this);
  return that;
} // image::base_iterator::operator++() [postincrement]

/*----------------------------------------------------------------------------*/
/**
 * \brief Predecrement.
 */
template<typename Image, typename Pixel>
inline typename claw::graphic::image::base_iterator<Image, Pixel>::self_type&
claw::graphic::image::base_iterator<Image, Pixel>::operator--()
{
  CLAW_PRECOND( !is_final() );
  CLAW_PRECOND( (m_pos.y > 0) || (m_pos.x > 0) );

  if ( m_pos.x == 0 )
    {
      m_pos.x = m_owner->width() - 1;
      --m_pos.y;
    }
  else
    --m_pos.x;

  return *this;
} // image::base_iterator::operator--()

/*----------------------------------------------------------------------------*/
/**
 * \brief Postdecrement.
 */
template<typename Image, typename Pixel>
inline typename claw::graphic::image::base_iterator<Image, Pixel>::self_type
claw::graphic::image::base_iterator<Image, Pixel>::operator--(int)
{
  self_type that(*this);
  --(*this);
  return that;
} // image::base_iterator::operator--() [postdecrement]

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a reference on the pointed pixel.
 */
template<typename Image, typename Pixel>
inline typename claw::graphic::image::base_iterator<Image, Pixel>::reference
claw::graphic::image::base_iterator<Image, Pixel>::operator*() const
{
  CLAW_PRECOND( !is_final() );

  return (*m_owner)[m_pos.y][m_pos.x];
} // image::base_iterator::operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a pointer on the pointed pixel.
 */
template<typename Image, typename Pixel>
inline typename claw::graphic::image::base_iterator<Image, Pixel>::pointer
claw::graphic::image::base_iterator<Image, Pixel>::operator->() const
{
  CLAW_PRECOND( !is_final() );

  return &(*m_owner)[m_pos.y][m_pos.x];
} // image::base_iterator::operator->()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a pixel, using the iterator like an array.
 * \param n Index of the cell from which we want the pixel.
 */
template<typename Image, typename Pixel>
inline typename claw::graphic::image::base_iterator<Image, Pixel>::reference
claw::graphic::image::base_iterator<Image, Pixel>::operator[]( int n ) const
{
  return *(*this + n);
} // image::base_iterator::operator[]()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the iterator is past the end of its owner.
 */
template<typename Image, typename Pixel>
inline bool
claw::graphic::image::base_iterator<Image, Pixel>::is_final() const
{
  if ( !m_owner )
    return true;
  else if ( m_pos.y >= m_owner->height() )
    return true;
  else if ( m_pos.y == m_owner->height() - 1 )
    return m_pos.x >= m_owner->width();
  else
    return false;
} // image::base_iterator::is_final()




/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a line of the image.
 */
inline claw::graphic::image::scanline&
claw::graphic::image::operator[](unsigned int i)
{
  return m_data[i];
} // image::operator[]()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a line of the image.
 */
inline const claw::graphic::image::scanline&
claw::graphic::image::operator[](unsigned int i) const
{
  return m_data[i];
} // image::operator[]() [const]
