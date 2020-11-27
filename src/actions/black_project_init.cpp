#include <iostream>
#include <fstream>
#include <string_view>
#include <string>
#include <list>
#include <regex>

#include <yaml-cpp/yaml.h>
#include <boost/algorithm/string/split.hpp>

// Le but de cette action est de proposer l'initialisation
// en demandant à l'utilisateur un nom, la liste des packages
// ainsi que la liste des repository git, un auteur et une
// courte description
namespace yaml = YAML;

namespace black
{
  struct project
  {
    std::string name;
    std::string description;
    std::string author;
    std::list<std::string> package;
    std::list<std::string> repo;
  };

  std::string ask(std::string_view question);
  std::string ask_one(std::string_view question);
  std::list<std::string> ask_multi(std::string_view question);

  std::string ask_name();
  std::string ask_description();
  std::string ask_author();
  std::list<std::string> ask_package();
  std::list<std::string> ask_repo();

  bool check_project(const project &pro);

  bool check_name(const std::string &name);
  bool check_author(const std::string &author);
  bool check_description(const std::string &description);
  bool check_packages(const std::list<std::string> &packages);
  bool check_package(const std::string &package);
  bool check_repos(const std::list<std::string> &repos);
  bool check_repo(const std::string &repo);

  yaml::Node convert(const project &pro);
  bool save(const yaml::Node &nd);
} // namespace black

int main()
{
  black::project pro;

  pro.name = black::ask_name();
  pro.author = black::ask_author();
  pro.description = black::ask_description();
  pro.package = black::ask_package();
  pro.repo = black::ask_repo();

  if (black::check_project(pro))
  {
    yaml::Node nd = black::convert(pro);
    bool res = black::save(nd);
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

namespace black
{
  std::string ask(std::string_view question)
  {
    std::string response;
    std::cout << question << " : \n>>> ";
    std::getline(std::cin, response);
    return response;
  }

  std::string ask_one(std::string_view question)
  {
    return ask(question);
  }

  std::list<std::string> ask_multi(std::string_view question)
  {
    std::string response = ask(question);
    constexpr auto is_space = [](auto c) { return c == ' '; };
    std::list<std::string> responses;
    boost::split(responses, response, is_space, boost::token_compress_on);

    return responses;
  }

  std::string ask_name()
  {
    std::string question = " -- name ?";
    return ask_one(question);
  }
  std::string ask_description()
  {
    std::string question = " -- description ?";
    return ask_one(question);
  }

  std::string ask_author()
  {
    std::string question = " -- author ?";
    return ask_one(question);
  }

  std::list<std::string> ask_package()
  {
    std::string question = " -- package ?";
    return ask_multi(question);
  }

  std::list<std::string> ask_repo()
  {
    std::string question = " -- repo ?";
    return ask_multi(question);
  }

  bool check_project(const project &pro)
  {
    return check_name(pro.name) and
           check_author(pro.author) and
           check_description(pro.description) and
           check_packages(pro.package) and
           check_repos(pro.repo);
  }

  bool check_name(const std::string &name)
  {
    return std::regex_match(name, std::regex("[a-zA-Z_0-9]*"));
  }

  bool check_author(const std::string &author)
  {
    return true;
  }

  bool check_description(const std::string &description)
  {
    return true;
  }

  bool check_packages(const std::list<std::string> &packages)
  {
    return std::all_of(
        packages.begin(), packages.end(),
        [](const std::string &p) { return check_package(p); });
  }

  bool check_package(const std::string &package)
  {
    return true;
  }

  bool check_repos(const std::list<std::string> &repos)
  {
    return std::all_of(
        repos.begin(), repos.end(),
        [](const std::string &r) { return check_repo(r); });
  }

  bool check_repo(const std::string &repo)
  {
    return true;
  }

  yaml::Node convert(const project &pro)
  {
    yaml::Node pn;
    pn["name"] = pro.name;
    pn["author"] = pro.author;
    pn["description"] = pro.description;
    pn["package"] = pro.package;
    pn["repo"] = pro.repo;

    yaml::Node n;
    n["project"] = pn;
    return n;
  }

  bool save(const yaml::Node &nd)
  {
    std::ofstream out(nd["project"]["name"].as<std::string>() + ".yml");
    out << "---\n";
    out << nd;
    return true;
  }

} // namespace black
