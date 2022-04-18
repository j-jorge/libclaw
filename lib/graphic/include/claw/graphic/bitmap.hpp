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
 * \file bitmap.hpp
 * \brief A class for bitmap pictures.
 * \author Julien Jorge
 */
#ifndef __CLAW_BITMAP_HPP__
#define __CLAW_BITMAP_HPP__

#include <claw/graphic/color_palette.hpp>
#include <claw/graphic/image.hpp>

#include <claw/buffered_istream.hpp>
#include <claw/rle_decoder.hpp>

#include <iostream>
#include <vector>

namespace claw
{
  namespace graphic
  {
    /**
     * \brief A class for bitmap images.
     * \author Julien Jorge
     */
    class bitmap : public image
    {
    private:
      /**
       * \brief Tool class used for defining the structures of the datas stored
       *        in a bitmap file.
       * \author Julien Jorge
       */
      class file_structure
      {
      public:
        /** \brief The type of the color palette for low color image files. */
        typedef color_palette<rgba_pixel_8> color_palette_type;

        /** \brief Compression mode. */
        enum compression
        {
          BMP_COMPRESSION_RGB = 0,
          BMP_COMPRESSION_RLE8 = 1,
          BMP_COMPRESSION_RLE4 = 2,
          BMP_COMPRESSION_BITFIELDS = 3
        };

#pragma pack(push, 2)

        /**
         * \brief Header of a bitmap file.
         */
        struct header
        {
          /** \brief File identifier (must be 'BM'). */
          char id[2];

          /** \brief File's size. */
          unsigned int file_size;

          /** \brief not used. */
          unsigned int nop;

          /** \brief Begininf of the datas. */
          unsigned int data_offset;

          /** \brief Header's size. */
          unsigned int header_size;

          /** \brief Image's width. */
          unsigned int width;

          /** \brief Image's height. */
          unsigned int height;

          /** \brief Number of layers. */
          unsigned short layers;

          /** \brief Bits per pixel. */
          unsigned short bpp;

          /** \brief Compression algorithm. */
          unsigned int compression;

          /** \brief Image's size (bytes). */
          unsigned int image_size;

          /** \brief Horizontal resolution (pixels per meter). */
          unsigned int ppm_x;

          /** \brief Vertical resolution (pixels per meter). */
          unsigned int ppm_y;

          /** \brief Number of colors. */
          unsigned int colors_count;

          /** \brief Number of important colors. */
          unsigned int importants_colors;
        };
#pragma pack(pop)

      }; // class file_structure

    public:
      /**
       * \brief This class read data from a bitmap file and store it in an
       *        image.
       * \author Julien Jorge
       */
      class reader : private file_structure
      {
      private:
        /** \brief The type of the input buffer associated with the file when
            decoding RLE files. */
        typedef buffered_istream<std::istream> file_input_buffer;

        /**
         * \brief The output buffer for the RLE decoder.
         *
         * \b Template \b parameters
         * - Coded4bits, true is the RLE patterns are coded in four bits ;
         *               otherwise the patterns are supposed to be on eight
         *               bits.
         *
         * \author Julien Jorge
         */
        template <bool Coded4bits>
        class rle_bitmap_output_buffer
        {
        public:
          rle_bitmap_output_buffer(const color_palette_type& palette,
                                   image& image);

          void fill(unsigned int n, unsigned char pattern);
          void copy(unsigned int n, file_input_buffer& buffer);

          void next_line();
          void delta_move(unsigned char x, unsigned char y);

        private:
          /** \brief Color palette. */
          const color_palette_type& m_palette;

          /** \brief The image to fill. */
          image& m_image;

          /** \brief Current column index in the bitmap. */
          unsigned int m_x;

          /** \brief Current row index in the bitmap. */
          unsigned int m_y;

        }; // class rle_bitmap_output_buffer

        /**
         * \brief RLE decoder for bitmap RLE format
         *
         * \b Template \b parameters :
         * - \a OutputBuffer The type of the output buffer.
         *
         * The \a OutputBuffer type must match the type requirements of the
         * template parameter OutputBuffer of the rle_decoder class, plus two
         * methods :
         * - next_line(), set the output position at the begining of the next
         *                line.
         * - delta_move( unsigned char x, unsigned char y ), move the output
         *                position \a x pixels right and \a y pixels down.
         *
         * \author Julien Jorge
         */
        template <typename OutputBuffer>
        class rle_bitmap_decoder
          : public rle_decoder<char, file_input_buffer, OutputBuffer>
        {
        public:
          /** \brief Type of the output buffer. */
          typedef OutputBuffer output_buffer_type;

        private:
          virtual void read_mode(file_input_buffer& input,
                                 output_buffer_type& output);
        }; // class rle_bitmap_decoder

        /** \brief RLE decoder for 4 bpp bitmap images. */
        typedef rle_bitmap_decoder<rle_bitmap_output_buffer<true> >
            rle4_decoder;

        /** \brief RLE decoder for 8 bpp bitmap images. */
        typedef rle_bitmap_decoder<rle_bitmap_output_buffer<false> >
            rle8_decoder;

        /**
         * \brief Functor converting a 1bpp buffer to a 32bpp buffer.
         */
        class pixel1_to_pixel32
        {
        public:
          void operator()(scanline& dest, const char* src,
                          const color_palette_type& palette) const;
        }; // class pixel1_to_pixel32

        /**
         * \brief Functor converting a 4bpp buffer to a 32bpp buffer.
         */
        class pixel4_to_pixel32
        {
        public:
          void operator()(scanline& dest, const char* src,
                          const color_palette_type& palette) const;
        }; // class pixel4_to_pixel32

        /**
         * \brief Functor converting a 8bpp buffer to a 32bpp buffer.
         */
        class pixel8_to_pixel32
        {
        public:
          void operator()(scanline& dest, const char* src,
                          const color_palette_type& palette) const;
        }; // class pixel8_to_pixel32

        /**
         * \brief Functor converting a 24bpp buffer to a 32bpp buffer.
         */
        class pixel24_to_pixel32
        {
        public:
          void operator()(scanline& dest, const char* src,
                          const color_palette_type& palette) const;
        }; // class pixel24_to_pixel32

      public:
        reader(image& img);
        reader(image& img, std::istream& f);

        void load(std::istream& f);

      private:
        void load_palette(const header& h, std::istream& f,
                          color_palette_type& palette) const;

        void load_1bpp(const header& h, std::istream& f);
        void load_4bpp(const header& h, std::istream& f);
        void load_8bpp(const header& h, std::istream& f);
        void load_24bpp(const header& h, std::istream& f);

        void load_4bpp_rle(const header& h, std::istream& f,
                           const color_palette_type& palette);
        void load_4bpp_rgb(const header& h, std::istream& f,
                           const color_palette_type& palette);
        void load_8bpp_rle(const header& h, std::istream& f,
                           const color_palette_type& palette);
        void load_8bpp_rgb(const header& h, std::istream& f,
                           const color_palette_type& palette);

        template <typename Convert>
        void load_rgb_data(std::istream& f, unsigned int buffer_size,
                           const color_palette_type& palette,
                           const Convert& pixel_convert);

      private:
        /** \brief The image in which we store the data we read. */
        image& m_image;

      }; // class reader

      /**
       * \brief This class write an image in a bitmap file.
       * \author Julien Jorge
       */
      class writer : private file_structure
      {
      public:
        writer(const image& img);
        writer(const image& img, std::ostream& f);

        void save(std::ostream& f) const;

      private:
        void save_data(std::ostream& f) const;

        void pixel32_to_pixel24(char* dest, const scanline& src) const;

        void init_header(header& h) const;

      private:
        /** \brief The image from which we read the data. */
        const image& m_image;

      }; // class writer

    public:
      bitmap(unsigned int w, unsigned int h);
      bitmap(const image& that);
      bitmap(std::istream& f);

      void save(std::ostream& f) const;
    }; // class bitmap

  }
}

#include <claw/graphic/bitmap_reader.tpp>

#endif // __CLAW_BITMAP_HPP__
