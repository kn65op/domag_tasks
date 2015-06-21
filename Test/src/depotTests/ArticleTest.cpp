#include <gtest/gtest.h>
#include "../../../Code/depot/inc/Article.h"

using namespace ::testing;
using depot::Article;

struct ArticleTest: public Test
{
  Article t;
};

TEST_F(ArticleTest, ArticleCreatedShouldNotHaveEmptyName)
{
  ASSERT_NE(t.getName(), "");
}

TEST_F(ArticleTest, ArticleSetNameShouldNotAcceptEmptyName)
{
  ASSERT_THROW(t.setName(""), Article::NameEmptyException);
}

TEST_F(ArticleTest, ArticleAfterSetNameShouldGetSameName)
{
  std::string name = "name";
  t.setName(name);
  ASSERT_EQ(t.getName(), name);
}

TEST_F(ArticleTest, ShouldBeAbleToSetUnit)
{
  std::string unit{"unit"};
  t.setUnit(unit);
  ASSERT_EQ(unit, t.getUnit());
}

TEST_F(ArticleTest, AfterAdditionShouldHaveOneDependentArticleAndAfterRemovalShouldNotHaveDependedArticles)
{
  auto article = std::make_shared<Article>();
  t.addDependentArticle(article);
  EXPECT_EQ(1U, t.getArticles().size());
  t.removeDependentArticle(article);
  EXPECT_EQ(0U, t.getArticles().size());
}

TEST_F(ArticleTest, ShouldThrowWhenTriedToRemoveNotDependentArticle)
{
  EXPECT_THROW(t.removeDependentArticle(std::make_shared<Article>()), Article::NoExistDependentArticle);
}