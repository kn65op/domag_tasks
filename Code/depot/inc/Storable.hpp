#pragma once

#include <memory>

#include <optional>

namespace depot
{

class AbstractContainer;

class Storable
{
public:
    std::optional<std::shared_ptr<AbstractContainer>> getStorehause() const
    {
        return getStorehauseImpl();
    }

    virtual ~Storable() = default;

private:
    virtual std::optional<std::shared_ptr<AbstractContainer>> getStorehauseImpl() const = 0;
};

} // namespace depot