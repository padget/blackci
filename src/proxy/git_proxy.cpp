#include <iostream>
#include <boost/program_options.hpp>
#include <boost/process.hpp>

using namespace std;
namespace bp = boost::process;
namespace fs = boost::filesystem;

int main(int argc, char **argv)
{
  fs::path p = bp::search_path("git");

  if (p.empty())
  {
    cout << "git not found\n";
    return EXIT_FAILURE;
  }

  cout << "git proxy\n";
  return EXIT_SUCCESS;
}
