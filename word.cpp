#include "word.hpp"

Word::Word(int id, const QString& name)
    : m_id(id), m_name(name)
{
}

int Word::id() const
{
    return m_id;
}

const QString& Word::name() const
{
    return m_name;
}
