#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <exception>
#include <filesystem>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include "fmt_to_style.hpp"
#include <fmt/format.h>
#include "../utils/clon.hpp"
#include "../utils/clon_utils.hpp"

using namespace black;



struct input
{
  std::string path;
  std::string value;
};

struct output
{
  fmt::text_style style;
  std::string pattern;
  std::string path;
};

struct filter
{
  std::string name;
  input in;
  output out;
};

struct configuration
{
  std::string path;
  std::vector<filter> filters;
};

clon::clon load_config_clon(const std::string&);
configuration load_config(const clon::clon&);

std::string
get_config_path(int argc, char** argv);

std::string
get_filter_name(int argc, char** argv);

const filter&
load_filter(
  const std::string& name,
  const std::vector<filter>& filters);

void apply_filter(
  const clon::clon& c,
  const filter& f);

class bad_configuration : public std::exception
{
  std::string msg;

public:
  explicit bad_configuration(const std::string& _msg) : msg{ _msg } {}
  const char* what() const noexcept
  {
    return msg.c_str();
  }
};

void check_nb_args(int argc, int nb)
{
  if (argc != nb)
    throw std::invalid_argument(fmt::format("{} args expected", nb));
}

int main(int argc, char** argv)
try
{
  check_nb_args(argc, 3);
  auto path = get_config_path(argc, argv);
  auto fname = get_filter_name(argc, argv);
  auto cclon = load_config_clon(path);
  auto config = load_config(cclon);
  
  std::cout << config.filters.size() << std::endl;
  
  auto filter = load_filter(fname, config.filters);

  
  std::string line;
  clon::clon c;

  while (std::getline(std::cin, line))
  {
    c = clon::parse(line);
    apply_filter(c, filter);
  }

  return EXIT_SUCCESS;
}
catch (std::exception& e)
{
  auto&& err = clon::from_error(e);
  auto&& style =
    fmt::fg(fmt::color::red) |
    fmt::emphasis::bold;
  auto&& pattern = "{}\n";
  std::cout << fmt::format(style, pattern, clon::to_string(err));
  return EXIT_FAILURE;
}


clon::clon load_config_clon(
  const std::string& fname)
{
  clon::clon config;

  if (std::filesystem::exists(fname))
  {
    std::ifstream inp(fname);
    config = clon::parse_stream(inp);
  }

  return config;
}

configuration load_config(const clon::clon& c)
{
  configuration config;

  if (clon::is_object(c) and c.name == "filters")
    for (auto&& cfilter : clon::as_object(c))
    {
      filter flt;

      if (cfilter.name == "filter")
      {
        auto&& cname = clon::get("name", cfilter);
        auto&& inpath = clon::get("input.path", cfilter);
        auto&& inval = clon::get("input.value", cfilter);
        auto&& outpath = clon::get("output.path", cfilter);
        auto&& outpattern = clon::get("output.pattern", cfilter);

        if (clon::is_string(cname))
          flt.name = clon::as_string(cname);

        if (clon::is_string(inpath))
          flt.in.path = clon::as_string(inpath);

        if (clon::is_string(inval))
          flt.in.value = clon::as_string(inval);

        if (clon::is_string(outpattern))
          flt.out.pattern = clon::as_string(outpattern);

        if (clon::is_string(outpath))
          flt.out.path = clon::as_string(outpath);

        auto&& cout = clon::get("output", cfilter);

        std::vector<std::string> styles;

        if (clon::is_object(cout))
          for (auto&& sty : clon::as_object(cout))
            if (sty.name == "style" and clon::is_string(sty))
              styles.push_back(clon::as_string(sty));

        flt.out.style = to_style(styles);
      }
      
      config.filters.emplace_back(flt);
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
split(const std::string& s,
  const char c)
{
  using namespace boost::algorithm;
  using namespace boost;
  std::list<std::string> res;
  split(res, s, is_char(c), token_compress_on);
  return res;
}

std::string
get_config_path(int argc, char** argv)
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
      auto&& e = "--config arg malformed";
      throw std::invalid_argument(e);
    }
  }
  else
  {
    auto&& e = "--config=<path> is mandatory";
    throw std::invalid_argument(e);
  }

  return path;
}

std::string
get_filter_name(int argc, char** argv)
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
      auto&& e = "--filter arg malformed";
      throw std::invalid_argument(e);
    }
  }
  else
  {
    auto&& e = "--filter=<path> is mandatory";
    throw std::invalid_argument(e);
  }

  return name;
}

const filter&
load_filter(
  const std::string& name,
  const std::vector<filter>& filters)
{
  for (auto&& flt : filters)
    if (flt.name == name)
      return flt;

  auto&& p = "filter '{}' does not exist";
  auto&& e = fmt::format(p, name);
  throw std::invalid_argument(e);
}

void apply_filter(
  const clon::clon& c,
  const filter& f)
{
  auto&& indetect = clon::get(f.in.path, c);

  if (clon::is_string(indetect))
    if (clon::as_string(indetect) == f.in.value)
    {
      std::string pattern = "{}\n";
      std::string value;

      auto&& cout = clon::get(f.out.path, c);

      if (clon::is_string(cout))
        value = std::move(clon::as_string(cout));
      else
        value = std::move(clon::to_string(cout));

      if (not f.out.pattern.empty())
        pattern = fmt::format(pattern, f.out.pattern);

      std::cout << fmt::format(f.out.style, pattern, value);
    }
}
