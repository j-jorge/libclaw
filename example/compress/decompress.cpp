#include <cassert>
#include <claw/bit_istream.hpp>
#include <claw/buffered_istream.hpp>
#include <claw/buffered_ostream.hpp>
#include <claw/lzw_decoder.hpp>
#include <iostream>
#include <limits>

class input_buffer
{
public:
  input_buffer(std::istream& is)
    : val(0)
    , buffer(is)
    , stream(buffer)
    , m_code_size(9)
    , m_code_limit(1 << m_code_size)
  {}

  bool end_of_data() const
  {
    return !stream || (val == 256);
  }

  unsigned int symbols_count() const
  {
    return (unsigned int)std::numeric_limits<unsigned char>::max() + 2;
  }

  unsigned int get_next()
  {
    val = 0;
    stream.read(reinterpret_cast<char*>(&val), m_code_size);
    return val;
  }

  void reset()
  {
    val = 0;
    m_code_size = 9;
    m_code_limit = 1 << m_code_size;
  }

  void new_code(unsigned int code)
  {
    if((code == m_code_limit) && (m_code_size != 12))
      {
        ++m_code_size;
        m_code_limit = 1 << m_code_size;
      }
  }

private:
  unsigned int val;
  claw::buffered_istream<std::istream> buffer;
  claw::bit_istream<claw::buffered_istream<std::istream> > stream;

  unsigned int m_code_size;
  unsigned int m_code_limit;
};

class output_buffer
{
public:
  output_buffer(std::ostream& os, unsigned int max)
    : stream(os)
    , m_max_code(max)
  {}

  unsigned int max_code() const
  {
    return m_max_code;
  }

  void write(unsigned int code)
  {
    assert(code < 256);
    const char v(code);
    stream.write(v);
  }

private:
  claw::buffered_ostream<std::ostream> stream;
  const unsigned int m_max_code;
};

int main()
{
  claw::lzw_decoder<input_buffer, output_buffer> decoder;
  input_buffer input(std::cin);
  output_buffer output(std::cout, 4096);

  while(std::cin)
    {
      input.reset();
      decoder.decode(input, output);
    }

  return 0;
}
