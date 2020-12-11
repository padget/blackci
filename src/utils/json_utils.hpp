#ifndef __json_utils_hpp__
#define __json_utils_hpp__

#include "json.hpp"

namespace black::json
{

  template <typename excp_t>
  nlohmann::json from_error(const excp_t &exc)
  {
    nlohmann::json err;
    err["log"]["type"] = "exception";
    err["log"]["level"] = "fatal";
    err["log"]["message"] = exc.what();
    return err;
  }

  inline nlohmann::json
  log_info(
      const std::string &mess)
  {
    nlohmann::json log;
    log["log"]["type"] = "simple";
    log["log"]["level"] = "info";
    log["log"]["message"] = mess;
    return log;
  }

  inline nlohmann::json
  report(
      const std::string &status)
  {
    nlohmann::json report;
    report["log"]["type"] = "report";
    report["log"]["report"]["status"] = status;
    return report;
  }

  inline nlohmann::json
  title(
      const std::string &val,
      const std::string &lvl)
  {
    nlohmann::json jt;
    jt["title"]["level"] = lvl;
    jt["title"]["value"] = val;
    return jt;
  }

  inline nlohmann::json
  question(const std::string &demand)
  {
    nlohmann::json jq;
    jq["question"] = demand;
    return jq;
  }

  inline bool
  exists(
      const std::string &path,
      const nlohmann::json &json)
  {
    auto pos = path.find('.');

    if (pos == std::string::npos)
      return json.contains(path);
    else if (json.contains(path.substr(0, pos)))
      return exists(
          path.substr(pos + 1),
          json.at(path.substr(0, pos)));
    else
      return false;
  }

  const nlohmann::json &
  get(
      const std::string &path,
      const nlohmann::json &json)
  {
    auto pos = path.find('.');

    if (pos == std::string::npos)
      return json.at(path);
    else
      return get(
          path.substr(pos + 1),
          json.at(path.substr(0, pos)));
  }

  std::string
  get_str(const std::string &path,
          const nlohmann::json &j)
  {
    if (exists(path, j))
    {
      nlohmann::json v = get(path, j);

      if (v.is_string())
        return v.get<std::string>();
    }

    return "";
  }
} // namespace black::json

#endif
