#pragma once

#include <string>

namespace depot
{
namespace main
{

class DomagRunner
{
public:
  DomagRunner();
  ~DomagRunner();

private:
  const std::string databaseFileName = "database.domag";
};

}
}