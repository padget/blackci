
#include "../utils/clon.hpp"
#include <iostream>

int main()
{
  /**
   * (log 
   *    (type "exception") 
   *    (level "fatal") n
   *    (message "on est sous l'eau"))
   *    (report 
   *      (status "ok")))
   */
  black::clon::clon c;
  c["log"]["type"] = "exception";
  c["log"]["level"] = "fatal";
  c["log"]["message"] = 2;
  c["log"]["report"]["status"] = "oops";
  black::clon::clon start;
  start["toto"] = "null";
  c["log"]["start"] = start;
  c["log"]["start"] = start;

  std::cout << c.to_string() << std::endl;

  return EXIT_SUCCESS;
}
