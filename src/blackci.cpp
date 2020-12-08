#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>
#include <sstream>
#include <list>

#include <boost/process.hpp>

#include "utils/json.hpp"

nlohmann::json log(
  std::string_view lvl, 
  const std::string& msg)

{
  nlohmann::json j;
  j["log"]["msg"] = msg;
  j["log"]["lvl"] = lvl;
  return j;
}

int main(int argc, char **argv)
{
  namespace bp = boost::process;
  namespace json = nlohmann;

  if (argc <= 1)
  {
    std::cerr << log("info", u8R"(bad args line \n->black<action><args>...)");
    return EXIT_FAILURE;
  }

  std::string action = argv[1];
  std::vector<std::string> args;

  for (int i = 2; i < argc; i++)
    args.push_back(argv[i]);
  
  std::string exe;

  if (action == "project")
    exe = "./actions_project";
  else if (action == "job")
    exe = "./actions_job";
  else if (action == "admin")
    exe = "./actions_admin";
  else
  {
    std::cerr << "ERROR : bad action\n";
    std::cerr << "possible value: project, job or admin\n";
    return EXIT_FAILURE;
  }
  auto raw = R"(qlksdjqklsjdqlkdjk)";
  bp::child(bp::exe = exe,
            bp::args = args,
            bp::std_in = stdin,
            bp::std_out = stdout,
            bp::std_err = stderr)
      .wait();

  return EXIT_SUCCESS;
}
