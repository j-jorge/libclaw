/**
 * \file main.cpp
 * \brief Example program to show the use of the the claw::avl class.
 *
 * The program take avl values from a file and do some operations.
 *
 * \author Julien Jorge
 */
#include <fstream>
#include <iostream>

#include <claw/avl.hpp>

/**
 * \brief This class allows us to test the claw::avl class with a non- type.
 */
class some_class
{
public:
  int value;

public:
  bool operator==(const some_class& that) const
  {
    return value == that.value;
  }
  bool operator<(const some_class& that) const
  {
    return value < that.value;
  }

  friend std::ostream& operator<<(std::ostream& os, const some_class& o)
  {
    return os << o.value;
  }
}; // class some_class

void instanciate_methods(const claw::avl<some_class>& tree)
{
  claw::avl<some_class> cpy(tree.begin(), tree.end());

  std::cout << (tree == cpy) << std::endl;
  std::cout << (tree != cpy) << std::endl;
  std::cout << (tree <= cpy) << std::endl;
  std::cout << (tree >= cpy) << std::endl;
  std::cout << (tree < cpy) << std::endl;
  std::cout << (tree > cpy) << std::endl;

  claw::avl<some_class>::const_iterator it;

  for(it = tree.begin(); it != tree.end(); ++it)
    std::cout << *it << ' ';

  std::cout << std::endl;

  for(it = cpy.begin(); it != cpy.end(); ++it)
    std::cout << *it << ' ';

  std::cout << std::endl;

  it = tree.find_nearest_greater(*tree.lower_bound());
  std::cout << *it << std::endl;

  it = tree.find_nearest_lower(*tree.upper_bound());
  std::cout << *it << std::endl;

  cpy.insert(tree.begin(), tree.end());
}

int main(int argc, char* argv[])
{
  if(argc != 2)
    {
      std::cout << argv[0] << " test_file" << std::endl;
      return 1;
    }
  else
    {
      claw::avl<some_class> tree;
      claw::avl<some_class>::const_iterator it;

      std::ifstream f(argv[1]);

      if(!f)
        {
          std::cerr << "Can't open file " << argv[1] << std::endl;
          return 1;
        }
      else
        {
          int i, j;
          some_class o;

          if(tree.empty())
            std::cout << "Tree is empty" << std::endl;
          else
            std::cout << "Tree is not empty but should !" << std::endl;

          std::cout << "File loading..." << std::endl;

          while(f >> o.value)
            tree.insert(o);

          f.close();

          std::cout << "size : " << tree.size() << std::endl;

          instanciate_methods(tree);

          it = tree.lower_bound();
          if(it != tree.end())
            std::cout << "Min = " << *it << std::endl;

          it = tree.upper_bound();
          if(it != tree.end())
            std::cout << "Max = " << *it << std::endl;

          if(tree.empty())
            std::cout << "Tree is empty but shouldn't !" << std::endl;
          else
            std::cout << "Tree's not empty" << std::endl;

          std::cout << "Found :";
          for(o.value = 0; o.value != 100; ++o.value)
            if(tree.find(o) != tree.end())
              std::cout << " " << o;

          std::cout << std::endl;
          ;

          std::cout << "Scan with an iterator :";

          for(it = tree.begin(); it != tree.end(); ++it)
            std::cout << " " << *it;

          std::cout << std::endl;
          std::cout << "Scan bis, iterator affectation :";

          for(it = tree.begin(); it != tree.end(); ++it)
            {
              claw::avl<some_class>::const_iterator it_bis = it;
              std::cout << *it << ":";

              for(; it_bis != tree.end(); ++it_bis)
                std::cout << " " << *it_bis;
              std::cout << std::endl;
            }

          std::cout << "Deletions" << std::endl;

          // Try to be a few random
          for(i = 0; i != 10; ++i)
            for(j = 0; j != 100; ++j)
              {
                o.value = i + j * 10;
                tree.erase(o);
              }

          std::cout << "Copy and scan" << std::endl;
          std::cout << "source size : " << tree.size() << std::endl;

          claw::avl<some_class> avl_2(tree);

          std::cout << "copy size : " << tree.size() << std::endl;

          if(tree.empty())
            std::cout << "Tree's copy is empty" << std::endl;
          else
            std::cout << "Tree's copy is not empty" << std::endl;

          for(it = avl_2.begin(); it != avl_2.end(); ++it)
            std::cout << " " << *it;

          std::cout << std::endl;

          std::cout << "Reverse scan" << std::endl;

          it = avl_2.end();
          while(it != avl_2.begin())
            {
              --it;
              std::cout << " " << *it;
            }
          std::cout << std::endl;

          std::cout << "Assignment" << std::endl;
          avl_2 = tree;
          std::cout << "Clear" << std::endl;
          avl_2.clear();

          if(avl_2.empty())
            std::cout << "Tree's is empty" << std::endl;
          else
            std::cout << "Tree's is not empty but should !" << std::endl;

          return 0;
        }
    }
}
