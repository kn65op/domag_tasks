#include <gtest/gtest.h>
#include "../../../Code/depot/inc/Article.h"

using namespace ::testing;
using depot::Article;

struct ArticleTest: public Test
{
    Article::ArticlePtr article = std::dynamic_pointer_cast<Article>(Article::createTopLevelArticle());

  void TearDown()
  {
    Article::clearTopLevelArticles();
  }
};

TEST_F(ArticleTest, ArticleCreatedShouldNotHaveEmptyName)
{
  EXPECT_NE("", article->getName());
}

TEST_F(ArticleTest, TopLevelAndDependentArticleCreatedWithNameShouldHaveProperName)
{
  std::string beer_string{"Beer"};
  auto beer = Article::createTopLevelArticle(beer_string);
  EXPECT_EQ(beer_string, beer->getName());

  std::string wheat_beer_string{"Wheat beer"};
  auto wheat_beer = beer->createDependentArticle(wheat_beer_string);
  EXPECT_EQ(wheat_beer_string, wheat_beer->getName());
}

TEST_F(ArticleTest, TopLevelAndDependentArticleShouldBeCreatedWithEmptyName)
{
  EXPECT_NO_THROW(Article::createTopLevelArticle(""));
  EXPECT_NO_THROW(article->createDependentArticle(""));

  const std::string unit{"l"};
  EXPECT_NO_THROW(Article::createTopLevelArticle("", unit));
  EXPECT_NO_THROW(article->createDependentArticle("", unit));
}

TEST_F(ArticleTest, TopLevelAndDependentArticleCreatedWithNameShouldHaveProperNameAndUnit)
{
  const std::string unit = "liter";
  const std::string beer_string{"Beer"};
  auto beer = Article::createTopLevelArticle(beer_string, unit);
  EXPECT_EQ(unit, beer->getUnit());

  const std::string wheat_beer_string{"Wheat beer"};
  auto wheat_beer = beer->createDependentArticle(wheat_beer_string, unit);
  EXPECT_EQ(unit, wheat_beer->getUnit());
}

TEST_F(ArticleTest, AllArticlesShouldBeStoredInOnePlaseAndShouldBeSearchalbe)
{
  EXPECT_EQ(1U, Article::getTopLevelArticles().size());
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
  EXPECT_EQ(1U, Article::getTopLevelArticles().size());
  auto article_dependent = Article::createTopLevelArticle();
  EXPECT_EQ(2U, Article::getTopLevelArticles().size());
  article->addDependentArticle(article_dependent);
  EXPECT_EQ(1U, Article::getTopLevelArticles().size());
  EXPECT_EQ(1U, article->getArticles().size());
  article->removeDependentArticle(article_dependent);
  EXPECT_EQ(0U, article->getArticles().size());
  EXPECT_EQ(2U, Article::getTopLevelArticles().size());
  Article::removeTopLevelArticle(article_dependent);
  EXPECT_EQ(1U, Article::getTopLevelArticles().size());
}

TEST_F(ArticleTest, ShouldNotBeAbleToAddYourselfAsDependentArticle)
{
  EXPECT_THROW(article->addDependentArticle(article), Article::CannotMakeDependent);
}

TEST_F(ArticleTest, ShouldNotBeAbleToMakeCircularDependency)
{
  auto article_middle = article->createDependentArticle();
  auto article_bottom = article_middle->createDependentArticle();
  EXPECT_THROW(article_bottom->addDependentArticle(article), Article::CannotMakeDependent);
  EXPECT_THROW(article_bottom->addDependentArticle(article_middle), Article::CannotMakeDependent);
}

TEST_F(ArticleTest, ShouldThrowWhenTriedToRemoveNotDependentArticle)
{
  EXPECT_THROW(article->removeDependentArticle(Article::createTopLevelArticle()), Article::NoExistDependentArticle);
}

TEST_F(ArticleTest, NewArticleShouldNotHavePrecedentArticle)
{
  EXPECT_THROW(article->getPrecedentArticle(), Article::NoPrecedentArticle);
}

TEST_F(ArticleTest, AfterAddingDependentArticleShouldDependentArticleShouldHaveValidPrecedentAndAfterRemovalItShouldHaveNone)
{
  auto article_inside = article->createDependentArticle();
  EXPECT_EQ(article, article_inside->getPrecedentArticle());
  article->removeDependentArticle(article_inside);
  EXPECT_THROW(article_inside->getPrecedentArticle(), Article::NoPrecedentArticle);
  Article::removeTopLevelArticle(article_inside);
}

