#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <QString>

class Actor
{
    private:
        int m_id;
        QString m_name;
    public:
        Actor(int _id, const QString& name);

        int id() const;
        const QString& name() const;
};

#endif // ACTOR_HPP
