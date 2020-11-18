#include <iostream>
#include <boost/process.hpp>
#include <boost/algorithm/string.hpp>
#include "../utils/console.hpp"
using namespace std;
namespace bp = boost::process;
namespace fs = boost::filesystem;
namespace cons = black::console;

int main(int argc, char **argv)
{

  if (argc == 2)
  {
    std::string p = argv[1];
    std::vector<std::string> kv;
    boost::split(kv, p, boost::is_any_of("="));

    if (kv.size() == 2)
    {
      auto &&key = kv[0];
      auto &&value = kv[1];

      if (key == "package")
      {
        cons::blue("--- installation of " + value);
        std::string cmd = "sudo apt install " + value;
        int res = bp::system(cmd, bp::std_out > stdout, bp::std_err > stderr);

        if (res == 0)
          cons::green("--- installation done");
        else
          cons::red("--- error during installation");
      }
    }
  }

  return EXIT_FAILURE;
}
