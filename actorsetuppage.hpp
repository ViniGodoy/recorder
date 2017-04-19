#ifndef ACTORSETUPPAGE_HPP
#define ACTORSETUPPAGE_HPP

#include <QWizardPage>

namespace Ui {
class ActorSetupPage;
}

class ActorSetupPage : public QWizardPage
{
        Q_OBJECT
        
    public:
        explicit ActorSetupPage(QWidget *parent = 0);
        ~ActorSetupPage();
        
    private:
        Ui::ActorSetupPage *ui;
        void parseXml();
};

#endif // ACTORSETUPPAGE_HPP
