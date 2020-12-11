#include <iostream>
#include <fstream>
#include <string_view>
#include <string>
#include <list>
#include <regex>

#include <yaml-cpp/yaml.h>
#include <boost/algorithm/string/split.hpp>
#include <fmt/format.h>
#include "../../utils/json.hpp"
#include "../../utils/json_utils.hpp"

// Le but de cette action est de proposer l'initialisation
// en demandant à l'utilisateur un nom, la liste des packages
// ainsi que la liste des repository git, un auteur et une
// courte description
namespace yaml = YAML;

namespace black
{
  std::string ask(const std::string& q);
  std::string ask_one(const std::string& question);
  std::list<std::string> ask_multi(const std::string& question);

  std::string ask_name();
  std::string ask_description();
  std::string ask_author();
  std::list<std::string> ask_package();
  std::list<std::string> ask_repo();

  bool save(const nlohmann::json &nd);
} // namespace black

int main()
{
  std::cout << black::json::title("Init new project", "2");
  nlohmann::json jproj;
  jproj["project"]["name"] = black::ask_name();
  jproj["project"]["author"] = black::ask_author();
  jproj["project"]["description"] = black::ask_description();
  jproj["project"]["packages"] = black::ask_package();
  jproj["project"]["repos"] = black::ask_repo();
  
  bool res = black::save(jproj);

  std::cout << black::json::report("changed");

  return EXIT_SUCCESS;
}

namespace black
{
  std::string ask(const std::string& question)
  {
    std::string response;
    std::cout << black::json::question(question);
    std::getline(std::cin, response);
    return response;
  }

  std::string ask_one(const std::string& question)
  {
    return ask(question);
  }

  std::list<std::string> ask_multi(const std::string& question)
  {
    std::string response = ask(question);
    constexpr auto is_space = [](auto c) { return c == ' '; };
    std::list<std::string> responses;
    boost::split(responses, response, is_space, boost::token_compress_on);
    return responses;
  }

  std::string ask_name()
  {
    std::string question = "name (name of the project configuration file)";
    return ask_one(question);
  }

  std::string ask_description()
  {
    std::string question = "description (short description of the project)";
    return ask_one(question);
  }

  std::string ask_author()
  {
    std::string question = "author (ex: Jhon Doe)";
    return ask_one(question);
  }

  std::list<std::string> ask_package()
  {
    std::string question = "package (separated by space list)";
    return ask_multi(question);
  }

  std::list<std::string> ask_repo()
  {
    std::string question = "repo (separated by space list)";
    return ask_multi(question);
  }

  bool save(const nlohmann::json &jproj)
  {
    auto name = jproj["project"]["name"].get<std::string>();
    std::ofstream out(name + ".json");
    out << jproj;
    return true;
  }

} // namespace black
