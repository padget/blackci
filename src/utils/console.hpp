#ifndef __black_console_hpp__
#define __black_console_hpp__

#include <iostream>
#include <fmt/color.h>

namespace black::console
{

  template <typename... args_t>
  void print(const std::string &format, const args_t &... args)
  {
    fmt::print(fmt::fg(fmt::color::deep_sky_blue) | fmt::emphasis::bold, format, args...);
  }
} // namespace black::console

#endif
