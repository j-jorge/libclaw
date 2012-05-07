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
 * \file xbm.hpp
 * \brief A class for xbm pictures.
 * \author Julien Jorge
 */
#ifndef __CLAW_XBM_HPP__
#define __CLAW_XBM_HPP__

#include <claw/image.hpp>
#include <iostream>
#include <string>

namespace claw
{
  namespace graphic
  {
    /**
     * \brief A class for xbm pictures.
     * \author Julien Jorge
     */
    class xbm : public image
    {
    public:
      /*----------------------------------------------------------------------*/
      /**
       * \brief This class read data from a xbm file and store it in an image.
       * \author Julien Jorge
       */
      class reader
      {
      public:
        reader( image& img );
        reader( image& img, std::istream& f );
        reader( xbm& img, std::istream& f );
        ~reader();

        void load( std::istream& f );

      private:
        void read_from_file( std::istream& f );

        void read_size( std::istream& f );
        unsigned int read_dim( const std::string& line ) const;
        unsigned int read_bits_per_entry( std::istream& f ) const;
        void read_name( std::istream& f );
        void read_pixels( std::istream& f, unsigned int bpe ) const;

        void read_line
        ( std::istream& f, std::string& line, char endchar ) const;
        void remove_comments
        ( std::istream& f, std::string& line, char endchar ) const;

      private:
        /** \brief The image in which we store the data we read. */
        image& m_image;

        /** \brief The name of the xbm image. */
        std::string m_name;

        /** \brief The position of the hot spot in the image. */
        claw::math::coordinate_2d<int>* m_hot;

      }; // class reader

      /*----------------------------------------------------------------------*/
      /**
       * \brief This class write an image in a xbm file.
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

          options( const std::string& n,
                   const claw::math::coordinate_2d<int>* h );

        public:
          /** \brief The name of the image structure in the file. */
          std::string name;

          /** \brief The position of the hot spot in the image. */
          const claw::math::coordinate_2d<int>* hot;

        }; // options

      public:
        writer( const image& img );
        writer( const image& img, std::ostream& f,
                const options& opt = options() );
        
        void save( std::ostream& f, const options& opt = options() ) const;

      private:
        void save_bits( std::ostream& f ) const;

      private:
        /** \brief The image from which we take the data to save. */
        const image& m_image;

      }; // class writer

    public:
      xbm( unsigned int w, unsigned int h );
      xbm( const image& that );
      xbm( std::istream& f );
      ~xbm();

      void save( std::ostream& os ) const;

      void set_name( const std::string& name );
      void set_hot( const claw::math::coordinate_2d<int>& hot );

    private:
      /** \brief The name of the image structure in the file. */
      std::string m_name;

      /** \brief The position of the hot spot in the image. */
      claw::math::coordinate_2d<int>* m_hot;

    }; // class xbm
  } // namespace graphic
} // namespace claw

#endif // __CLAW_XBM_HPP__
