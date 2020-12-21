#include "../utils/clon.hpp"
#include <iostream>

namespace std
{
  constexpr char lf = '\n';
}

int main()
try
{
  using namespace black::clon;
  std::string s =
    R"((log   
        (coucou 
          (status 
            (prout "coucou") 
            (lol "coucou")
            (toto true)
            (toto 12) 
            (titi false)
          )
        )
      ))";

  clon c = parse(s.begin(), s.end());
  
  const string& st = get_string("coucou.status.prout", c);
  std::cout << st << std::lf;
  std::cout << get_number("coucou.status.toto:1", c) << std::lf;
  set("coucou.status.toto:1", c, "coucou2");
  std::cout << get_string("coucou.status.toto:1", c) << std::lf;
  std::cout << "" << std::lf;
  
  return EXIT_SUCCESS;
}
catch (const std::exception& e)
{
  std::cout << "error " << e.what() << std::endl;
}


