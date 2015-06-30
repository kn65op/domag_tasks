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
  EXPECT_NE(article->getName(), "");
}

TEST_F(ArticleTest, TopLevelAndDependentArticleCreatedWithNameShouldHaveProperName)
{
  std::string beer_string{"Beer"};
  auto beer = TopLevelArticles::createTopLevelArticle(beer_string);
  EXPECT_EQ(beer_string, beer->getName());

  std::string wheat_beer_string{"Wheat beer"};
  auto wheat_beer = Article::createDependentArticle(beer, wheat_beer_string);
  EXPECT_EQ(wheat_beer_string, wheat_beer->getName());
}

TEST_F(ArticleTest, TopLevelAndDependentArticleShouldNotBeCreatedWithEmptyName)
{
  EXPECT_THROW(TopLevelArticles::createTopLevelArticle(""), Article::NameEmptyException);
  EXPECT_THROW(Article::createDependentArticle(article, ""), Article::NameEmptyException);

  const std::string unit{"l"};
  EXPECT_THROW(TopLevelArticles::createTopLevelArticle("", unit), Article::NameEmptyException);
  EXPECT_THROW(Article::createDependentArticle(article, "", unit), Article::NameEmptyException);
}

TEST_F(ArticleTest, TopLevelAndDependentArticleCreatedWithNameShouldHaveProperNameAndUnit)
{
  const std::string unit = "liter";
  const std::string beer_string{"Beer"};
  auto beer = TopLevelArticles::createTopLevelArticle(beer_string, unit);
  EXPECT_EQ(unit, beer->getUnit());

  const std::string wheat_beer_string{"Wheat beer"};
  auto wheat_beer = Article::createDependentArticle(beer, wheat_beer_string, unit);
  EXPECT_EQ(unit, wheat_beer->getUnit());
}

TEST_F(ArticleTest, AllArticlesShouldBeStoredInOnePlaseAndShouldBeSearchalbe)
{
  EXPECT_EQ(1, TopLevelArticles::getTopLevelArticles().size());
}

TEST_F(ArticleTest, ArticleSetNameShouldNotAcceptEmptyName)
{
  EXPECT_THROW(article->setName(""), Article::NameEmptyException);
}

TEST_F(ArticleTest, ArticleAfterSetNameShouldGetSameName)
{
  const std::string name = "name";
  article->setName(name);
  ASSERT_EQ(article->getName(), name);
}

TEST_F(ArticleTest, ShouldBeAbleToSetUnit)
{
  const std::string unit{"unit"};
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