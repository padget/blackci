#include <yaml-cpp/yaml.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <list>

#include "utils/console.hpp"
#include "utils/format.hpp"
#include "utils/process.hpp"
#include "utils/options.hpp"

namespace yaml = YAML;
namespace cons = black::console;
namespace po = boost::program_options;

namespace black
{
  struct project
  {
    std::string name;
    std::string author;
    std::string description;
    std::list<std::string> package;
    std::list<std::string> repo;
  };
} // namespace black

namespace YAML
{
  template <>
  struct convert<black::project>
  {
    static Node encode(
        const black::project &p)
    {
      Node node;
      node.push_back(p.name);
      node.push_back(p.author);
      node.push_back(p.description);
      node.push_back(p.package);
      node.push_back(p.repo);
      return node;
    }

    static bool decode(
        const Node &n,
        black::project &p)
    {
      using strings = std::list<std::string>;
      using string = std::string;

      if (n["name"])
        p.name = n["name"].as<string>();
      if (n["author"])
        p.author = n["author"].as<string>();
      if (n["description"])
        p.description = n["description"].as<string>();
      if (n["package"])
        p.package = n["package"].as<strings>();
      if (n["repo"])
        p.repo = n["repo"].as<strings>();
      return true;
    }
  };
} // namespace YAML

bool configure_project(
    black::project const &p)
{
  using namespace black;

  bool global_res = true;
  std::string proxy = "apt_proxy";

  for (auto &&package : p.package)
  {
    int res = black::run_attached(
        proxy, format("package=%1%", package));
    global_res = global_res && res == 0;
  }

  return global_res;
}

int main(int argc, char **argv)
{
  black::options opts("black");
  opts.add("help,h", "Help screen");
  opts.add<std::string>("job,j", "job name to execute");
  opts.add<std::string>("project,p", "project to scan to find the job");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, opts.get()), vm);
  po::notify(vm);

  if (vm.count("help") == 1)
  {
    std::cout << opts.get() << '\n';
  }

  if (vm.count("project") == 1 and vm.count("job") == 1)
  {
    std::string projyml = vm["project"].as<std::string>();
    std::string jobyml = vm["job"].as<std::string>();

    try
    {
      yaml::Node node = yaml::LoadFile(projyml);
      if (node["project"])
      {
        black::project p = node["project"].as<black::project>();
        configure_project(p);
      }
    }
    catch (const std::exception &e)
    {
      cons::red(e.what());
    }
  }
}
