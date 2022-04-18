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
 * \file image.hpp
 * \brief A class to deal with images.
 * \author Julien Jorge
 */
#ifndef __CLAW_IMAGE_HPP__
#define __CLAW_IMAGE_HPP__

#include <claw/graphic/pixel.hpp>

#include <claw/math.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <vector>

namespace claw
{
  namespace graphic
  {
    /**
     * \brief A class to deal with images.
     * \author Julien Jorge
     */
    class image
    {
    public:
      /** \brief The type representing the colors of the pixels in the image.
       */
      typedef rgba_pixel pixel_type;

      /**
       * \brief One line in the image.
       * \author Julien Jorge
       */
      class scanline : private std::vector<pixel_type>
      {
        friend class image;

      public:
        /** \brief The type of the parent class. */
        typedef std::vector<pixel_type> super;

        /** \brief The type of the pixels. */
        typedef super::value_type value_type;

        /** \brief Reference to a pixel.. */
        typedef super::reference reference;

        /** \brief Const reference to a pixel. */
        typedef super::const_reference const_reference;

        /** \brief Iterator in the line. */
        typedef super::iterator iterator;

        /** \brief Const iterator in the line. */
        typedef super::const_iterator const_iterator;

        /** \brief An unsigned integral type. */
        typedef super::size_type size_type;

      public:
        iterator begin();
        iterator end();

        const_iterator begin() const;
        const_iterator end() const;

        inline reference operator[](unsigned int i);
        inline const_reference operator[](unsigned int i) const;

        size_type size() const;

      }; // class scanline

    public:
      /**
       * \brief Base class for iterators on an image.
       * \author Julien Jorge.
       */
      template <typename Image, typename Pixel>
      class base_iterator
        : public std::iterator<std::random_access_iterator_tag, Pixel>
      {
      private:
        /** \brief The type of the image we are iterating through. */
        typedef Image image_type;

        /** \brief The type of the pointed pixels. */
        typedef Pixel pixel_type;

        /** \brief The type of the current class. */
        typedef base_iterator<image_type, pixel_type> self_type;

      public:
        /** \brief The type of the values accessed by the iterator. */
        typedef pixel_type value_type;

        /** \brief The type of the references to the values accesssed by the
            iterator. */
        typedef pixel_type& reference;

        /** \brief The type of the pointers to the values accesssed by the
            iterator. */
        typedef pixel_type* pointer;

        /** \brief The type of the distance between two iterators. */
        typedef ptrdiff_t difference_type;

        /** \brief The type of this category. */
        typedef std::random_access_iterator_tag iterator_category;

      public:
        inline base_iterator();
        inline base_iterator(image_type& owner, unsigned int x = 0,
                             unsigned int y = 0);

        inline bool operator==(const self_type& that) const;
        inline bool operator!=(const self_type& that) const;
        inline bool operator<(const self_type& that) const;
        inline bool operator>(const self_type& that) const;
        inline bool operator<=(const self_type& that) const;
        inline bool operator>=(const self_type& that) const;

        inline self_type& operator+=(int n);
        inline self_type& operator-=(int n);

        inline self_type operator+(int n) const;
        inline self_type operator-(int n) const;

        /**
         * \brief Get an iterator at a specific distance of the current
         *        iterator.
         * \param n The distance of the wanted iterator.
         * \param self The reference iterator.
         */
        template <typename ImageT, typename PixelT>
        friend inline self_type operator+(int n, const self_type& self);

        inline difference_type operator-(const self_type& that) const;

        inline self_type& operator++();
        inline self_type operator++(int);
        inline self_type& operator--();
        inline self_type operator--(int);

        inline reference operator*() const;
        inline pointer operator->() const;

        inline reference operator[](int n) const;

      private:
        bool is_final() const;

      private:
        /** \brief The image we are iterating through. */
        image_type* m_owner;

        /** \brief Coordinates of the pointed pixel in m_owner. */
        math::coordinate_2d<unsigned int> m_pos;

      }; // class base_iterator

    public:
      /**
       * \brief The type of the iterator on the pixels of the image.
       *
       * The pixels are accessed from the top line to the bottom line, and from
       * the left to the right.
       */
      typedef base_iterator<image, pixel_type> iterator;

      /**
       * \brief The type of the iterator to access constant pixels.
       *
       * The pixels are accessed from the top line to the bottom line, and from
       * the left to the right.
       */
      typedef base_iterator<const image, const pixel_type> const_iterator;

    public:
      image();
      image(unsigned int w, unsigned int h);
      image(std::istream& f);

      void swap(image& that);

      unsigned int width() const;
      unsigned int height() const;

      inline scanline& operator[](unsigned int i);
      inline const scanline& operator[](unsigned int i) const;

      iterator begin();
      iterator end();
      const_iterator begin() const;
      const_iterator end() const;

      void merge(const image& that);
      void merge(const image& that, const math::coordinate_2d<int>& pos);

      void partial_copy(const image& that,
                        const math::coordinate_2d<int>& pos);

      void flip();
      void fill(const math::rectangle<int> r, const pixel_type& c);

      void set_size(unsigned int w, unsigned int h);

      void load(std::istream& f);

    private:
      /** \brief Data of the picture. */
      std::vector<scanline> m_data;

    }; // class image

  }
}

namespace std
{
  void swap(claw::graphic::image& a, claw::graphic::image& b);
}

// Inline methods
#include <claw/graphic/image.ipp>

#endif // __CLAW_IMAGE_HPP__
