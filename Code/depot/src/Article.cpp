#include "Article.hpp"

#include <TLogger.h>
#include <algorithm>

using depot::Article;

template <typename Entity, typename EntityInterface>
typename depot::HierarchicalClass<Entity, EntityInterface>::EntitiesContainer
    depot::HierarchicalClass<Entity, EntityInterface>::top_level_entities;

Article::~Article()
{
    LOG << "Removing: " << name.getContent();
}

Article::Article(const std::string& n, const std::string& u) : name{n}, unit{u}
{
    LOG << "Article " << name.getContent() << " with unit: " << unit << " created";
}

void Article::checkPassedName(const std::string& n)
{
    if (n.empty())
    {
        LOG << "Set default category name";
        THelper::String::UniqueStdCategorizedString<Article::UniqueStringCategory>::setDefaultName("Unnamed Article");
    }
}

std::string Article::getName() const noexcept
{
    return name;
}

void Article::setName(const std::string& n)
{
    if (n.empty())
    {
        throw NameEmptyException();
    }
    name = n;
}

void Article::setUnit(const std::string& u)
{
    unit = u;
}

std::string Article::getUnit() const
{
    return unit;
}

void Article::addDependentArticle(DependentArticle article)
{
    LOG << "Add dependent article" << article->getName();
    addInferiorEntity(article, this);
}

const Article::Articles& Article::getArticles() const
{
    return getInferiorEntities();
}

Article::DependentArticle Article::removeDependentArticle(DependentArticle article)
{
    LOG << "Remove dependent article" << article->getName();
    return removeInferiorEntity(article);
}

std::optional<Article::DependentArticle> Article::getPrecedentArticle() const
{
    const auto entity = getPrecedentEntity();
    if (entity)
    {
        return entity.value();
    }
    return {};
}

Article::DependentArticle Article::createDependentArticle(const std::string& n, const std::string& u)
{
    return createDependentEntity(n, u);
}
