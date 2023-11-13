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
 * \file image.cpp
 * \brief Implementation of the claw::graphic::image class.
 * \author Julien Jorge
 */
#include <claw/graphic/image.hpp>

#include <claw/exception.hpp>
#include <claw/graphic/bitmap.hpp>
#include <claw/graphic/gif.hpp>
#include <claw/graphic/pcx.hpp>
#include <claw/graphic/targa.hpp>
#include <claw/graphic/xbm.hpp>

/* The png.h file must be included before any other file that includes setjmp.h
   (as jpeg.hpp). */
#include <claw/graphic/png.hpp>

#include <claw/graphic/jpeg.hpp>

#include <algorithm>

/**
 * \brief Get an iterator on the first pixel.
 */
claw::graphic::image::scanline::iterator
claw::graphic::image::scanline::begin()
{
  return super::begin();
}

/**
 * \brief Get en iterator past the last pixel.
 */
claw::graphic::image::scanline::iterator claw::graphic::image::scanline::end()
{
  return super::end();
}

/**
 * \brief Get an iterator on constant data on the first pixel.
 */
claw::graphic::image::scanline::const_iterator
claw::graphic::image::scanline::begin() const
{
  return super::begin();
}

/**
 * \brief Get an iterator on constant data past the last pixel.
 */
claw::graphic::image::scanline::const_iterator
claw::graphic::image::scanline::end() const
{
  return super::end();
}

/**
 * \brief Get the length of the line.
 */
claw::graphic::image::scanline::size_type
claw::graphic::image::scanline::size() const
{
  return super::size();
}

/**
 * \brief Constructor. Creates an image without datas.
 * \post width() == height() == 0
 */
claw::graphic::image::image()
{}

/**
 * \brief Constructor. Reads an image from an input stream.
 * \param f The stream to read from.
 */
claw::graphic::image::image(std::istream& f)
{
  load(f);
}

/**
 * \brief Constructor. Creates an empty image.
 * \param w Image's width.
 * \param h Image's height.
 * \pre w > 0 and h > 0
 */
claw::graphic::image::image(unsigned int w, unsigned int h)
{
  set_size(w, h);
}

/**
 * \brief Swap the content of two images.
 * \param that The image to swap with.
 */
void claw::graphic::image::swap(image& that)
{
  std::swap(m_data, that.m_data);
}

/**
 * \brief Gets image's width.
 */
unsigned int claw::graphic::image::width() const
{
  if(m_data.empty())
    return 0;
  else
    return m_data[0].size();
}

/**
 * \brief Gets image's height.
 */
unsigned int claw::graphic::image::height() const
{
  return m_data.size();
}

/**
 * \brief Get an iterator pointing on the first pixel.
 */
claw::graphic::image::iterator claw::graphic::image::begin()
{
  return iterator(*this);
}

/**
 * \brief Get an iterator pointing just past the last pixel.
 */
claw::graphic::image::iterator claw::graphic::image::end()
{
  return iterator(*this, width(), height() - 1);
}

/**
 * \brief Get an iterator pointing on the first pixel.
 */
claw::graphic::image::const_iterator claw::graphic::image::begin() const
{
  return const_iterator(*this);
}

/**
 * \brief Get an iterator pointing just past the last pixel.
 */
claw::graphic::image::const_iterator claw::graphic::image::end() const
{
  return const_iterator(*this, width(), height() - 1);
}

/**
 * \brief Merge an image on the current image.
 * \param that The image to merge.
 */
void claw::graphic::image::merge(const image& that)
{
  merge(that, math::coordinate_2d<int>(0, 0));
}

/**
 * \brief Merge an image on the current image.
 * \param that The image to merge.
 * \param pos The position of the top left corner.
 */
void claw::graphic::image::merge(const image& that,
                                 const math::coordinate_2d<int>& pos)
{
  math::rectangle<int> my_box(0, 0, width(), height());
  math::rectangle<int> his_box(pos.x, pos.y, that.width(), that.height());

  if(my_box.intersects(his_box))
    {
      math::rectangle<int> intersection;
      unsigned int that_y = pos.y < 0 ? -pos.y : 0;
      unsigned int that_x = pos.x < 0 ? -pos.x : 0;
      const double max_comp(
          std::numeric_limits<rgba_pixel::component_type>::max());

      intersection = my_box.intersection(his_box);

      for(int y = 0; y != intersection.height; ++y)
        {
          scanline::const_iterator first = that[y + that_y].begin() + that_x;
          scanline::const_iterator last = first + intersection.width;
          scanline::iterator dest
              = (*this)[y + intersection.position.y].begin()
                + intersection.position.x;

          for(; first != last; ++first, ++dest)
            {
              const double src_alpha(first->components.alpha);
              const double dest_alpha(dest->components.alpha
                                      * (max_comp - src_alpha));

              const double red = (double)first->components.red * src_alpha
                                 + (double)dest->components.red * dest_alpha;
              const double green
                  = (double)first->components.green * src_alpha
                    + (double)dest->components.green * dest_alpha;
              const double blue = (double)first->components.blue * src_alpha
                                  + (double)dest->components.blue * dest_alpha;
              const double alpha = src_alpha + dest_alpha;

              dest->components.red = std::min(red, max_comp);
              dest->components.green = std::min(green, max_comp);
              dest->components.blue = std::min(blue, max_comp);
              dest->components.alpha = std::min(alpha, max_comp);
            }
        }
    }
}

/**
 * \brief Copy an image on the current image.
 * \param that The image to copy.
 * \param pos The position of the top left corner.
 */
void claw::graphic::image::partial_copy(const image& that,
                                        const math::coordinate_2d<int>& pos)
{
  math::rectangle<int> my_box(0, 0, width(), height());
  math::rectangle<int> his_box(pos.x, pos.y, that.width(), that.height());

  if(my_box.intersects(his_box))
    {
      math::rectangle<int> intersection;
      unsigned int that_y = pos.y < 0 ? -pos.y : 0;
      unsigned int that_x = pos.x < 0 ? -pos.x : 0;

      intersection = my_box.intersection(his_box);

      for(int y = 0; y != intersection.height; ++y)
        {
          scanline::const_iterator first = that[y + that_y].begin() + that_x;
          scanline::const_iterator last = first + intersection.width;
          scanline::iterator dest
              = (*this)[y + intersection.position.y].begin()
                + intersection.position.x;

          std::copy(first, last, dest);
        }
    }
}

/**
 * \brief Set the image upside down.
 */
void claw::graphic::image::flip()
{
  for(unsigned int y = 0; y != height() / 2; ++y)
    std::swap(m_data[y], m_data[height() - y - 1]);
}

/**
 * \brief Fill an area of the image with a given color.
 * \param r The area to fill.
 * \param c The color to fill with.
 */
void claw::graphic::image::fill(const math::rectangle<int> r,
                                const pixel_type& c)
{
  math::rectangle<int> my_box(0, 0, width(), height());

  if(my_box.intersects(r))
    {
      const math::rectangle<int> intersection(my_box.intersection(r));
      const double max_comp(
          std::numeric_limits<rgba_pixel::component_type>::max());

      for(int y = 0; y != intersection.height; ++y)
        {
          scanline::iterator first
              = (*this)[intersection.position.y + y].begin()
                + intersection.position.x;
          const scanline::iterator last = first + intersection.width;

          for(; first != last; ++first)
            {
              const double src_alpha(c.components.alpha);

              double red = (double)first->components.red
                           + src_alpha * (double)c.components.red / max_comp;
              double green
                  = (double)first->components.green
                    + src_alpha * (double)c.components.green / max_comp;
              double blue = (double)first->components.blue
                            + src_alpha * (double)c.components.blue / max_comp;
              double alpha = (double)first->components.alpha
                             + (max_comp - src_alpha) / max_comp;

              first->components.red = std::min(red, max_comp);
              first->components.green = std::min(green, max_comp);
              first->components.blue = std::min(blue, max_comp);
              first->components.alpha = std::min(alpha, max_comp);
            }
        }
    }
}

/**
 * \brief Set a new size to the image.
 * \remark Image's data won't be lost. If a dimension is set larger than its
 *         current value, extra pixels won't be initialized.
 * \pre (w!=0) && (h!=0)
 */
void claw::graphic::image::set_size(unsigned int w, unsigned int h)
{
  if(w == 0)
    m_data.clear();
  else
    {
      m_data.resize(h);

      for(unsigned int y = 0; y != height(); ++y)
        m_data[y].resize(w);
    }
}

/**
 * \brief Read the image from a stream.
 * \param f The stream to read from.
 */
void claw::graphic::image::load(std::istream& f)
{
  bool ok = false;

  if(!ok)
    try
      {
        jpeg::reader(*this, f);
        ok = true;
      }
    catch(...)
      {}

  if(!ok)
    try
      {
        png::reader(*this, f);
        ok = true;
      }
    catch(...)
      {}

  if(!ok)
    try
      {
        bitmap::reader(*this, f);
        ok = true;
      }
    catch(...)
      {}

  if(!ok)
    try
      {
        targa::reader(*this, f);
        ok = true;
      }
    catch(...)
      {}

  if(!ok)
    try
      {
        gif::reader(*this, f);
        ok = true;
      }
    catch(...)
      {}

  if(!ok)
    try
      {
        pcx::reader(*this, f);
        ok = true;
      }
    catch(...)
      {}

  if(!ok)
    try
      {
        xbm::reader(*this, f);
        ok = true;
      }
    catch(...)
      {}

  if(!ok)
    throw claw::bad_format("image::load: file format isn't supported.");
}

/**
 * \brief Swap the content of two images.
 * \param a The image to swap with \a b.
 * \param b The image to swap with \a a.
 */
void std::swap(claw::graphic::image& a, claw::graphic::image& b)
{
  a.swap(b);
}
