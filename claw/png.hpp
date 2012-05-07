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
 * \file png.hpp
 * \brief A class for png pictures.
 * \author Julien Jorge
 */
#ifndef __CLAW_PNG_HPP__
#define __CLAW_PNG_HPP__

#include <claw/image.hpp>
#include <png.h>
#include <setjmp.h>
#include <iostream>
#include <string>

namespace claw
{
  namespace graphic
  {
    /**
     * \brief A class for png pictures.
     * \author Julien Jorge
     */
    class png : public image
    {
    public:
      /*----------------------------------------------------------------------*/
      /**
       * \brief This class read data from a png file and store it in an image.
       * \author Julien Jorge
       */
      class reader
      {
        // classes that need to be accessible from png callbacks.
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

          void read( png_bytep data, png_size_t length );

        private:
          /** \brief The stream from which we get data. */
          std::istream& m_input;

        }; // struct source_manager

      public:
        reader( image& img );
        reader( image& img, std::istream& f );

        void load( std::istream& f );

      private:
        void read_from_file( std::istream& f );
        void check_if_png( png_structp png_ptr, std::istream& f ) const;

        void read_image( png_structp png_ptr, png_infop info_ptr );
        void read_sequential_image( png_structp png_ptr, png_infop info_ptr );
        void read_interlaced_image( png_structp png_ptr, png_infop info_ptr,
                                    unsigned int passes );

        void copy_pixel_line( png_bytep data, unsigned int y );

        void create_read_structures( png_structp& png_ptr,
                                     png_infop& info_ptr ) const;


      private:
        /** \brief The image in which we store the data we read. */
        image& m_image;

        /** \brief Size, in bytes, of a red/green/blue/alpha pixel in a png
            file. */
        static const unsigned int s_rgba_pixel_size;

      }; // class reader

      /*----------------------------------------------------------------------*/
      /**
       * \brief This class write an image in a png file.
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
          /** \brief Compression level in the interlaced image. */
          enum compression_level
            {
              no_compression = Z_NO_COMPRESSION,
              best_speed = Z_BEST_SPEED,
              best_compression = Z_BEST_COMPRESSION,
              default_compression = Z_DEFAULT_COMPRESSION
            }; // enum compression_level

          /** \brief The algorithm to use to interlace the saved image. */
          enum interlace_type
            {
              /** \brief Saved image won't be interaced. */
              none = PNG_INTERLACE_NONE,

              /** \brief Saved image will be interlaced using the Adam7
                  algorithm. */
              adam7 = PNG_INTERLACE_ADAM7
            }; // enum interlace_type

        public:
          options();
          options( compression_level compression_level_,
                   interlace_type interlace_ );

        public:
          /** \brief Compression level to use in the saved stream. */
          compression_level compression;

          /** \brief Interlace method to apply to the saved image. */
          interlace_type interlace;

        }; // struct options

        // classes that need to be accessible from png callbacks.

        /*--------------------------------------------------------------------*/
        /**
         * \brief Target manager that allow us to write in a std::ostream.
         * \author Julien Jorge
         */
        struct target_manager
        {
        public:
          target_manager( std::ostream& os );

          void write( png_bytep data, png_size_t length );
          void flush();

        private:
          /** \brief The stream in which we write the data. */
          std::ostream& m_output;

        }; // struct target_manager

      public:
        writer( const image& img );
        writer( const image& img, std::ostream& f,
                const options& opt = options() );

        void save( std::ostream& f, const options& opt = options() ) const;

      private:
        void set_options( png_structp png_ptr, png_infop info_ptr,
                          const options& opt ) const;
        void save_image( png_structp png_ptr, png_infop info_ptr ) const;
        
        void copy_pixel_line( png_bytep data, unsigned int y ) const;

        void create_write_structures( png_structp& png_ptr,
                                      png_infop& info_ptr ) const;


      private:
        /** \brief The image from which we thake the data to save. */
        const image& m_image;

        /** \brief Size, in bytes, of a red/green/blue/alpha pixel in a png
            file. */
        static const unsigned int s_rgba_pixel_size;

      }; // class writer

    public:
      png( unsigned int w, unsigned int h );
      png( const image& that );
      png( std::istream& f );

      void save( std::ostream& os,
                 const writer::options& opt = writer::options() ) const;

    }; // class png
  } // namespace graphic
} // namespace claw

#endif // __CLAW_PNG_HPP__
