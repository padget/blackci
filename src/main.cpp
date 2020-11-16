#include <iostream>
#include <yaml-cpp/yaml.h>
#include "subprocess.hpp"

namespace yaml = YAML;

int main()
{
  subprocess::popen p = subprocess::popen("ls", {});

  std::cout << p.stdout().rdbuf();

  yaml::Node node = yaml::LoadFile("examples/test.yml");

  std::cout << node;
  std::cout << "coucou" << std::endl;
}
