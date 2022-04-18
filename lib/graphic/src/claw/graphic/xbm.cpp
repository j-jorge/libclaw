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
 * \file xbm.cpp
 * \brief Implementation of the claw::graphic::xbm class.
 * \author Julien Jorge
 */
#include <claw/graphic/xbm.hpp>

/**
 * \brief Constructor. Creates an empty image.
 * \param w Image's width.
 * \param h Image's height.
 * \pre w > 0 and h > 0
 */
claw::graphic::xbm::xbm(unsigned int w, unsigned int h)
  : image(w, h)
  , m_hot(NULL)
{}

/**
 * \brief Copy constructor.
 * \param that Image to copy from.
 */
claw::graphic::xbm::xbm(const image& that)
  : image(that)
  , m_hot(NULL)
{}

/**
 * \brief Constructor. Load an image from a xbm file.
 * \param f Xbm file.
 */
claw::graphic::xbm::xbm(std::istream& f)
  : m_hot(NULL)
{
  reader(*this, f);
}

/**
 * \brief Destructor.
 */
claw::graphic::xbm::~xbm()
{
  if(m_hot != NULL)
    delete m_hot;
}

/**
 * \brief Save the image.
 * \param f The file in which we write.
 */
void claw::graphic::xbm::save(std::ostream& f) const
{
  writer(*this, f, writer::options(m_name, m_hot));
}

/**
 * \brief Set the name of the image.
 * \param name The name of the image;
 */
void claw::graphic::xbm::set_name(const std::string& name)
{
  m_name = name;
}

/**
 * \brief Set the hot spot of the image.
 * \param hot The hot spot of the image;
 */
void claw::graphic::xbm::set_hot(const claw::math::coordinate_2d<int>& hot)
{
  if(m_hot != NULL)
    delete m_hot;

  m_hot = new claw::math::coordinate_2d<int>(hot);
}
