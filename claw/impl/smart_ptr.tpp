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
 * \file smart_ptr.tpp
 * \brief Implementation of the claw::memory::smart_ptr class.
 * \author Julien Jorge
 */
#include <cassert>
#include <cstdlib>

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
template<typename T>
claw::memory::smart_ptr<T>::smart_ptr()
  : m_ref_count(NULL), m_ptr(NULL)
{

} // smart_ptr::smart_ptr()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor from a pointer.
 * \param data Pointer on the data.
 *
 * \b Warning: this constructor allows expressions like
 *
 * <tt>int a; smart_ptr<int> p(&a);</tt>
 *
 * Nevertheless, you should never fo that.
 */
template<typename T>
claw::memory::smart_ptr<T>::smart_ptr( pointer data )
  : m_ref_count(NULL), m_ptr(NULL)
{
  if (data)
    {
      m_ref_count = new unsigned int(1);
      m_ptr = data;
    }
} // smart_ptr::smart_ptr() [pointer]

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The smart_pointer to copy.
 */
template<typename T>
claw::memory::smart_ptr<T>::smart_ptr( const self_type& that )
{
  copy( that );
} // smart_ptr::smart_ptr() [copy]

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor. The memory is freed only if no more smart_ptr point on it.
 */
template<typename T>
claw::memory::smart_ptr<T>::~smart_ptr()
{
  release();
} // smart_ptr::~smart_ptr()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 * \param that The smart_ptr to copy.
 */
template<typename T>
typename claw::memory::smart_ptr<T>::self_type&
claw::memory::smart_ptr<T>::operator=( const self_type& that )
{
  if ( &that != this )
    {
      release();
      copy( that );
    }

  return *this;
} // smart_ptr::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality operator.
 * \param that The pointer to compare to.
 * \return !(*this < that) && !(that < *this).
 */
template<typename T>
bool claw::memory::smart_ptr<T>::operator==( const self_type& that ) const
{
  return !(*this < that) && !(that < *this);
} // smart_ptr::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disequality operator.
 * \param that The pointer to compare to.
 * \return (*this < that) || (that < *this).
 */
template<typename T>
bool claw::memory::smart_ptr<T>::operator!=( const self_type& that ) const
{
  return (*this < that) || (that < *this);
} // smart_ptr::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief "Less than" operator.
 * \param that The pointer to compare to.
 * \return True if the address pointed by \a this is lower than the address
 *         pointed by \a that.
 */
template<typename T>
bool claw::memory::smart_ptr<T>::operator<( const self_type& that ) const
{
  return m_ptr < that.m_ptr;
} // smart_ptr::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief "Less or equal" operator.
 * \param that The pointer to compare to.
 * \return !(that < *this).
 */
template<typename T>
bool claw::memory::smart_ptr<T>::operator<=( const self_type& that ) const
{
  return !(that < *this);
} // smart_ptr::operator<=()

/*----------------------------------------------------------------------------*/
/**
 * \brief "Greater than" operator.
 * \param that The pointer to compare to.
 * \return \a that < *this.
 */
template<typename T>
bool claw::memory::smart_ptr<T>::operator>( const self_type& that ) const
{
  return that < *this;
} // smart_ptr::operator>()

/*----------------------------------------------------------------------------*/
/**
 * \brief "Greater or equal" operator.
 * \param that The pointer to compare to.
 * \return !(*this < that).
 */
template<typename T>
bool claw::memory::smart_ptr<T>::operator>=( const self_type& that ) const
{
  return !(*this < that);
} // smart_ptr::operator>=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Dereference operator.
 */
template<typename T>
typename claw::memory::smart_ptr<T>::pointer
claw::memory::smart_ptr<T>::operator->()
{
  return m_ptr;
} // smart_ptr::operator->()

/*----------------------------------------------------------------------------*/
/**
 * \brief Dereference operator.
 */
template<typename T>
typename claw::memory::smart_ptr<T>::pointer
claw::memory::smart_ptr<T>::operator->() const
{
  return m_ptr;
} // smart_ptr::operator->()

/*----------------------------------------------------------------------------*/
/**
 * \brief Dereference operator.
 */
template<typename T>
typename claw::memory::smart_ptr<T>::reference
claw::memory::smart_ptr<T>::operator*()
{
  return *m_ptr;
} // smart_ptr::operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Dereference operator.
 */
template<typename T>
typename claw::memory::smart_ptr<T>::reference
claw::memory::smart_ptr<T>::operator*() const
{
  return *m_ptr;
} // smart_ptr::operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy a smart_ptr.
 * \param that The smart_pointer to copy.
 */
template<typename T>
void claw::memory::smart_ptr<T>::copy( const self_type& that )
{
  assert( this != &that );

  m_ref_count = that.m_ref_count;
  m_ptr = that.m_ptr;

  if (m_ref_count)
    ++(*m_ref_count);
} // smart_ptr::copy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Release the allocated memory.
 *
 * The memory is release only if no more smart_ptr point on it.
 */
template<typename T>
void claw::memory::smart_ptr<T>::release()
{
  if (m_ref_count)
    if ( *m_ref_count )
      {
	--(*m_ref_count);

	if ( !(*m_ref_count) )
	  {
	    delete m_ptr;
	    delete m_ref_count;
	      
	    m_ref_count = NULL;
	  }
	  
	m_ptr = NULL;
      }
} // smart_ptr::release()
