#ifndef __json_utils_hpp__
#define __json_utils_hpp__

#include "clon.hpp"

namespace black::clon
{

  inline clon from_error(
    const std::exception& exc)
  {
    return parse_fmt(
      R"((log 
          (type "exception") 
          (level "fatal")
          (message "{}")))",
      exc.what());
  }

  inline clon log_info(
    const std::string& mess)
  {
    return parse_fmt(
      R"((log 
          (type "simple") 
          (level "info")
          (message "{}")))",
      mess);
  }

  inline clon report(
    const std::string& status)
  {
    return parse_fmt(
      R"((log 
          (type "report")
          (report 
            (status "{}"))))",
      status);
  }

  inline clon title(
    const std::string& val,
    const std::string& lvl)
  {
    return parse_fmt(
      R"((title 
          (level "{}")
          (value "{}")))",
      lvl, val);
  }

  inline clon question(
    const std::string& demand)
  {
    return parse_fmt(
      R"((question "{}"))", 
      demand);
  }
} // namespace black::json

#endif
