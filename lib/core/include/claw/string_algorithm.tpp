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
 * \file string_algorithm.tpp
 * \brief Implementation of the algorithms on strings.
 * \author Julien Jorge
 */

#include <claw/algorithm.hpp>
#include <claw/glob.hpp>

#include <iterator>
#include <sstream>
#include <string>

/**
 * \brief A portable version of std::getline( is, str, '\\n' ) that removes a
 *        tailing '\\r'.
 * \param is The stream in which we read.
 * \param str The line read from the stream.
 */
template <typename StreamType, typename StringType>
StreamType& claw::text::getline(StreamType& is, StringType& str)
{
  std::getline(is, str);

  if(!str.empty())
    if(str[str.size() - 1] == typename StringType::value_type('\r'))
      str.erase(str.size() - 1);

  return is;
}

/**
 * \brief Remove characters at the begining of a string.
 * \param str The string to modify.
 * \param s The characters to remove.
 */
template <typename StringType>
void claw::text::trim_left(StringType& str,
                           const typename StringType::value_type* const s)
{
  typename StringType::size_type p = str.find_first_not_of(s);

  if(p != StringType::npos)
    str = str.substr(p);
}

/**
 * \brief Remove characters at the end of a string.
 * \param str The string to modify.
 * \param s The characters to remove.
 */
template <typename StringType>
void claw::text::trim_right(StringType& str,
                            const typename StringType::value_type* const s)
{
  typename StringType::size_type p = str.find_last_not_of(s);

  if(p != StringType::npos)
    str = str.substr(0, p + 1);
}

/**
 * \brief Remove characters at the begining end at the end of a string.
 * \param str The string to modify.
 * \param s The characters to remove.
 */
template <typename StringType>
void claw::text::trim(StringType& str,
                      const typename StringType::value_type* const s)
{
  typename StringType::size_type first = str.find_first_not_of(s);
  typename StringType::size_type last = str.find_last_not_of(s);

  if(first != StringType::npos)
    str = str.substr(first, last - first + 1);
}

/**
 * \brief Squeeze successive characters of a string into one character.
 * \param str The string to modify.
 * \param s The characters to remove.
 *
 * \b Example :
 * <tt>
 * std::string s("word aaa bbb abab");
 * claw::squeeze( s, "ab" );
 * std::cout << s << std::end; // result is "word a b abab"
 * </tt>
 */
template <typename StringType>
void claw::text::squeeze(StringType& str,
                         const typename StringType::value_type* const s)
{
  typedef typename StringType::size_type size_type;

  size_type first(0);

  do
    {
      first = str.find_first_of(s, first);

      if(first != StringType::npos)
        {
          size_type last = str.find_first_not_of(str[first], first + 1);

          if(last == StringType::npos)
            str = str.substr(0, first + 1);
          else if(last - first > 1)
            str = str.substr(0, first + 1) + str.substr(last);

          ++first;
        }
    }
  while((first != StringType::npos) && (first != str.length()));
}

/**
 * \brief Replace a set of characters by other characters.
 * \param str The string to modify.
 * \param e1 The characters to remove.
 * \param e2 The characters replacing the ones in \a e1.
 *
 * \return The number of replaced characters.
 *
 * Each character e1[i] will be replaced with e2[i]. If \a e1 is smaller than
 * \a e2, the latter will be completed by repeating its last character.
 *
 * \b Example :
 * <tt>
 * std::string s("word aaa bbb abab");
 * claw::replace( s, "ab", "ba" );
 * std::cout << s << std::end; // result is "word bbb aaa baba"
 * </tt>
 */
template <typename StringType>
std::size_t claw::text::replace(StringType& str, const StringType& e1,
                                const StringType& e2)
{
  return claw::replace(str.begin(), str.end(), e1.begin(), e1.end(),
                       e2.begin(), e2.end());
}

/**
 * \brief Test if the content of a string is immediately convertible to a type.
 * \param str The string to test.
 */
template <typename T, typename StringType>
bool claw::text::is_of_type(const StringType& str)
{
  std::basic_istringstream<typename StringType::value_type,
                           typename StringType::traits_type,
                           typename StringType::allocator_type>
      iss(str);

  T val;
  bool result = false;

  if(iss >> val)
    result = iss.eof();

  return result;
}

/**
 * \brief Split a string into several substrings, according to a given
 *        separator.
 * \param sequence A sequence in which the substrings are added.
 * \param str The string to split.
 * \param sep The separator on which the string is splitted.
 */
template <typename Sequence>
void claw::text::split(Sequence& sequence,
                       const typename Sequence::value_type& str,
                       const typename Sequence::value_type::value_type sep)
{
  split(sequence, str.begin(), str.end(), sep);
}

/**
 * \brief Split a string into several substrings, according to a given
 *        separator.
 * \param sequence A sequence in which the substrings are added.
 * \param first Iterator on the beginning of the string to split.
 * \param last Iterator just past the end of the string to split.
 * \param sep The separator on which the string is splitted.
 */
template <typename Sequence>
void claw::text::split(Sequence& sequence,
                       typename Sequence::value_type::const_iterator first,
                       typename Sequence::value_type::const_iterator last,
                       const typename Sequence::value_type::value_type sep)
{
  typedef typename Sequence::value_type string_type;

  string_type line;
  std::basic_istringstream<typename string_type::value_type,
                           typename string_type::traits_type,
                           typename string_type::allocator_type>
      iss(string_type(first, last));

  while(std::getline(iss, line, sep))
    *std::insert_iterator<Sequence>(sequence, sequence.end()) = line;
}

/**
 * \brief Find escaped symbols in a sequence of characters and replace them by
 *        their c-equivalent.
 *
 * \param first Iterator on the beginning of the string to escape.
 * \param last Iterator just past the end of the string to escape.
 * \param out Iterator on the beginning of the output string.
 * \pre \a out points on a range long enough to store the resulting string.
 *
 * \code
 * std::string s("\\a\\t\\n\\r");
 * std::string r;
 *
 * claw::text::c_escape( s.begin(), s.end(), std::insert_iterator(r, r.end())
 * );
 *
 * if ( r == "\a\t\n\r" )
 *   std::cout << "It works!" << std::endl;
 * \endcode
 *
 * \remark This method has not been tested with wide chars yet.
 */
template <typename InputIterator, typename OutputIterator>
void claw::text::c_escape(InputIterator first, InputIterator last,
                          OutputIterator out)
{
  typedef typename std::iterator_traits<InputIterator>::value_type char_type;
  typedef std::basic_string<char_type> string_type;

  const string_type oct("01234567");
  const string_type hex("0123456789ABCDEFabcdef");

  bool escape(false);

  for(; first != last; ++out)
    if(escape)
      {
        switch(*first)
          {
          case 'a':
            *out = '\a';
            ++first;
            break;
          case 'b':
            *out = '\b';
            ++first;
            break;
          case 'f':
            *out = '\f';
            ++first;
            break;
          case 'n':
            *out = '\n';
            ++first;
            break;
          case 'r':
            *out = '\r';
            ++first;
            break;
          case 't':
            *out = '\t';
            ++first;
            break;
          case 'v':
            *out = '\v';
            ++first;
            break;
          case 'o':
            {
              ++first;
              int v(0);
              const InputIterator e(
                  find_first_not_of(first, last, oct.begin(), oct.end()));

              std::basic_istringstream<char_type> iss(string_type(first, e));
              iss >> std::oct >> v;
              *out = (char_type)v;
              first = e;
              break;
            }
          case 'x':
            {
              ++first;
              int v(0);
              const InputIterator e(
                  find_first_not_of(first, last, hex.begin(), hex.end()));

              std::basic_istringstream<char_type> iss(string_type(first, e));
              iss >> std::hex >> v;
              *out = (char_type)v;
              first = e;
              break;
            }
          default:
            *out = *first;
            ++first;
          }

        escape = false;
      }
    else if(*first == '\\')
      {
        escape = true;
        ++first;
      }
    else
      {
        *out = *first;
        ++first;
      }
}

/**
 * \brief Check if a string matches a given pattern.
 * \param pattern The pattern.
 * \param text The text to check.
 * \param any_sequence A value representing any sequence of values, empty or
 *        not.
 * \param zero_or_one A value representing any value or no value.
 * \param any A value representing any value.
 */
template <typename StringType>
bool claw::text::glob_match(const StringType& pattern, const StringType& text,
                            const typename StringType::value_type any_sequence,
                            const typename StringType::value_type zero_or_one,
                            const typename StringType::value_type any)
{
  return claw::glob_match(pattern.begin(), pattern.end(), text.begin(),
                          text.end(), any_sequence, zero_or_one, any);
}

/**
 * \brief Check if a string may match a given pattern.
 * \param pattern The pattern.
 * \param text The text to check.
 * \param any_sequence A value representing any sequence of values, empty or
 *        not.
 * \param zero_or_one A value representing any value or no value.
 * \param any A value representing any value.
 */
template <typename StringType>
bool claw::text::glob_potential_match(
    const StringType& pattern, const StringType& text,
    const typename StringType::value_type any_sequence,
    const typename StringType::value_type zero_or_one,
    const typename StringType::value_type any)
{
  return claw::glob_potential_match(pattern.begin(), pattern.end(),
                                    text.begin(), text.end(), any_sequence,
                                    zero_or_one, any);
}
