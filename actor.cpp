#include "actor.hpp"

Actor::Actor(int id, const QString& name)
    : m_id(id), m_name(name)
{
}

int Actor::id() const
{
    return m_id;
}

const QString& Actor::name() const
{
    return m_name;
}
