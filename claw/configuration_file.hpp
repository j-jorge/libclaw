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
 * \file configuration_file.hpp
 * \brief A class to get the content of a configuration file.
 * \author Julien Jorge.
 */
#ifndef __CLAW_CONFIGURATION_FILE_HPP__
#define __CLAW_CONFIGURATION_FILE_HPP__

#include <claw/iterator.hpp>
#include <claw/functional.hpp>

#include <iostream>
#include <map>
#include <string>

namespace claw
{
  /**
   * \brief A class to get the content of a configuration file.
   * \author Julien Jorge.
   */
  class configuration_file
  {
  public:
    /** \brief This class tells us how to parse the input file. */
    struct syntax_description
    {
    public:
      /** \brief Two symbols making a pair (like () or []). */
      typedef std::pair<char, char> paired_symbol;

    public:
      syntax_description();

      std::string make_comment( const std::string& value ) const;
      std::string make_assignment
      ( const std::string& key, const std::string& value ) const;
      std::string make_section_name( const std::string& name ) const;

    public:
      /** \brief Symbol used to comment the rest of the line. */
      char comment;

      /** \brief Symbol used to assign a value to a field. */
      char assignment;

      /** \brief Pair of symbols around a section name. */
      paired_symbol section_name;
      
    }; // struct syntax_descritpion

  private:
    /** \brief The content of a section. */
    typedef std::multimap<std::string, std::string> section_content;

    /** \brief The sections in the file. */
    typedef std::map<std::string, section_content> file_content;

    /** \brief Pointer to a section_content. */
    typedef section_content* section_content_ptr;

  public:
    /** \brief Iterator on the name of the sections. */
    typedef claw::wrapped_iterator
    < const file_content::key_type,
      file_content::const_iterator,
      const_pair_first<file_content::value_type>
      >::iterator_type const_file_iterator;

    /** \brief Iterator on the fields of a section. */
    typedef claw::wrapped_iterator
    < const section_content::key_type,
      section_content::const_iterator,
      const_pair_first<section_content::value_type>
      >::iterator_type const_section_iterator;

    /**
     * \brief This class is an iterator on the values set for a same field name.
     */
    class const_field_iterator
    {
    private:
      /** \brief Iterator on the fields. */
      typedef section_content::const_iterator wrapped_iterator_type;

    public:
      typedef std::string value_type;
      typedef const value_type& reference;
      typedef const value_type* pointer;
      typedef wrapped_iterator_type::difference_type difference_type;

      typedef wrapped_iterator_type::iterator_category iterator_category;

    public:
      const_field_iterator() {}
      const_field_iterator( wrapped_iterator_type it ) : m_iterator(it) {}

      bool operator==( const const_field_iterator& that ) const
      {
	return m_iterator == that.m_iterator;
      } // operator==()

      bool operator!=( const const_field_iterator& that ) const
      {
	return m_iterator != that.m_iterator;
      } // operator!=()

      const_field_iterator& operator++()
      {
	++m_iterator;
	return *this;
      } // operator++()

      const_field_iterator operator++(int)
      {
	const_field_iterator tmp(*this);
	++m_iterator;
	return tmp;
      } // operator++() [post]

      const_field_iterator& operator--()
      {
	--m_iterator;
	return *this;
      } // operator--()

      const_field_iterator operator--(int)
      {
	const_field_iterator tmp(*this);
	--m_iterator;
	return tmp;
      } // operator--() [post]

      reference operator*() const
      {
	return m_iterator->second;
      } // operator*()

      pointer operator->() const
      {
	return &m_iterator->second;
      } // operator->()

    private:
      /** \brief Iterator on a section content. */
      wrapped_iterator_type m_iterator;

    }; // class const_field_iterator

  public:
    configuration_file();
    configuration_file
    (std::istream& is, const syntax_description& syntax = syntax_description());

    bool open
    (std::istream& is, const syntax_description& syntax = syntax_description());
    void save
    (std::ostream& os, const syntax_description& syntax = syntax_description());

    const std::string&
    operator()( const std::string& section, const std::string& field ) const;

    const std::string& operator()( const std::string& field ) const;

    bool has_field
    ( const std::string& section, const std::string& field ) const;
    bool has_field( const std::string& field ) const;

    void set_value
    ( const std::string& section, const std::string& field,
      const std::string& val );
    void set_value( const std::string& field, const std::string& val );

    void add_value
    ( const std::string& section, const std::string& field,
      const std::string& val );
    void add_value( const std::string& field, const std::string& val );

    void clear_section( const std::string& section );

    const_field_iterator
    field_begin( const std::string& section, const std::string& field ) const;
    const_field_iterator
    field_end( const std::string& section, const std::string& field ) const;

    const_field_iterator field_begin( const std::string& field ) const;
    const_field_iterator field_end( const std::string& field ) const;

    const_section_iterator section_begin() const;
    const_section_iterator section_end() const;

    const_section_iterator section_begin( const std::string& section ) const;
    const_section_iterator section_end( const std::string& section ) const;

    const_file_iterator file_begin() const;
    const_file_iterator file_end() const;

  private:
    bool get_line( std::istream& is, const syntax_description& syntax,
		   std::string& line ) const;
    bool
    process_line( const std::string& line, const syntax_description& syntax,
		  section_content_ptr& section );

    void escape_line( std::istream& is, const syntax_description& syntax,
		      std::string& line ) const;

    void escape_char
    ( char escaped, const syntax_description& syntax, std::string& str ) const;

    void save_section_content
    ( const section_content& c, std::ostream& os,
      const syntax_description& syntax ) const;

  private:
    /** \brief The fields set outside a section. */
    section_content m_noname_section;

    /** \brief All the sections and their content. */
    file_content m_sections;

    /** \brief String returned when asking for a not filled field. */
    static const std::string s_unknow_field_value;

  }; // class configuration_file
} // namespace claw

#endif // __CLAW_CONFIGURATION_FILE_HPP__
