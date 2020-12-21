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
            (tata 12) 
            (titi false)
          )
        )
      ))";

  const clon& c = parse(s.begin(), s.end());
  const clon& status = get("coucou.status.prout", c);
  std::cout << (int)status.type << std::lf;
  std::cout << to_string(status) << std::lf;
  std::cout << cast_string(status) << std::lf;
  std::cout << get_value<string>("coucou.status.prout", c) << std::lf;
  return EXIT_SUCCESS;
}
catch (const std::exception& e)
{
  std::cout << "error " << e.what() << std::endl;
}


