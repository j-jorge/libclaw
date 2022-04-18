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
 * \file color_palette.hpp
 * \brief A palette of color, for palettized images.
 * \author Julien Jorge
 */
#ifndef __CLAW_GRAPHIC_COLOR_PALETTE_HPP__
#define __CLAW_GRAPHIC_COLOR_PALETTE_HPP__

#include <vector>

namespace claw
{
  namespace graphic
  {
    /**
     * \brief A palette of colors, for palettized images.
     * \author Julien Jorge
     */
    template <typename Color>
    class color_palette
    {
    public:
      /** \brief The type of the colors stored in the palette. */
      typedef Color color_type;

    public:
      color_palette(unsigned int n);

      unsigned int size() const;

      color_type& operator[](unsigned int i);
      const color_type& operator[](unsigned int i) const;

      const color_type& operator()(unsigned int i) const;

    private:
      /** \brief The colors in the palette. */
      std::vector<color_type> m_palette;

    }; // class color_palette
  }
}

#include <claw/graphic/color_palette.tpp>

#endif // __CLAW_GRAPHIC_COLOR_PALETTE_HPP__
