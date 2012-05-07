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
 * \file configuration_file.cpp
 * \brief Implementation of the claw::configuration_file class.
 * \author Julien Jorge
 */
#include <claw/configuration_file.hpp>
#include <claw/assert.hpp>
#include <claw/string_algorithm.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
claw::configuration_file::syntax_description::syntax_description()
  : comment('#'), assignment('='), section_name('[', ']')
{

} // configuration_file::syntax_description::syntax_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a comment from a string.
 * \param value The string to comment.
 */
std::string claw::configuration_file::syntax_description::make_comment
( const std::string& value ) const
{
  return comment + (' ' + value);
} // configuration_file::syntax_description::make_comment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Make an assignment of a value to a key.
 * \param key The key.
 * \param value The value given to the key.
 */
std::string claw::configuration_file::syntax_description::make_assignment
( const std::string& key, const std::string& value ) const
{
  return key + ' ' + assignment + ' ' + value;
} // configuration_file::syntax_description::make_assignment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a section name from a string.
 * \param name The name of the section.
 */
std::string claw::configuration_file::syntax_description::make_section_name
( const std::string& name ) const
{
  return section_name.first + name + section_name.second;
} // configuration_file::syntax_description::make_section_name()




/*----------------------------------------------------------------------------*/
// empty string
const std::string claw::configuration_file::s_unknow_field_value;

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
claw::configuration_file::configuration_file()
{
  // nothing to do
} // configuration_file::configuration_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param is The stream to read from.
 * \param syntax Description of the file's syntax.
 */
claw::configuration_file::configuration_file
( std::istream& is, const syntax_description& syntax )
{
  open(is, syntax);
} // configuration_file::configuration_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the configuration from a stream.
 * \param is The stream to read from.
 * \param syntax Description of the file's syntax.
 */
bool claw::configuration_file::open
( std::istream& is, const syntax_description& syntax )
{
  std::string line;
  bool ok = true;
  section_content_ptr current_section = &m_noname_section;

  while ( get_line(is, syntax, line) && ok )
    {
      text::trim_right(line, " \t");

      if ( !line.empty() )
        ok = process_line( line, syntax, current_section );
    }

  return ok;
} // configuration_file::open()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write the configuration in a stream.
 * \param os The stream to write in.
 * \param syntax Description of the file's syntax.
 */
void claw::configuration_file::save
( std::ostream& os, const syntax_description& syntax )
{
  if ( !m_noname_section.empty() )
    {
      save_section_content( m_noname_section, os, syntax );
      os << '\n';
    }

  file_content::const_iterator it;
  for ( it=m_sections.begin(); it!=m_sections.end(); ++it )
    {
      os << syntax.make_section_name(it->first) << '\n';
      save_section_content( it->second, os, syntax );
      os << '\n';
    }
} // configuration_file::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of a field.
 * \param section The name of the section in which is the field.
 * \param field The name of the field to get.
 */
const std::string& claw::configuration_file::operator()
  ( const std::string& section, const std::string& field ) const
{
  file_content::const_iterator sect = m_sections.find(section);

  if ( sect == m_sections.end() )
    return s_unknow_field_value;
  else
    {
      section_content::const_iterator fld = sect->second.find(field);

      if ( fld == sect->second.end() )
        return s_unknow_field_value;
      else
        return fld->second;
    }
} // configuration_file::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of a field.
 * \param field The name of the field to get.
 *
 * \remark The field is searched in the fields declared outside any section.
 */
const std::string&
claw::configuration_file::operator()( const std::string& field ) const
{
  section_content::const_iterator fld = m_noname_section.find(field);

  if ( fld == m_noname_section.end() )
    return s_unknow_field_value;
  else
    return fld->second;
} // configuration_file::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a field exists.
 * \param section The name of the section containing the field.
 * \param field The name of the field to test.
 */
bool claw::configuration_file::has_field
( const std::string& section, const std::string& field ) const
{
  return field_begin( section, field ) != field_end( section, field );
} // configuration_file::has_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a field exists.
 * \param field The name of the field to test.
 *
 * \remark The field is searched outside any section.
 */
bool claw::configuration_file::has_field( const std::string& field ) const
{
  return field_begin( field ) != field_end( field );
} // configuration_file::has_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of a field.
 * \param section The name of the section containing the field.
 * \param field The name of the field to set.
 * \param val The value.
 *
 * This method removes all previous values for the given field.
 */
void claw::configuration_file::set_value
( const std::string& section, const std::string& field, const std::string& val )
{
  file_content::iterator it = m_sections.find(section);

  if ( it!=m_sections.end() )
    it->second.erase(field);

  add_value(section, field, val);
} // configuration_file::set_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of a field.
 * \param field The name of the field to Set.
 * \param val The value.
 *
 * This method removes all previous values for the given field.
 *
 * \remark The value is inserted in the fields declared outside any section.
 */
void claw::configuration_file::set_value
( const std::string& field, const std::string& val )
{
  m_noname_section.erase(field);
  add_value(field, val);
} // configuration_file::set_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a value to a field.
 * \param section The name of the section containing the field.
 * \param field The name of the field to add.
 * \param val The value.
 *
 * This method keeps all previous values for the given field.
 */
void claw::configuration_file::add_value
( const std::string& section, const std::string& field, const std::string& val )
{
  m_sections[section].insert( section_content::value_type(field, val) );
} // configuration_file::add_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of a field.
 * \param field The name of the field to Set.
 * \param val The value.
 *
 * This method keeps all previous values for the given field.
 */
void claw::configuration_file::add_value
( const std::string& field, const std::string& val )
{
  m_noname_section.insert( section_content::value_type(field, val) );
} // configuration_file::add_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a section and its fields.
 * \param section The name of the section to remove.
 */
void claw::configuration_file::clear_section( const std::string& section )
{
  m_sections.erase(section);
} // configuration_file::clear_section()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first value set for a field.
 * \param section The name of the section in which is the field.
 * \param field The name of the field to get.
 */
claw::configuration_file::const_field_iterator
claw::configuration_file::field_begin
( const std::string& section, const std::string& field ) const
{
  file_content::const_iterator it = m_sections.find(section);

  if (it == m_sections.end())
    return const_field_iterator();
  else
    return const_field_iterator( it->second.lower_bound(field) );
} // configuration_file::field_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the last value set for a field.
 * \param section The name of the section in which is the field.
 * \param field The name of the field to get.
 */
claw::configuration_file::const_field_iterator
claw::configuration_file::field_end
( const std::string& section, const std::string& field ) const
{
  file_content::const_iterator it = m_sections.find(section);

  if (it == m_sections.end())
    return const_field_iterator();
  else
    return const_field_iterator( it->second.upper_bound(field) );
} // configuration_file::field_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first value set for a field.
 * \param field The name of the field to get.
 *
 * \remark The field is searched in the fields declared outside any section.
 */
claw::configuration_file::const_field_iterator
claw::configuration_file::field_begin( const std::string& field ) const
{
  return const_field_iterator( m_noname_section.lower_bound(field) );
} // configuration_file::field_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the last value set for a field.
 * \param field The name of the field to get.
 *
 * \remark The field is searched in the fields declared outside any section.
 */
claw::configuration_file::const_field_iterator
claw::configuration_file::field_end( const std::string& field ) const
{
  return const_field_iterator( m_noname_section.upper_bound(field) );
} // configuration_file::field_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the field names of a section.
 *
 * \remark The names are searched in the fields declared outside any section.
 */
claw::configuration_file::const_section_iterator
claw::configuration_file::section_begin() const
{
  return const_section_iterator( m_noname_section.begin() );
} // configuration_file::section_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the last field name of a section.
 *
 * \remark The names are searched in the fields declared outside any section.
 */
claw::configuration_file::const_section_iterator
claw::configuration_file::section_end() const
{
  return const_section_iterator( m_noname_section.end() );
} // configuration_file::section_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the field names of a section.
 * \param section The name of the section in which the fields are searched.
 */
claw::configuration_file::const_section_iterator
claw::configuration_file::section_begin( const std::string& section ) const
{
  file_content::const_iterator it = m_sections.find(section);

  if (it == m_sections.end())
    return const_section_iterator();
  else
    return const_section_iterator( it->second.begin() );
} // configuration_file::section_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the last field name of a section.
 * \param section The name of the section in which the fields are searched.
 */
claw::configuration_file::const_section_iterator
claw::configuration_file::section_end( const std::string& section ) const
{
  file_content::const_iterator it = m_sections.find(section);

  if (it == m_sections.end())
    return const_section_iterator();
  else
    return const_section_iterator( it->second.end() );
} // configuration_file::section_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first named section.
 */
claw::configuration_file::const_file_iterator
claw::configuration_file::file_begin() const
{
  return const_file_iterator( m_sections.begin() );
} // configuration_file::file_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator just past the last named section.
 */
claw::configuration_file::const_file_iterator
claw::configuration_file::file_end() const
{
  return const_file_iterator( m_sections.end() );
} // configuration_file::file_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a line in the stream.
 * \param is The stream to read the line from.
 * \param syntax Description of the file's syntax.
 * \param line (out) The read line.
 */
bool claw::configuration_file::get_line
( std::istream& is, const syntax_description& syntax, std::string& line ) const
{
  bool result = text::getline(is, line);

  if ( result )
    {
      text::trim_left(line, " \t");
      escape_line(is, syntax, line);
    }

  return result;
} // configuration_file::get_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a section or field with the content of a line.
 * \param line The line to process.
 * \param syntax Description of the file's syntax.
 * \param section The section we are filling.
 */
bool claw::configuration_file::process_line
( const std::string& line, const syntax_description& syntax,
  section_content_ptr& section )
{
  CLAW_PRECOND( !line.empty() );

  bool result = true;

  if ( (line.size() >= 2)
       && (line[0] == syntax.section_name.first)
       && ( *(--line.end()) == syntax.section_name.second) )
    {
      std::string section_name( line.substr(1, line.length()-2) );
      text::trim( section_name, " \t" );
      section = &m_sections[section_name];
    }
  else
    {
      std::string::size_type pos = line.find_first_of(syntax.assignment);

      if (pos != std::string::npos)
        {
          std::string field( line.substr(0, pos) );
          std::string value;

          if ( (pos+1) != line.length() )
            {
              value = ( line.substr(pos+1) );
              text::trim(value, " \t");
            }

          text::trim(field, " \t");
          section->insert( section_content::value_type(field, value) );
        }
      else
        result = false;
    }

  return result;
} // configuration_file::process_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert escaped symbols from a line.
 * \param is The stream to read the line from.
 * \param syntax Description of the file's syntax.
 * \param line (out) The read line.
 */
void claw::configuration_file::escape_line
( std::istream& is, const syntax_description& syntax, std::string& line ) const
{
  std::string input_line(line);
  std::string::iterator it, last;
  bool stop = false;

  line.clear();
  last = input_line.begin();

  for (it = last; (it!=input_line.end()) && !stop; )
    if (*it == syntax.comment)
      stop = true;
    else if (*it == '\\')
      {
        line += std::string(last, it);
        ++it;

        if ( it == input_line.end() )
          {
            std::string remaining;
            get_line(is, syntax, remaining);
            line += remaining;
          }
        else
          {
            escape_char(*it, syntax, line);
            ++it;
          }

        last = it;
      }
    else
      ++it;

  line += std::string(last, it);
} // configuration_file::escape_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert an escaped character and append it to a string.
 * \param escaped The character that have been escaped.
 * \param syntax Description of the file's syntax.
 * \param str (out) The string in which we add the symbol.
 */
void claw::configuration_file::escape_char
( char escaped, const syntax_description& syntax, std::string& str ) const
{
  switch (escaped)
    {
    case '\'' : str += "\'"; break;
    case '\"' : str += "\""; break;
    case '\\' : str += "\\"; break;
    case 'a' : str += "\a"; break;
    case 'b' : str += "\b"; break;
    case 'f' : str += "\f"; break;
    case 'n' : str += "\n"; break;
    case 'r' : str += "\r"; break;
    case 't' : str += "\t"; break;
    case 'v' : str += "\v"; break;
    default :
      if ( escaped == syntax.comment )
        str += syntax.comment;
      else
        (str += "\\") += escaped;
    }
} // configuration_file::escape_char()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write the content of a section in a stream.
 * \param os The stream to write in.
 * \param c The content to write.
 * \param syntax Description of the file's syntax.
 */
void claw::configuration_file::save_section_content
( const section_content& c, std::ostream& os,
  const syntax_description& syntax ) const
{
  section_content::const_iterator it;

  for (it=c.begin(); it!=c.end(); ++it)
    os << syntax.make_assignment(it->first, it->second) << '\n';
} // configuration_file::save_section_content()
