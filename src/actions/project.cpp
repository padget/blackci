#include <iostream>
#include <string>
#include <vector>

#include <boost/process.hpp>
#include "../utils/json.hpp"
#include "../utils/json_utils.hpp"
#include <fmt/format.h>
#include <fmt/color.h>

int main(int argc, char **argv)
{
  namespace bp = boost::process;
  namespace json = nlohmann;

  bp::opstream err;
  bp::child errpaint(
      bp::exe = "filters_paint",
      bp::args = {"--config=filters_paint.json",
                  "--filter=exception"},
      bp::std_in = err,
      bp::std_out = stdout,
      bp::std_err = stderr);

  if (argc < 2)
  {
    auto &&msg = "missing arguments";
    auto &&exc = std::invalid_argument(msg);
    auto &&je = black::json::from_error(exc);
    auto &&style = fmt::fg(fmt::color::gray) |
                   fmt::emphasis::bold;
    err << fmt::format(style, "{}\n", je.dump());
    errpaint.wait();

    return EXIT_FAILURE;
  }

  std::string subaction = argv[1];
  std::string exe;
  std::vector<std::string> args;

  for (int i = 2; i < argc; i++)
    args.push_back(argv[i]);

  if (subaction == "--init")
    exe = "./actions_project_init";

  if (not subaction.empty())
  {
    bp::pipe act_to_pnt;

    bp::child pnt = bp::child(
        bp::exe = "filters_paint",
        bp::args = {"--config=filters_paint.json",
                    "--filter=actions_project_init"},
        bp::std_in = act_to_pnt,
        bp::std_out = stdout,
        bp::std_err = stderr);

    bp::child act = bp::child(
        bp::exe = exe,
        bp::args = args,
        bp::std_in = stdin,
        bp::std_out = act_to_pnt,
        bp::std_err = stderr);

    act.wait();
    pnt.wait();
  }

  errpaint.wait();
  return EXIT_SUCCESS;
}
