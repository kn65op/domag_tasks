#include <gtest/gtest.h>
#include "../../../Code/depot/inc/Article.h"

using namespace ::testing;
using depot::Article;

struct ArticleTest: public Test
{
  Article::ArticlePtr article = Article::createArticle();
};

TEST_F(ArticleTest, ArticleCreatedShouldNotHaveEmptyName)
{
  ASSERT_NE(article->getName(), "");
}

TEST_F(ArticleTest, ArticleSetNameShouldNotAcceptEmptyName)
{
  ASSERT_THROW(article->setName(""), Article::NameEmptyException);
}

TEST_F(ArticleTest, ArticleAfterSetNameShouldGetSameName)
{
  std::string name = "name";
  article->setName(name);
  ASSERT_EQ(article->getName(), name);
}

TEST_F(ArticleTest, ShouldBeAbleToSetUnit)
{
  std::string unit{"unit"};
  article->setUnit(unit);
  ASSERT_EQ(unit, article->getUnit());
}

TEST_F(ArticleTest, AfterAdditionShouldHaveOneDependentArticleAndAfterRemovalShouldNotHaveDependedArticles)
{
  auto article = Article::createArticle();
  article->addDependentArticle(article);
  EXPECT_EQ(1U, article->getArticles().size());
  article->removeDependentArticle(article);
  EXPECT_EQ(0U, article->getArticles().size());
}

TEST_F(ArticleTest, ShouldThrowWhenTriedToRemoveNotDependentArticle)
{
  EXPECT_THROW(article->removeDependentArticle(Article::createArticle()), Article::NoExistDependentArticle);
}
/*
TEST_F(ContainerTest, AfterAddingOneContainerShouldContainerInsideShouldKnowWhereItLies)
{
  auto article_inside = Article::createArticle();
  article->addDependentArticle(article_inside);
  EXPECT_EQ(article, article_inside->getPrecedentArticle());
}

TEST_F(ContainerTest, AfterAddingOneContainerShouldReturnOneContainerAndShouldBeRemoveable)
{
  auto cont = Container::createContainer();
  c->addContainer(cont);
  EXPECT_EQ(1U, c->getContainers().size());
  c->removeContainer(cont);
  EXPECT_EQ(0U, c->getContainers().size());
}
*/