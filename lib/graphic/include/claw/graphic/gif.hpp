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
 * \file gif.hpp
 * \brief Image class for gif files.
 * \author Julien Jorge
 */
#ifndef __CLAW_GIF_HPP__
#define __CLAW_GIF_HPP__

#include <claw/graphic/color_palette.hpp>
#include <claw/graphic/image.hpp>

#include <claw/functional.hpp>
#include <claw/iterator.hpp>
#include <claw/lzw_decoder.hpp>
#include <claw/types.hpp>

#include <list>

namespace claw
{
  namespace graphic
  {
    /**
     * \brief A class for gif pictures.
     * \author Julien Jorge
     */
    class gif : public image
    {
    public:
      /** \brief One frame in the animation. */
      class frame : public image
      {
      public:
        /** \brief The type of the parent class. */
        typedef image super;

      public:
        frame();
        frame(std::size_t w, std::size_t h);

        void set_delay(unsigned int d);
        unsigned int get_delay() const;

      private:
        /** \brief Hundredths of a seconds to wait before continuing with the
            next frame. */
        unsigned int m_delay;

      }; // class frame

    private:
      /** \brief A list of frame stored in the gif. */
      typedef std::list<frame*> frame_list;

      /** \brief The type of the parent class. */
      typedef image super;

    public:
      /** \brief Iterator on the content of the gif. */
      typedef wrapped_iterator<frame, frame_list::iterator,
                               claw::dereference<frame> >::iterator_type
          frame_iterator;

      /** \brief Iterator on the content of the gif. */
      typedef wrapped_iterator<const frame, frame_list::const_iterator,
                               claw::const_dereference<frame> >::iterator_type
          const_frame_iterator;

    private:
#pragma pack(push, 1)

      /** \brief Header of the gif files. */
      struct header
      {
        /** \brief Signature of the format. */
        u_int_8 signature[3];

        /** \brief Version of the gif format. */
        u_int_8 version[3];

      }; // struct header

      /** \brief Some informations on the screen where the image is rendered.
       */
      struct screen_descriptor
      {
      public:
        bool has_global_color_table() const;
        unsigned int color_palette_size() const;

      public:
        /** \brief Logical screen width. */
        u_int_16 screen_width;

        /** \brief Logical screen height. */
        u_int_16 screen_height;

        /** \brief Some flags. */
        u_int_8 packed;

        /** \brief Background color index. */
        u_int_8 background_color;

        /** \brief Pixel aspect ratio. */
        u_int_8 aspect_ratio;

      }; // struct screen_descriptor

      /** Description of an image in the file. */
      struct image_descriptor
      {
      public:
        /** \brief Identifier of the block. */
        static const u_int_8 block_id = 0x2C;

      public:
        bool has_color_table() const;
        bool is_interlaced() const;
        unsigned int color_palette_size() const;

      public:
        /** \brief Left position in the logical screen. */
        u_int_16 left;

        /** \brief Top position in the logical screen. */
        u_int_16 top;

        /** \brief Width of the image. */
        u_int_16 width;

        /** \brief Height of the image. */
        u_int_16 height;

        /** \brief Some flags. */
        u_int_8 packed;

      }; // struct image_descriptor

      /** \brief Extension of the format. */
      struct extension
      {
        /** \brief Identifier of the block. */
        static const u_int_8 block_id = 0x21;

        // no field
      }; // struct extension

      /** \brief The end of the file. */
      struct trailer
      {
        /** \brief Identifier of the block. */
        static const u_int_8 block_id = 0x3B;

        // no field
      }; // trailer

      /** \brief Extension describing a rendering block. */
      struct graphic_control_extension
      {
      public:
        /** \brief Identifier of the extension. */
        static const u_int_8 block_label = 0xF9;

        /** \brief Tell how to initialise the canvas before rendering a
            frame. */
        enum disposal_method
        {
          /** \brief No disposal specified. The decoder is not required to
              take any action. */
          dispose_none,

          /** \brief Do not dispose. The graphic is to be left in place. */
          dispose_do_not_dispose,

          /** \brief Restore to background color. The area used by the graphic
              must be restored to the background color. */
          dispose_background,

          /** \brief Restore to previous. The decoder is required to restore
              the area overwritten by the graphic with what was there prior to
              rendering the graphic. */
          dispose_previous

        }; // enum disposal_method

      public:
        disposal_method get_disposal_method() const;
        bool has_transparent_color() const;

      public:
        /** \brief Number of bytes in the block. */
        u_int_8 block_size;

        /** \brief Some flags. */
        u_int_8 packed;

        /** \brief Hundredths of a second to wait before continuing. */
        u_int_16 delay;

        /** \brief Index of the transparent color in the palette. */
        u_int_8 transparent_color;

        /** \brief Block terminator. */
        u_int_8 terminator;

      }; // struct graphic_control_extension

      /** \brief Extension commenting the file. */
      struct comment_extension
      {
      public:
        /** \brief Identifier of the extension. */
        static const u_int_8 block_label = 0xFE;

      public:
        // this block is ignored.

      }; // struct comment_extension

      /** \brief Extension adding graphic text support. */
      struct plain_text_extension
      {
      public:
        /** \brief Identifier of the extension. */
        static const u_int_8 block_label = 0x01;

      public:
        // this block is ignored.

      }; // struct plain_text_extension

      /** \brief Extension adding some informations about the application. */
      struct application_extension
      {
      public:
        /** \brief Identifier of the extension. */
        static const u_int_8 block_label = 0xFF;

      public:
        // this block is ignored.

      }; // struct application_extension
#pragma pack(pop)

    public:
      /**
       * \brief This class reads data from a gif file. The image is resized to
       *        the size of the screen (as defined in the gif file) and the
       *        frames are stored in a list of frames passed as parameter.
       * \author Julien Jorge
       */
      class reader
      {
      private:
        /** \brief The type of the color palettes in the file. */
        typedef color_palette<rgb_pixel> palette_type;

        /** \brief Some global data needed when reading the file. */
        struct reader_info
        {
          /** \brief Description of the screen. */
          screen_descriptor sd;

          /** \brief The global palette. */
          palette_type* palette;

          /** \brief The index of the transparent color for the current
           * frame.*/
          int transparent_color_index;

          /** \brief The disposal method of the frames. */
          std::vector<graphic_control_extension::disposal_method>
              disposal_method;

        }; // struct reader_info

        /** \brief The buffer passed to the LZW decoder to decode the input. */
        class input_buffer
        {
        public:
          input_buffer(std::istream& is, u_int_8 code_size);

          bool end_of_data() const;
          bool end_of_information() const;
          unsigned int symbols_count() const;
          unsigned int get_next();

          void reset();
          void new_code(unsigned int code);

        private:
          void fill_buffer();

        private:
          /** \brief The last value read from the input. */
          unsigned int m_val;

          /** \brief The input data. */
          std::istream& m_input;

          /**
           * \brief Encoded data read from the stream.
           *
           * A data block is 255 bytes long, maximum. But we may have to keep
           * up to 12 bits from the previous block. So we need two more bytes.
           */
          char m_buffer[257];

          /** \brief The position of the next byte to read in \a m_buffer. */
          std::size_t m_pending;

          /** \brief The number of available bits in m_buffer[m_pending]. */
          unsigned char m_pending_bits;

          /** \brief The end of the data in \a m_buffer; */
          std::size_t m_pending_end;

          /** \brief The length of the next data block. */
          u_int_8 m_next_data_length;

          /** \brief The initial size of the codes. */
          const unsigned int m_initial_code_size;

          /** \brief The current size of the codes. */
          unsigned int m_code_size;

          /** \brief The maximum code allowed with the current code size. */
          unsigned int m_code_limit;

        }; // class input_buffer

        /** \brief Buffer passed to the LZW decoder to write decoded data. */
        class output_buffer
        {
        public:
          output_buffer(const palette_type& p, const image_descriptor& id,
                        int transparent_color_index, image& output);

          void write(unsigned int code);

        private:
          /** \brief The palette from whick we take the colors of the image. */
          const palette_type& m_palette;

          /** \brief The image descriptor of the frame. */
          const image_descriptor& m_id;

          /** \brief The index of the transparent color. -1 if none. */
          const int m_transparent_color_index;

          /** \brief The image in which we save the decoded data. */
          image& m_output;

          /** \brief The x-position of the next pixel in the image. */
          std::size_t m_x;

          /** \brief The y-position of the next pixel in the image. */
          std::size_t m_y;

          /** \brief Current pass in an interlaced image [0-3]. */
          int m_interlace_pass;

          /** \brief Increment in the current pass of an interlaced image. */
          int m_interlace_step;

        }; // class output_buffer

        /** \brief LZW decoder for the GIF data. */
        typedef claw::lzw_decoder<input_buffer, output_buffer> gif_lzw_decoder;

      public:
        reader(image& img);
        reader(image& img, std::istream& f);
        reader(frame_list& frames, std::istream& f);
        reader(image& img, frame_list& frames, std::istream& f);
        ~reader();

        void load(std::istream& f);

      private:
        void clear();
        void inside_load(std::istream& f);
        void make_frames(const reader_info& info);
        void fill_background(image& img, const reader_info& info) const;

        void check_if_gif(std::istream& f) const;
        void read_screen_descriptor(std::istream& f, reader_info& info);

        void read_palette(std::istream& f, palette_type& p) const;
        void read_data(std::istream& f, reader_info& info);
        void read_frame(std::istream& f, reader_info& info);
        void read_frame_with_gce(std::istream& f, reader_info& info);

        void skip_extension(std::istream& f) const;
        void read_frame_data(std::istream& f, const reader_info& info,
                             frame& the_frame) const;

        void decode_data(std::istream& f, const palette_type& palette,
                         const image_descriptor& id,
                         int transparent_color_index, frame& the_frame) const;

      private:
        /** \brief The gif file on which we work. */
        image* m_image;

        /** \brief The images in which we store the data we read. */
        frame_list m_frame;

      }; // class reader

    public:
      gif();
      gif(const gif& that);
      gif(std::istream& f);
      ~gif();

      gif& operator=(const gif& that);
      void swap(gif& that);

      frame_iterator frame_begin();
      frame_iterator frame_end();
      const_frame_iterator frame_begin() const;
      const_frame_iterator frame_end() const;

    private:
      /** \brief The list of frames in the animation. */
      frame_list m_frame;

    }; // class gif
  }
}

namespace std
{
  void swap(claw::graphic::gif& a, claw::graphic::gif& b);
}

#endif // __CLAW_GIF_HPP__
