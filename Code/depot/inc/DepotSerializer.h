#pragma once

#include <iostream>
#include <map>
#include "Article.h"

namespace depot
{
namespace serialize
{

class DepotSerializer
{
public:
  void serialize(std::ostream& out);

private:
  std::map<Article::ArticlePtr, int> articles;
};

}
}
