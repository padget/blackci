#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <list>

#include <boost/process.hpp>

int main(int argc, char **argv)
{
  namespace bp = boost::process;
  bp::pipe p;

  bp::child act = bp::child("./dist/black_project_init",
                            bp::std_in = stdin,
                            bp::std_out = p,
                            bp::std_err = stderr);

  bp::child paint = bp::child("./dist/black_paint",
                              "black_project_init",
                              bp::std_in = p,
                              bp::std_out = stdout,
                              bp::std_err = stderr);

  act.wait();
  paint.wait();

  return EXIT_SUCCESS;
}
