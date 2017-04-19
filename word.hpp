#ifndef WORD_HPP
#define WORD_HPP

#include <QString>

class Word
{
    private:
        int m_id;
        QString m_name;
    public:
        Word(int _id, const QString& name);

        int id() const;
        const QString& name() const;
};

#endif // WORD_HPP
