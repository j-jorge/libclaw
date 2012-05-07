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
 * \file targa.hpp
 * \brief A class for targa pictures.
 * \author Julien Jorge
 */
#ifndef __CLAW_TARGA_HPP__
#define __CLAW_TARGA_HPP__

#include <iostream>
#include <claw/image.hpp>
#include <claw/rle_decoder.hpp>
#include <claw/rle_encoder.hpp>
#include <claw/color_palette.hpp>
#include <claw/buffered_istream.hpp>

namespace claw
{
  namespace graphic
  {
    /**
     * \brief A class for targa pictures.
     * \author Julien Jorge
     */
    class targa : public image
    {
    private:
      /*----------------------------------------------------------------------*/
      /**
       * \brief Tool class used for defining the structures of the datas stored
       *        in a targa file.
       * \author Julien Jorge
       */
      class file_structure
      {
      public:
        enum image_coding
          {
            color_mapped        = 1,
            true_color          = 2,
            black_and_white     = 3,
            rle_color_mapped    = 9,
            rle_true_color      = 10,
            rle_black_and_white = 11
          }; // enum image_coding

#       pragma pack (push,1)

        /*--------------------------------------------------------------------*/
        /**
         * \brief Header of a targa file.
         */
        class header
        {
        public:
          header();
          header( unsigned int w, unsigned int h );

        public:
          /** \brief Image identifier length. */
          char id_length;
          /** \brief 1 if there is a color map, 0 otherwise. */
          char color_map;
          /** \brief Image type. */
          char image_type;

          /** \brief Color map specification. */
          struct
          {
            /** \brief Index of the first color map entry. */
            unsigned short first_entry_index;
            /** \brief Total number of color map entries included. */
            unsigned short length;
            /** \brief Number of bits per enrty. */
            unsigned char entry_size;
          } color_map_specification;

          /** \brief Image specification. */
          struct specification
          {
            /** \brief Lower left corner X-origin. */
            unsigned short x_origin;
            /** \brief Lower left corner Y-origin. */
            unsigned short y_origin;
            /** \brief Image width. */
            unsigned short width;
            /** \brief Image height. */
            unsigned short height;
            /** \brief Bits per pixel. */
            unsigned char bpp;
            /** \brief descriptor. */
            unsigned char descriptor;

            bool up_down_oriented() const ;
            bool left_right_oriented() const ;
            unsigned char alpha() const;
          } image_specification;
        }; // struct header

        /*--------------------------------------------------------------------*/
        /**
         * \brief Item in the developper directory.
         */
        struct developer_item
        {
          /** \brief Item identifier. */
          unsigned short tag;
          /** \brief Offset in the file. */
          unsigned int offset;
          /** \brief Fielf size. */
          unsigned int size;
        }; // struct developer_item

        /*--------------------------------------------------------------------*/
        /**
         * \brief Extension area.
         * \todo Describe this the day it seems useful.
         */
        struct extension
        {
           
        }; // struct extension

        /*--------------------------------------------------------------------*/
        /**
         * \brief Footer of a targa file.
         */
        class footer
        {
        public:
          footer();

          bool is_valid() const;

        public:
          /** \brief Offset of the extension area. */
          unsigned int extension_offset;

          /** \brief Offset of the developer directory. */
          unsigned int developer_offset;

          /** \brief Footer identier. Must be as long as
              std::string("TRUEVISION-XFILE.") + 1 (for the last '\\0').*/
          char signature[18];

        private:
          /** \brief The string content in the signature. */
          static const std::string s_signature;

        }; // struct footer
#       pragma pack (pop)

      }; // class file_structure

      /*----------------------------------------------------------------------*/
      /**
       * \brief A pixel color in 16 bits in targa files.
       *
       * \remark We need a type for template specialisation. No content is
       *         needed.
       */
      struct pixel16
      {
      }; // struct pixel16

      /*----------------------------------------------------------------------*/
      /**
       * \brief A pixel color in 8 bits in targa files.
       *
       * \remark We need a type for template specialisation. No content is
       *         needed.
       */
      struct pixel8
      {
      }; // struct pixel8

      /*----------------------------------------------------------------------*/
      /**
       * \brief A color palette of RGBA colors.
       */
      typedef color_palette<rgba_pixel_8> color_palette32;

    public:
      /*----------------------------------------------------------------------*/
      /**
       * \brief This class read data from a targa file and store it in an
       *        image.
       * \author Julien Jorge
       */
      class reader : private file_structure
      {
      private:
        /*--------------------------------------------------------------------*/
        /**
         * \brief The type of the input buffer associated with the file when
         *        decoding RLE files.
         *
         * \b Template \b parameters
         * - \a Pixel, the type of the pixels in the input buffer.
         */
        template<typename Pixel>
        class file_input_buffer : public buffered_istream<std::istream>
        {
        private:
          /** \brief The type of the pixels in the input buffer. */
          typedef Pixel pixel_type;

        public:
          file_input_buffer( std::istream& f );
          rgba_pixel_8 get_pixel();

        }; // class file_input_buffer

        /*--------------------------------------------------------------------*/
        /**
         * \brief The type of the input buffer associated with the file when
         *        decoding RLE files using a color palette.
         *
         * \b Template \b parameters
         * - \a Pixel, the type of the pixels in the input buffer.
         */
        template<typename Pixel>
        class mapped_file_input_buffer:
          public buffered_istream<std::istream>
        {
        private:
          /** \brief The type of the pixels in the input buffer. */
          typedef Pixel pixel_type;

        public:
          mapped_file_input_buffer( std::istream& f, const color_palette32& p );
          rgba_pixel_8 get_pixel();

        private:
          /** \brief The color map of the image. */
          const color_palette32& m_palette;

        }; // class mapped_file_input_buffer

        /*--------------------------------------------------------------------*/
        /**
         * \brief The output buffer for the RLE decoder.
         *
         * \b Template \b parameters
         * - \a InputBuffer, the type of the input buffer.
         *
         * \author Julien Jorge
         */
        template< typename InputBuffer >
        class rle_targa_output_buffer
        {
        private:
          /** \brief The type of he pixels in the input buffer. */
          typedef rgba_pixel_8 pixel_type;

          /** \brief The type of the input buffer. */
          typedef InputBuffer input_buffer_type;

        public:
          rle_targa_output_buffer( image& img, bool up_down, bool left_right );
        
          void fill( unsigned int n, rgba_pixel_8 pattern );
          void copy( unsigned int n, input_buffer_type& buffer );
        
          bool completed() const;

        private:
          void adjust_position(int x);

        private:
          /** \brief The targa image to fill. */
          image& m_image;

          /** \brief Current column index in the image. */
          unsigned int m_x;

          /** \brief Current row index in the image. */
          unsigned int m_y;

          /** \brief Horizontal increment. */
          const int m_x_inc;

          /** \brief Vertical increment. */
          const int m_y_inc;

        }; // class rle_targa_output_buffer

        /*--------------------------------------------------------------------*/
        /**
         * \brief RLE decoder for targa RLE format
         * 
         * \b Template \b parameters :
         * - \a Pixel, the type of the pixels in the file.
         * - \a OutputBuffer, the type of the output buffer.
         *
         * The \a OutputBuffer type must match the type requirements of the
         * template parameter OutputBuffer of the rle_decoder class.
         *
         * \author Julien Jorge
         */
        template< typename InputBuffer,
                  typename OutputBuffer = rle_targa_output_buffer<InputBuffer> >
        class rle_targa_decoder
          : public rle_decoder< rgba_pixel_8, InputBuffer, OutputBuffer >
        {
        public:
          /** \brief Type of the input buffer. */
          typedef InputBuffer input_buffer_type;

          /** \brief Type of the output buffer. */
          typedef OutputBuffer output_buffer_type;

        private:
          virtual void
          read_mode( input_buffer_type& input, output_buffer_type& output );

        }; // class rle_targa_decoder

        /*--------------------------------------------------------------------*/
        /** \brief RLE decoder for 32 bpp targa images. */
        typedef
        rle_targa_decoder< file_input_buffer<rgba_pixel_8> > rle32_decoder;

        /*--------------------------------------------------------------------*/
        /** \brief RLE decoder for 24 bpp targa images. */
        typedef
        rle_targa_decoder< file_input_buffer<rgb_pixel_8> > rle24_decoder;
      
        /*--------------------------------------------------------------------*/
        /** \brief RLE decoder for 16 bpp targa images. */
        typedef rle_targa_decoder< file_input_buffer<pixel16> > rle16_decoder;

        /*--------------------------------------------------------------------*/
        /** \brief RLE decoder for color mapped 8 bpp targa images. */
        typedef rle_targa_decoder< mapped_file_input_buffer<pixel8> >
        rle8_decoder;

      public:
        reader( image& img );
        reader( image& img, std::istream& f );

        void load( std::istream& f );

      private:
        void check_if_targa( std::istream& f ) const;

        void load_palette
        ( const header& h, std::istream& f, color_palette32& palette ) const;

        void load_color_mapped( const header& h, std::istream& f );
        void load_rle_color_mapped( const header& h, std::istream& f );
        void load_true_color( const header& h, std::istream& f );
        void load_rle_true_color( const header& h, std::istream& f );
        
        template<typename Pixel>
        void load_color_mapped_raw
        ( const header& h, std::istream& f, const color_palette32& palette );

        template<typename Decoder>
        void decompress_rle_color_mapped
        ( const header& h, std::istream& f, const color_palette32& palette );

        template<typename Pixel>
        void load_true_color_raw( const header& h, std::istream& f );

        template<typename Decoder>
        void decompress_rle_true_color( const header& h, std::istream& f );

        template<typename Pixel>
        void
        load_palette_content( std::istream& f, color_palette32& palette ) const;

     private:
        /** \brief The image in which we store the data we read. */
        image& m_image;

      }; // class reader

      /*----------------------------------------------------------------------*/
      /**
       * \brief This class write an image in a targa file.
       * \author Julien Jorge
       */
      class writer : private file_structure
      {
      public:
        /*--------------------------------------------------------------------*/
        /**
         * \brief The type of the output buffer associated with the file when
         *        encoding RLE data.
         *
         * \b Template \b parameters
         * - \a Pixel, the type of the pixels in the output buffer.
         */
        template<typename Pixel>
        class file_output_buffer
        {
        public:
          /** \brief The type of the pixels in the input buffer. */
          typedef Pixel pixel_type;

          typedef pixel_type pattern_type;

        public:
          file_output_buffer( std::ostream& os );
          void encode( unsigned int n, pattern_type pattern );

          template<typename Iterator>
          void raw( Iterator first, Iterator last );

          unsigned int min_interesting() const;
          unsigned int max_encodable() const;

          void order_pixel_bytes( const pixel_type& p );

        private:
          /** \brief The stream in which we write. */
          std::ostream& m_stream;

        }; // class file_output_buffer

        /*--------------------------------------------------------------------*/
        /**
         * \brief RLE encoder for targa format
         * 
         * \b Template \b parameters :
         * - \a Pixel, the type of the pixels in the file.
         *
         * \author Julien Jorge
         */
        template<typename Pixel>
        class rle_targa_encoder
          : public rle_encoder< file_output_buffer<Pixel> >
        {
        public:
          /** \brief Type of the output buffer. */
          typedef file_output_buffer<Pixel> output_buffer_type;

        }; // class rle_targa_encoder

        /*--------------------------------------------------------------------*/
        /** \brief RLE encoder for 32 bpp targa images. */
        typedef rle_targa_encoder<rgba_pixel_8> rle32_encoder;

      public:
        writer( const image& img );
        writer( const image& img, std::ostream& f, bool rle );

        void save( std::ostream& f, bool rle ) const;

      private:
        void save_true_color( std::ostream& os ) const;
        void save_rle_true_color( std::ostream& os ) const;

      private:
        /** \brief The image from which we read the data. */
        const image& m_image;

      }; // class writer

    public:
      targa( unsigned int w, unsigned int h );
      targa( const image& that );
      targa( std::istream& f );

      void save( std::ostream& os, bool rle ) const;

    }; // class targa
  } // namespace graphic
} // namespace claw

#include <claw/impl/targa_writer.tpp>
#include <claw/impl/targa_reader.tpp>

#endif // __CLAW_TARGA_HPP__
