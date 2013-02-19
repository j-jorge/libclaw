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
 * \file jpeg.hpp
 * \brief A class for jpeg pictures.
 * \author Julien Jorge
 */
#ifndef __CLAW_JPEG_HPP__
#define __CLAW_JPEG_HPP__

#include <claw/image.hpp>
#include <setjmp.h>
#include <iostream>
#include <string>
#include <cstdio>

extern "C"
{
#include <jpeglib.h>
}

namespace claw
{
  namespace graphic
  {
    /**
     * \brief A class for jpeg pictures.
     * \author Julien Jorge
     */
    class jpeg : public image
    {
    public:
      /*--------------------------------------------------------------------*/
      /**
       * \brief Error handler that throw an exception instead of exiting the
       *        program.
       * \brief To be used only in the jpeg::reader and jpeg::writer class.
       * \author Julien Jorge.
       */
      struct error_manager
      {
        /** \brief "public" fields, needed by the jpeg library. */
        struct jpeg_error_mgr pub;

        /** \brief For return to caller */
        jmp_buf setjmp_buffer;

        /** \brief A comprehensive description of the error. */
        std::string error_string;

      }; // struct error_manager

      /*----------------------------------------------------------------------*/
      /**
       * \brief This class read data from a jpeg file and store it in an image.
       * \author Julien Jorge
       */
      class reader
      {
        // classes that need to be accessible from jpeg callbacks.
      public:
        /*--------------------------------------------------------------------*/
        /**
         * \brief Source manager that allow us to read from a std::istream.
         * \author Julien Jorge
         */
        struct source_manager
        {
        public:
          source_manager( std::istream& is );
          ~source_manager();

          boolean fill_input_buffer();
          void skip_input_data(long num_bytes);

        public:
          /** \brief "public" fields, needed by the jpeg library. */
          struct jpeg_source_mgr pub;

        private:
          /** \brief The stream from which we get data. */
          std::istream& m_input;

          /** \brief Pointer on the begining of the buffer. */
          const JOCTET* m_buffer;

          /** \brief Number of bytes in the buffer. */
          const unsigned int m_buffer_size;

          /** \brief The size of the stream. */
          unsigned int m_stream_size;

          /** \brief The current position in the stream. */
          unsigned int m_stream_position;

        }; // struct source_manager

      private:
        /*--------------------------------------------------------------------*/
        /**
         * \brief Functor converting a RGB pixel to a ARGB pixel.
         */
        class RGB_to_pixel32
        {
        public:
          rgba_pixel_8 operator()( const JSAMPLE* pixel ) const;
        }; // class RGB_to_pixel32

        /*--------------------------------------------------------------------*/
        /**
         * \brief Functor converting a grey level pixel to a ARGB pixel.
         */
        class grayscale_to_pixel32
        {
        public:
          rgba_pixel_8 operator()( const JSAMPLE* pixel ) const;
        }; // class grayscale_to_pixel32

      public:
        reader( image& img );
        reader( image& img, std::istream& f );

        void load( std::istream& f );

      private:
        template<class Convert>
        void read_data( jpeg_decompress_struct& cinfo,
                        const Convert& pixel_convert );

        void read_from_file( std::istream& f );
        void decompress( std::istream& f, jpeg_decompress_struct& cinfo );

        void create_decompress_info( jpeg_decompress_struct& cinfo,
                                     source_manager& infile ) const;
      private:
        /** \brief The image in which we store the data we read. */
        image& m_image;

      }; // class reader

      /*----------------------------------------------------------------------*/
      /**
       * \brief This class write an image in a jpeg file.
       * \author Julien Jorge
       */
      class writer
      {
      public:
        /**
         * \brief Parameters of the writing algorithm.
         */
        struct options
        {
        public:
          options();
          options( unsigned char compression_quality_, bool progressive_ );

        public:
          /** \brief Quality level to use in the saved stream. */
          unsigned char quality;

          /** \brief Tell if we save a progressive jpeg. */
          bool progressive;

        }; // struct options

        // classes that need to be accessible from jpeg callbacks.

        /*--------------------------------------------------------------------*/
        /**
         * \brief Destination manager that allow us to write in a std::ostream.
         * \author Julien Jorge
         */
        struct destination_manager
        {
        public:
          destination_manager( std::ostream& os );
          ~destination_manager();

          void flush();
          void term();

        public:
          /** \brief "public" fields, needed by the jpeg library. */
          struct jpeg_destination_mgr pub;

        private:
          /** \brief The stream in which we write the data. */
          std::ostream& m_output;

          /** \brief Pointer on the begining of the buffer. */
          JOCTET* m_buffer;

          /** \brief Number of bytes in the buffer. */
          const unsigned int m_buffer_size;

        }; // struct destination_manager

      public:
        writer( const image& img );
        writer( const image& img, std::ostream& f,
                const options& opt = options() );

        void save( std::ostream& f, const options& opt = options() ) const;

      private:
        void set_options( jpeg_compress_struct& cinfo,
                          const options& opt ) const;
        void save_image( jpeg_compress_struct& cinfo ) const;
        
        void copy_pixel_line( JSAMPLE* data, unsigned int y ) const;

        void create_compress_info( jpeg_compress_struct& cinfo,
                                   destination_manager& outfile ) const;

      private:
        /** \brief The image from which we thake the data to save. */
        const image& m_image;

        /** \brief Size, in bytes, of a red/green/blue pixel in a jpeg
            file. */
        static const unsigned int s_rgb_pixel_size;

      }; // class writer

    public:
      jpeg( unsigned int w, unsigned int h );
      jpeg( const image& that );
      jpeg( std::istream& f );

      void save( std::ostream& os,
                 const writer::options& opt = writer::options() ) const;

    }; // class jpeg
  } // namespace graphic
} // namespace claw

#include <claw/impl/jpeg_reader.tpp>

#endif // __CLAW_JPEG_HPP__
