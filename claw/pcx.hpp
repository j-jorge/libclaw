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
 * \file pcx.hpp
 * \brief A class for pcx pictures.
 * \author Julien Jorge
 */
#ifndef __CLAW_PCX_HPP__
#define __CLAW_PCX_HPP__

#include <iostream>
#include <claw/image.hpp>
#include <claw/rle_decoder.hpp>
#include <claw/rle_encoder.hpp>
#include <claw/color_palette.hpp>
#include <claw/types.hpp>
#include <claw/buffered_istream.hpp>

namespace claw
{
  namespace graphic
  {
    /**
     * \brief A class for pcx pictures.
     * \author Julien Jorge
     */
    class pcx : public image
    {
    private:
      /* Some convenient renaming. */
      typedef unsigned_integer_of_size<8>::type u_int_8;
      typedef unsigned_integer_of_size<16>::type u_int_16;

      typedef integer_of_size<8>::type int_8;
      typedef integer_of_size<16>::type int_16;

      enum format_version
        {
          v_2_5                 = 0,
          v_2_8_with_palette    = 2,
          v_2_8_without_palette = 3,
          v_win                 = 4,
          v_3_0                 = 5
        }; // enum format_version

#       pragma pack (push,1)

        /*--------------------------------------------------------------------*/
        /**
         * \brief Header of a pcx file.
         */
        class header
        {
        public:
          /** \brief Manufacturer (should be == 10). */
          u_int_8 manufacturer;

          /** \brief Format version. */
          u_int_8 version;

          /** \brief Tell if the file is encoded (RLE). */
          u_int_8 encoded;

          /** \brief Number of bits per pixel. */
          u_int_8 bpp;

          struct
          {
            /** \brief Minimum X-coordinate of the image. */
            u_int_16 x_min;
            
            /** \brief Minimum Y-coordinate of the image. */
            u_int_16 y_min;

            /** \brief Maximum X-coordinate of the image. */
            u_int_16 x_max;

            /** \brief Maximum Y-coordinate of the image. */
            u_int_16 y_max;

          } window;

          /** \brief Horizontal resolution of image in DPI. */
          u_int_16 horizontal_dpi;

          /** \brief Vertical resolution of image in DPI. */
          u_int_16 vertical_dpi;

          /** \brief Color map. */
          rgb_pixel_8 color_map[16];

          /** \brief Reserved. Should be set to zero. */
          u_int_8 reserved;

          /** \brief Number of color planes. */
          u_int_8 color_planes;

          /** \brief Number of bytes to allocate for a scanline plane. Must be
              an even number. */
          u_int_16 bytes_per_line;

          /** \brief How to interpret the palette. */
          u_int_16 palette_info;

          /** \brief Screen size in pixels. */
          struct
          {
            /** \brief Horizontal screen size. */
            u_int_16 horizontal;

             /** \brief Vertical screen size. */
            u_int_16 vertical;

          } screen_size;

          /** \brief Zero bytes to fill the 128 bytes header. */
          u_int_8 filler[54];
        }; // struct header
#       pragma pack (pop)

      
      /*----------------------------------------------------------------------*/
      /** \brief A color palette of RGBA colors. */
      typedef color_palette<rgba_pixel_8> color_palette32;

      /** \brief The type in which we store a scan line of a color plane. */
      typedef std::vector<u_int_8> color_plane_type;

    public:
      /*----------------------------------------------------------------------*/
      /**
       * \brief This class read data from a pcx file and store it in an
       *        image.
       * \author Julien Jorge
       */
      class reader
      {
      private:
        /*--------------------------------------------------------------------*/
        /**
         * \brief The type of the input buffer associated with the file when
         *        decoding RLE files.
         */
        typedef buffered_istream<std::istream> rle_pcx_input_buffer;

        /*--------------------------------------------------------------------*/
        /**
         * \brief The output buffer for the RLE decoder.
         */
        class rle_pcx_output_buffer
        {
        public:
          rle_pcx_output_buffer( color_plane_type& result );
        
          void fill( unsigned int n, u_int_8 pattern );
          void copy( unsigned int n, rle_pcx_input_buffer& buffer );
        
          bool completed() const;

        private:
          /** \brief We save uncompressed in this buffer. */
          color_plane_type& m_result;

          /** \brief Position of the next byte to write in m_result. */
          unsigned int m_position;

        }; // class rle_pcx_output_buffer

        /*--------------------------------------------------------------------*/
        /**
         * \brief RLE decoder for pcx RLE format
         */
        class rle_pcx_decoder
          : public rle_decoder< u_int_8,
                                rle_pcx_input_buffer,
                                rle_pcx_output_buffer >
        {
        private:
          virtual void
          read_mode( input_buffer_type& input, output_buffer_type& output );

        }; // class rle_pcx_decoder

        /*--------------------------------------------------------------------*/
        /**
         * \brief Function object that converts a scanline of a monochrome pcx
         *        into 32 bpp pixels.
         */
        class converter_mono
        {
        public:
          void operator()
            ( const std::vector<color_plane_type>& scanline, image& img,
              unsigned int y ) const;
        }; // class converter_mono

        /*--------------------------------------------------------------------*/
        /**
         * \brief Function object that converts a scanline of a 4bpp color
         *        mapped pcx into 32 bpp pixels.
         */
        class converter_16
        {
        public:
          converter_16( const header& h );
          void operator()
            ( const std::vector<color_plane_type>& scanline, image& img,
              unsigned int y ) const;

        private:
          /** \brief The header of the file. It contains the color palette. */
          const header& m_header;

        }; // class converter_16

        /*--------------------------------------------------------------------*/
        /**
         * \brief Function object that converts a scanline of a 8bpp color
         *        mapped pcx into 32 bpp pixels.
         */
        class converter_256
        {
        public:
          converter_256( const color_palette32& palette );
          void operator()
            ( const std::vector<color_plane_type>& scanline, image& img,
              unsigned int y ) const;

        private:
          /** \brief The color palette. */
          const color_palette32& m_palette;

        }; // class converter_256

        /*--------------------------------------------------------------------*/
        /**
         * \brief Function object that converts a scanline of a 3 planes true
         *        color scanline into 32 bpp pixels.
         */
        class converter_true_color
        {
        public:
          void operator()
            ( const std::vector<color_plane_type>& scanline, image& img,
              unsigned int y ) const;

        }; // class converter_true_color

      public:
        reader( image& img );
        reader( image& img, std::istream& f );

        void load( std::istream& f );

      private:
        void check_if_pcx( const header& h ) const;

        void load_mono( const header& h, std::istream& f );
        void load_16_color_mapped( const header& h, std::istream& f );
        void load_true_color( const header& h, std::istream& f );
        void load_256_color_mapped( const header& h, std::istream& f );

        void
        decompress_line( std::istream& f, color_plane_type& scanline ) const;

        template<typename Converter>
        void decompress
        ( const header& h, std::istream& f, const Converter& convert );

      private:
        /** \brief The image in which we store the data we read. */
        image& m_image;

      }; // class reader

      /*----------------------------------------------------------------------*/
      /**
       * \brief This class write an image in a pcx file.
       * \author Julien Jorge
       */
      class writer
      {
      public:
        /*--------------------------------------------------------------------*/
        /**
         * \brief The type of the output buffer associated with the file when
         *        encoding RLE data.
         */
        class file_output_buffer
        {
        public:
          /** \brief The typ of the output patterns. */
          typedef u_int_8 pattern_type;

        public:
          file_output_buffer( std::ostream& os );
          void encode( unsigned int n, pattern_type pattern );

          template<typename Iterator>
          void raw( Iterator first, Iterator last );

          unsigned int min_interesting() const;
          unsigned int max_encodable() const;

        private:
          /** \brief The stream in which we write. */
          std::ostream& m_stream;

        }; // class file_output_buffer

        /*--------------------------------------------------------------------*/
        /**
         * \brief RLE encoder for pcx format
         * \author Julien Jorge
         */
        typedef rle_encoder<file_output_buffer> rle_pcx_encoder;

      public:
        writer( const image& img );
        writer( const image& img, std::ostream& f );

        void save( std::ostream& os ) const;

      private:
        void write_header
        ( std::ostream& os, unsigned int bytes_per_line ) const;
        void save_rle_true_color
        ( std::ostream& os, unsigned int bytes_per_line ) const;

      private:
        /** \brief The image from which we read the data. */
        const image& m_image;

      }; // class writer

    public:
      pcx( unsigned int w, unsigned int h );
      pcx( const image& that );
      pcx( std::istream& f );

      void save( std::ostream& os ) const;

    }; // class pcx
  } // namespace graphic
} // namespace claw

#include <claw/impl/pcx_writer.tpp>
#include <claw/impl/pcx_reader.tpp>

#endif // __CLAW_PCX_HPP__
