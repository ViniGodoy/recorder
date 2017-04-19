#include "actorsetuppage.hpp"
#include "ui_actorsetuppage.h"

#include <QtXml/QtXml>

#include "actor.hpp"
#include "word.hpp"

ActorSetupPage::ActorSetupPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ActorSetupPage)
{
    ui->setupUi(this);
    registerField("actor", ui->cmbActor);
    registerField("word", ui->cmbWord);
    registerField("take", ui->spinTake);
    parseXml();
}

ActorSetupPage::~ActorSetupPage()
{
    delete ui;
}

void ActorSetupPage::parseXml()
{
    QDomDocument xml;
    QFile file("recorder.xml");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug("Unable to open file");
        return;
    }
    if (!xml.setContent(&file))
    {
        qDebug("Unable to set xml content");
        return;
    }

    file.close();
    QDomElement docElem = xml.documentElement();

    //Parse the actor list
    QDomNodeList actorsElem = docElem.elementsByTagName("actor");
    for (int i = 0; i < actorsElem.size(); ++i)
    {
        QDomNamedNodeMap attributes = actorsElem.at(i).attributes();
        Actor actor(attributes.namedItem("id").nodeValue().toInt(),
                    attributes.namedItem("name").nodeValue());
        ui->cmbActor->addItem(actor.name(), actor.id());
    }

    //Parse the word list
    QDomNodeList wordsElem = docElem.elementsByTagName("word");
    for (int i = 0; i < wordsElem.size(); ++i)
    {
        QDomNamedNodeMap attributes = wordsElem.at(i).attributes();
        Word word(attributes.namedItem("id").nodeValue().toInt(),
                    attributes.namedItem("name").nodeValue());
        ui->cmbWord->addItem(word.name(), word.id());
    }
}
