#ifndef __black_console_hpp__
#define __black_console_hpp__

#include <iostream>

namespace black::console
{
  enum class color : int
  {
    black = 30,
    red = 31,
    green = 32,
    yellow = 33,
    blue = 34,
    magenta = 35,
    cyan = 36,
    white = 37
  };

  template <typename arg_t>
  void print(color c, const arg_t &arg)
  {
    std::cout << "\033[1;" << std::to_string((int)c) << "m";
    std::cout << arg;
    std::cout << "\033[0m\n";
  }

  template <typename arg_t>
  void red(const arg_t &arg)
  {
    print(color::red, arg);
  }

  template <typename arg_t>
  void yellow(const arg_t &arg)
  {
    print(color::yellow, arg);
  }

  template <typename arg_t>
  void green(const arg_t &arg)
  {
    print(color::green, arg);
  }

  template <typename arg_t>
  void blue(const arg_t &arg)
  {
    print(color::blue, arg);
  }

} // namespace black::console

#endif
