#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <list>

#include "utils/frontend.hpp"
#include "utils/process.hpp"
#include "utils/options.hpp"
#include "utils/yaml.hpp"

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
      Node n;
      n.push_back(p.name);
      n.push_back(p.author);
      n.push_back(p.description);
      n.push_back(p.package);
      n.push_back(p.repo);
      return n;
    }

    static bool decode(
        const Node &n,
        black::project &p)
    {
      using strings = std::list<std::string>;
      using string = std::string;
      namespace yaml = black::yaml;

      if (yaml::has(n, "name"))
        p.name = yaml::as_str(n, "name");
      if (yaml::has(n, "author"))
        p.author = yaml::as_str(n, "author");
      if (yaml::has(n, "description"))
        p.description = yaml::as_str(n, "description");
      if (yaml::has(n, "package"))
        p.package = yaml::as_strs(n, "package");
      if (n["repo"])
        p.repo = yaml::as_strs(n, "repo");
      return true;
    }
  };
} // namespace YAML

bool configure_project(
    black::project const &p)
{
  using namespace black;
  using namespace black::front;

  bool global_res = true;
  std::string proxy = "apt_proxy";

  std::cout << project_title(p.name);
  std::cout << "configuration du projet en cours...\n";
  for (auto &&package : p.package)
  {
    std::cout << project_package(p.name, package);
    std::cout << "vvérification package présent " << package << "\n";
    int res = run_attached(
        proxy, fmt::format("package={}", package));
    global_res = global_res && res == 0;
  }

  return global_res;
}

int main(int argc, char **argv)
{
  namespace yaml = black::yaml;

  black::options opts("black", argc, argv);
  opts.add("help,h", "Help screen");
  opts.add_str("job,j", "job name to execute", true);
  opts.add_str("project,p", "project to scan to find the job");

  try
  {
    opts.parse();

    if (opts.has("help"))
    {
      std::cout << "help page" << std::endl;
      std::cout << opts.description() << '\n';
      return EXIT_SUCCESS;
    }

    auto projyml = opts.get_str("project");
    auto jobyml = opts.get_str("job");

    YAML::Node node = YAML::LoadFile(projyml.value());

    if (yaml::has(node, "project"))
    {
      black::project p = yaml::as<black::project>(node, "project");
      configure_project(p);
    }
  }
  catch (std::bad_optional_access &bad)
  {
  }
  catch (const std::exception &e)
  {
  }

  return EXIT_SUCCESS;
}
