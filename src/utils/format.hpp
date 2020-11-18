#ifndef __black_format_hpp__
#define __black_format_hpp__

#include <boost/format.hpp>

namespace black
{
  template <typename... args_t>
  std::string format(const char *fmt, args_t &&... arg)
  {
    return (boost::format(fmt) % ... % arg).str();
  }
} // namespace black

#endif
