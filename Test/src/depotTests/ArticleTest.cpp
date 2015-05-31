#include <gtest/gtest.h>
#include "../../../Code/depot/inc/Article.h"

using namespace ::testing;
using depot::Article;

struct ArticleTest: public Test
{
  Article t;
};

TEST_F(ArticleTest, ThingCreatedShouldNotHaveEmptyName)
{
  ASSERT_NE(t.getName(), "");
}

TEST_F(ArticleTest, ThingSetNameShouldNotAcceptEmptyName)
{
  ASSERT_THROW(t.setName(""), Article::NameEmptyException);
}

TEST_F(ArticleTest, ThingAfterSetNameShouldGetSameName)
{
  std::string name = "name";
  t.setName(name);
  ASSERT_EQ(t.getName(), name);
}