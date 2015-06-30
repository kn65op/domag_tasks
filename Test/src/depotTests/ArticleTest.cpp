#include <gtest/gtest.h>
#include "../../../Code/depot/inc/Article.h"

using namespace ::testing;
using depot::Article;
using depot::TopLevelArticles;

struct ArticleTest: public Test
{
  Article::ArticlePtr article = TopLevelArticles::createTopLevelArticle();

  void TearDown()
  {
//    TopLevelArticles::removeTopLevelArticle(article);
    TopLevelArticles::clearTopLevelArticles();
  }
};

TEST_F(ArticleTest, ArticleCreatedShouldNotHaveEmptyName)
{
  ASSERT_NE(article->getName(), "");
}

TEST_F(ArticleTest, AllArticlesShouldBeStoredInOnePlaseAndShouldBeSearchalbe)
{
  EXPECT_EQ(1, TopLevelArticles::getTopLevelArticles().size());
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
  EXPECT_EQ(1, TopLevelArticles::getTopLevelArticles().size());
  auto article_dependent = TopLevelArticles::createTopLevelArticle();
  EXPECT_EQ(2, TopLevelArticles::getTopLevelArticles().size());
  article->addDependentArticle(article_dependent);
  EXPECT_EQ(1, TopLevelArticles::getTopLevelArticles().size());
  EXPECT_EQ(1U, article->getArticles().size());
  article->removeDependentArticle(article_dependent);
  EXPECT_EQ(0U, article->getArticles().size());
  EXPECT_EQ(2, TopLevelArticles::getTopLevelArticles().size());
  TopLevelArticles::removeTopLevelArticle(article_dependent);
  EXPECT_EQ(1, TopLevelArticles::getTopLevelArticles().size());
}

TEST_F(ArticleTest, ShouldThrowWhenTriedToRemoveNotDependentArticle)
{
  EXPECT_THROW(article->removeDependentArticle(TopLevelArticles::createTopLevelArticle()), Article::NoExistDependentArticle);
}

TEST_F(ArticleTest, NewArticleShouldNotHavePrecedentArticle)
{
  EXPECT_THROW(article->getPrecedentArticle(), Article::NoPrecedentArticle);
}

TEST_F(ArticleTest, AfterAddingDependentArticleShouldDependentArticleShouldHaveValidPrecedentAndAfterRemovalItShouldHaveNone)
{
  auto article_inside = Article::createDependentArticle(article);
  EXPECT_EQ(article, article_inside->getPrecedentArticle());
  article->removeDependentArticle(article_inside);
  EXPECT_THROW(article_inside->getPrecedentArticle(), Article::NoPrecedentArticle);
  TopLevelArticles::removeTopLevelArticle(article_inside);
}