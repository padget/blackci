#include "../utils/clon.hpp"
#include <iostream>

namespace std
{
  constexpr char newline = '\n';
}

int main()
try 
{
  std::string s =
    R"((log   
        (coucou 
          (status 
            (prout "coucou") 
            (lol "coucou")
            (toto true)
            (tata 12) 
            (titi false)
          )
        )
      ))";

  black::clon::clon c = black::clon::parse(s.begin(), s.end());
  
  std::cout << black::clon::to_string(black::clon::get("coucou.status:0", c));
  
  return EXIT_SUCCESS;
}
catch (const std::exception& e)
{
  std::cout << "error " << e.what() << std::endl;
}


