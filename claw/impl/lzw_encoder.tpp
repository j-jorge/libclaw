/**
 * \file lzw_encoder.tpp
 * \brief Implementation of the claw::lzw_encoder class.
 * \author Julien Jorge
 */
#include <map>

/*----------------------------------------------------------------------------*/
/**
 * \brief Encode a sequence of datas.
 * \param input Where we read the uncompressed data.
 * \param output Where we write compressed data.
 */
template<typename InputBuffer, typename OutputBuffer>
void claw::lzw_encoder<InputBuffer, OutputBuffer>::encode
( input_buffer_type& input, output_buffer_type& output ) const
{
  typedef std::pair<unsigned int, unsigned int> word;

  if ( !input.end_of_data() )
    {
      std::map<word, unsigned int> table;
      
      unsigned int symbol = input.get_next();
      unsigned int prefix_code = symbol;
      unsigned int next_code = input.symbols_count();

      while ( !input.end_of_data() && (next_code != output.max_code()) )
        {
          symbol = input.get_next();

          word new_word(prefix_code, symbol);

          if ( table.find(new_word) != table.end() )
            prefix_code = table[new_word];
          else
            {
              output.write(prefix_code);
              output.new_code(next_code);
              table[new_word] = next_code;
              prefix_code = symbol;

              ++next_code;
            }
        }

      output.write(prefix_code);
    }
} // lzw_encoder::encode()
