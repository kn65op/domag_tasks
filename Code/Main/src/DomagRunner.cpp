#include "DomagRunner.h"

#include <iostream>

#include "TLogger.h"

#include "DepotSerializer.h"

#include "HomeContainerCatalog.h"

#include "presenters/ContainersPresenter.hpp"

namespace depot
{
namespace main
{

DomagRunner::DomagRunner() : DomagRunner{nullptr}
{
}

DomagRunner::DomagRunner(std::unique_ptr<ui::ContainersPresenter> containersPresenter)
    : containersPresenter{std::move(containersPresenter)}
{
  LOG << "Initializing domag";
  depot::serialize::DepotSerializer serializer;
  std::ifstream database{databaseFileName};
  if (!database)
  {
    LOG << "Database not exists, so it will be created";
    return;
  }
  try
  {
    serializer.deserialize(database);
  }
  catch (const depot::serialize::DepotSerializer::InvalidVersion &ex)
  {
    LOG << "Database version is invalid so there will be no data";
  }
  LOG << "Domag initalized";
}

DomagRunner::~DomagRunner()
{
  LOG << "Finishing domag";
  depot::serialize::DepotSerializer serializer;
  std::ofstream database{databaseFileName};
  serializer.serialize(database);
  LOG << "Database stored to file";
  HomeContainerCatalog{}.clearAllContainers();
  LOG << "Domag finished";
}

void DomagRunner::run()
{
  constexpr int exitOption{0};
  while (true)
  {
    printMenu();
    const auto option = getOption();
    if (option == exitOption)
    {
      return;
    }
    if (option == 1)
    {
      containersPresenter->present(depot::HomeContainerCatalog{});
    }
    std::cout << "\n";
  }
}

void DomagRunner::printMenu()
{
  LOG << "Print menu";
  std::cout << "Menu:\n";
  std::cout << "1:\tShow Containers\n";
  std::cout << "0:\tExit\n";
}

int DomagRunner::getOption()
{
  std::string optionSelected;
  std::cin >> optionSelected;
  LOG << "Read option: " << optionSelected;
  try
  {
    return std::stoi(optionSelected);
  }
  catch (const std::exception &ex)
  {
    LOG << "Conversion from string: " << optionSelected << " to int failed due: " << ex.what();
    constexpr int invalidOption{-1};
    return invalidOption;
  }
}
}
}