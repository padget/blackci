#ifndef __black_process_hpp__
#define __black_process_hpp__

#include <boost/process.hpp>
#include <sstream>

namespace black
{
  // template <typename... args_t>
  // int run_detached(
  //     const std::string &cmd,
  //     const args_t &... args)
  // {
  //   return boost::process::system(cmd, args...);
  // }

  auto out_to_stdout()
  {
    return boost::process::std_out > stdout;
  }

  auto err_to_stderr()
  {
    return boost::process::std_err > stderr;
  }

  auto in_to_stdin()
  {
    return boost::process::std_in < stdin;
  }

  template <typename... args_t>
  int run_attached(
      const std::string &cmd,
      const args_t &... args)
  {
    std::stringstream ss;
    ss << cmd;
    ((ss << ' ' << args), ...);
    return boost::process::system(
        ss.str(), out_to_stdout(),
        err_to_stderr(), in_to_stdin());
  }
} // namespace black

#endif
