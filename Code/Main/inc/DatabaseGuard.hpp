#pragma once

#include <string>

namespace depot
{
namespace main
{

class DatabaseGuard
{
public:
  DatabaseGuard();
  ~DatabaseGuard();

private:
  const std::string databaseFileName = "database.domag";
};

}
}
