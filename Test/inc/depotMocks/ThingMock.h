#include "depot/inc/Thing.h"
#include "gmock/gmock.h"

namespace depot
{

class ThingMock : public IThing
{
  MOCK_CONST_METHOD0(setName, void(std::string));
  MOCK_METHOD1(getName, std::string());
};

}