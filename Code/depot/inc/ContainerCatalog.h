#pragma once

#include <memory>
#include <string>
#include <vector>

class Container;

namespace depot
{

class ContainerCatalog
{
public:
  using ContainerInside = std::shared_ptr<Container>;
  using Containers = std::vector<ContainerInside>;

  virtual ~ContainerCatalog() = default;

  virtual const Containers& getTopLevelContainers() const = 0;
  virtual void clearTopLevelContainers() = 0;
  virtual ContainerInside createTopLevelContainer(const std::string& name = "Unnamed container") = 0;
  virtual void removeTopLevelContainer(ContainerInside) = 0;

};

}
