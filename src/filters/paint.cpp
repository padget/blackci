#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <exception>
#include <filesystem>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include "../utils/json.hpp"
#include "../utils/json_utils.hpp"
#include "fmt_to_style.hpp"
#include <fmt/format.h>

namespace json = nlohmann;

struct display
{
  std::string pattern;
  std::string path;
};

struct format
{
  std::string path;
  std::string value;
  fmt::text_style style;
  std::vector<display> displays;
};

struct filter
{
  std::string name;
  std::vector<format> formats;
};

struct configuration
{
  std::string path;
  std::vector<filter> filters;
};

json::json load_config_json(const std::string &);
configuration load_config(const json::json &);

fmt::text_style
to_style(const std::vector<std::string> &styles);

std::string
get_config_path(int argc, char **argv);

std::string
get_filter_name(int argc, char **argv);

const filter &
load_filter(
    const std::string &name,
    const std::vector<filter> &filters);

void apply_filter(
    const json::json &j,
    const filter &f);

class bad_configuration : public std::exception
{
  std::string msg;

public:
  explicit bad_configuration(const std::string &_msg) : msg{_msg} {}
  const char *what() const noexcept
  {
    return msg.c_str();
  }
};

void check_nb_args(int argc, int nb)
{
  if (argc != nb)
    throw std::invalid_argument(fmt::format("{} args expected", nb));
}

int main(int argc, char **argv)
{
  try
  {
    check_nb_args(argc, 3);
    auto path = get_config_path(argc, argv);
    auto fname = get_filter_name(argc, argv);
    auto json = load_config_json(path);
    auto config = load_config(json);
    auto filter = load_filter(fname, config.filters);

    std::string line;
    json::json j;

    while (std::cin >> j)
      apply_filter(j, filter);

    return EXIT_SUCCESS;
  }
  catch (std::exception &e)
  {
    auto &&err = black::json::from_error(e);
    auto &&style = fmt::fg(fmt::color::red) |
                   fmt::emphasis::bold;
    auto &&pattern = "{}\n";
    std::cout << fmt::format(style, pattern, err.dump());
  }

  return EXIT_FAILURE;
}

json::json load_config_json(const std::string &fname)
{
  json::json config;

  if (std::filesystem::exists(fname))
  {
    std::ifstream inp(fname);
    inp >> config;
  }

  return config;
}

configuration load_config(const json::json &j)
{
  configuration config;

  for (auto &&jflt : j)
  {
    filter flt;
    flt.name = jflt["name"].get<std::string>();

    for (auto &&jfmt : jflt["formats"])
    {
      format fmt;
      fmt.path = jfmt["path"].get<std::string>();
      fmt.value = jfmt["value"].get<std::string>();
      fmt.style = to_style(jfmt["style"].get<std::vector<std::string>>());

      if (jfmt.contains("displays"))
        for (auto &&jdis : jfmt["displays"])
        {
          display dis;
          dis.pattern = jdis["pattern"].get<std::string>();
          dis.path = jdis["path"].get<std::string>();
          fmt.displays.push_back(dis);
        }

      flt.formats.push_back(fmt);
    }

    config.filters.push_back(flt);
  }

  return config;
}

auto is_char(const char c)
{
  return [=](const char o) {
    return c == o;
  };
}

std::list<std::string>
split(const std::string &s,
      const char c)
{
  using namespace boost::algorithm;
  using namespace boost;
  std::list<std::string> res;
  split(res, s, is_char(c), token_compress_on);
  return res;
}

std::string
get_config_path(int argc, char **argv)
{
  namespace ba = boost::algorithm;
  std::string arg(argv[1]);
  std::string key("--config=");
  std::string path;

  if (ba::starts_with(arg, key))
  {
    std::list<std::string> keyval = split(arg, '=');

    if (keyval.size() == 2)
      path = keyval.back();
    else
    {
      auto &&e = "--config arg malformed";
      throw std::invalid_argument(e);
    }
  }
  else
  {
    auto &&e = "--config=<path> is mandatory";
    throw std::invalid_argument(e);
  }

  return path;
}

std::string
get_filter_name(int argc, char **argv)
{
  namespace ba = boost::algorithm;
  std::string arg(argv[2]);
  std::string key("--filter=");
  std::string name;

  if (ba::starts_with(arg, key))
  {
    std::list<std::string> keyval = split(arg, '=');

    if (keyval.size() == 2)
      name = keyval.back();
    else
    {
      auto &&e = "--filter arg malformed";
      throw std::invalid_argument(e);
    }
  }
  else
  {
    auto &&e = "--filter=<path> is mandatory";
    throw std::invalid_argument(e);
  }

  return name;
}

const filter &
load_filter(
    const std::string &name,
    const std::vector<filter> &filters)
{
  for (auto &&flt : filters)
    if (flt.name == name)
      return flt;

  auto &&p = "filter '{}' does not exist";
  auto &&e = fmt::format(p, name);
  throw std::invalid_argument(e);
}

void apply_filter(
    const json::json &j,
    const filter &f)
{
  using namespace black::json;
  for (auto &&fmt : f.formats)
    if (exists(fmt.path, j))
      if (get_str(fmt.path, j) == fmt.value)
      {
        if (fmt.displays.empty())
          std::cout << fmt::format(fmt.style, "{}\n", j.dump());
        else
          for (auto &&dis : fmt.displays)
          {
            std::cout << fmt::format(fmt.style, dis.pattern, get_str(dis.path, j));
            std::cout << '\n';
          }
        return;
      }

  std::cout << fmt::format("{}\n", j.dump());
}
