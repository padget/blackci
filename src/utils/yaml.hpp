#ifndef __black_yaml_hpp__
#define __black_yaml_hpp__

#include <yaml-cpp/yaml.h>
#include <string>

namespace black::yaml
{
  inline bool has(
      const YAML::Node n,
      const std::string &key)
  {
    return static_cast<bool>(n[key]);
  }

  template <typename type_t>
  type_t as(
      const YAML::Node &n,
      const std::string &key)
  {
    return n[key].as<type_t>();
  }

  inline const std::list<std::string>
  as_strs(
      const YAML::Node &n,
      const std::string &k)
  {
    return as<std::list<std::string>>(n, k);
  }

  inline const std::string
  as_str(
      const YAML::Node &n,
      const std::string &k)
  {
    return as<std::string>(n, k);
  }
} // namespace black::yaml

#endif