#include <iostream>
#include <sstream>
#include <math.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  if (argc != 6)
    {
      std::cerr << argv[0] << " n_sommets n_isoles n_arcs symboles poids_max" 
                << std::endl;
      return 1;
    }
  else
    {
      int n_sommets, n_isoles, n_arcs, poids_max;
      std::string symboles = argv[4];
      std::istringstream iss;

      srand(time(NULL));

      iss.str(argv[1]); iss >> n_sommets; iss.clear();
      iss.str(argv[2]); iss >> n_isoles; iss.clear();
      iss.str(argv[3]); iss >> n_arcs; iss.clear();
      iss.str(argv[5]); iss >> poids_max; iss.clear();

      for (int i=0, j=0; (i<n_sommets) || (j<n_arcs); ++j)
        {
          int s1 = rand() % n_sommets +1;
          int s2 = rand() % n_sommets +1;

          std::cout << s1 << " " << s2 << " " 
                    << symboles[rand() % symboles.size()] << " "
                    << (rand() % poids_max) << std::endl;

          if (s1==s2) i+=1; else i+=2;
        }

      for (int i=0; i!=n_isoles; ++i)
        std::cout << (n_sommets + rand() % n_isoles) << " 0" << std::endl;
    }
  return 0;
}
