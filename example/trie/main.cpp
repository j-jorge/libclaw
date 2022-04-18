#include "claw/trie.hpp"
#include <fstream>
#include <iostream>

int main()
{
  const std::string filename = "words.txt";
  std::ifstream text(filename.c_str());

  if(!text)
    std::cerr << "Can't open file '" << filename << "'" << std::endl;
  else
    {
      std::string word;
      claw::trie<char> dictionary;

      while(text >> word)
        dictionary.insert(word.begin(), word.end());

      text.close();

      std::cout << dictionary.size() << " words." << std::endl;

      if(dictionary.empty())
        std::cout << "Dictionary is empty but shouldn't !!\n" << std::endl;
      else
        std::cout << "Dictionary is not empty.\n" << std::endl;

      claw::trie<char> dictionary_copy(dictionary);

      dictionary.clear();

      if(dictionary.empty())
        std::cout << "Dictionary is empty.\n" << std::endl;
      else
        std::cout << "Dictionary is not empty but should !\n" << std::endl;

      std::cout << "Dictionary copy" << std::endl;

      std::cout << dictionary_copy.size() << " words.\n" << std::endl;
      if(dictionary_copy.empty())
        std::cout << "Dictionary is empty but shouldn't !!" << std::endl;
      else
        std::cout << "Dictionary is not empty" << std::endl;

      std::cout << "Type a word to get how many times it's in the text, ";
      std::cout << "quit' to quit" << std::endl;
      do
        {
          std::cin >> word;
          std::cout << word << " is present "
                    << dictionary_copy.count(word.begin(), word.end())
                    << " times." << std::endl;
        }
      while(word != "quit");
    }
  return 0;
}
