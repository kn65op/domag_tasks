#include "CliCallback.hpp"

#include "HomeContainerCatalog.h"
#include "presenters/ContainersPresenter.hpp"
#include "presenters/Presenters.hpp"

namespace domagcli
{

std::unique_ptr<CliCallback> CliCallback::callback = nullptr;

std::string CliCallback::getAllContainers()
{
  return presenters->containersPresenter->present(depot::HomeContainerCatalog());
}

void CliCallback::initializeCallback(std::unique_ptr<ui::Presenters> pres)
{
  callback.reset(new CliCallback{std::move(pres)});
}

CliCallback::CliCallback(std::unique_ptr<ui::Presenters> pres) : presenters{std::move(pres)}
{
}

std::string CliCallback::showAllContainers()
{
  return callback->getAllContainers();
}
}
