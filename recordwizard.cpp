#include "recordwizard.hpp"
#include "ui_recordwizard.h"
#include <QMessageBox>

RecordWizard::RecordWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::RecordWizard),
    video(nullptr),
    tmrVideo(new QTimer(this)),
    wgtDepthRecording(new CVWidget()),
    wgtDepthFraming(new CVWidget()),
    wgtDepthSkeleton(new CVWidget())
{
    setDefaultProperty("QWizardComboBox", "currentItemData", "currentItemDataChanged");
    ui->setupUi(this);
    setButtonText(QWizard::FinishButton, tr("Finalizar"));
    setButtonText(QWizard::NextButton, tr("Avançar"));
    setButtonText(QWizard::BackButton, tr("Voltar"));    
    tmrVideo->setInterval(10);
    tmrVideo->setSingleShot(false);

    ui->layRecording->addWidget(wgtDepthRecording);
    ui->layFramingVideo->addWidget(wgtDepthFraming);
    ui->laySkeleton->addWidget(wgtDepthSkeleton);

    connect(tmrVideo.data(), SIGNAL(timeout()), this, SLOT(updateVideo()));
}

RecordWizard::~RecordWizard()
{
    delete ui;
}

void RecordWizard::initializePage(int id)
{
    if (id == PAGE_SKELETON) initializeSkeletonPage();
}

void RecordWizard::initializeSkeletonPage()
{
    try
    {
        if (video.isNull())
        {
            video.reset(new xncv::VideoSource());
            video->start();
            tracker.reset(new xncv::UserTracker(*video));
        }
        tmrVideo->start();
    }
    catch (xncv::Exception e)
    {
        qWarning(e.what());
        QMessageBox error;
        error.setWindowTitle(tr("Erro"));
        error.setText(tr("Não foi possivel inicializar o hardware!"));
        error.exec();
        done(QDialog::Rejected);
    }
}

void RecordWizard::updateVideo()
{
    if (video.isNull())
        return;

    video->update();
    cv::Mat depthHist = video->calcDepthHist();
    cv::Mat depth = xncv::cvtDepthTo8UHist(video->captureDepth(), depthHist);

    if (!tracker.isNull())
    {
        std::vector<xncv::User> users = xncv::filterClosest(tracker->getUsers());

        for (unsigned i = 0; i < users.size(); ++i)
        {
            //If already tracking user, draws it.
            xncv::drawLimbs(depth, users[i].getLimbs(video->getXnDepthGenerator()));
        }
    }

    if (currentId() == PAGE_SKELETON)
        wgtDepthSkeleton->sendImage(depth);
    else if (currentId() == PAGE_FRAMING)
    {
        //Draw guides
        cv::line(depth, cv::Point(5,10), cv::Point(635,10), cv::Scalar::all(255), 3);
        cv::line(depth, cv::Point(5,470), cv::Point(635,470), cv::Scalar::all(255), 3);
        wgtDepthFraming->sendImage(depth);
    }
    else if (currentId() == PAGE_RECORDING)
    {
        wgtDepthRecording->sendImage(depth);
        if (video->isRecording())
        {
            cv::circle(depth, cv::Point(600,15), 8, cv::Scalar::all(255), -1);

            std::vector<xncv::User> users = xncv::filterClosest(tracker->getUsers());
            skeletonWriter->beginFrame();
                for (unsigned i = 0; i < users.size(); ++i)
                    *skeletonWriter << users[i];
            skeletonWriter->endFrame();
        }
    }
}

void RecordWizard::on_btnRecord_clicked()
{
    ui->btnRecord->setEnabled(false);
    button(QWizard::CancelButton)->setEnabled(false);
    skeletonWriter.reset(new xncv::SkeletonWriter(*video));
    int idActor = field("actor").toInt();
    int idWord = field("word").toInt();
    int shot = field("take").toInt();

    oniFile = QString("V_%1_%2_%3.oni")
            .arg(idActor,2,10,QChar('0'))
            .arg(shot)
            .arg(idWord,2,10,QChar('0'));

    sklFile = QString("V_%1_%2_%3.skl")
            .arg(idActor,2,10,QChar('0'))
            .arg(shot)
            .arg(idWord,2,10,QChar('0'));

    try
    {
        video->startRecording(oniFile.toStdString(), xncv::IMG_NONE);
        skeletonWriter->open(sklFile.toStdString());
    }
    catch (xncv::Exception &e)
    {
        qDebug(e.what());
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Erro"));
        msgBox.setText(tr("Problemas ao carregar arquivo."));
        msgBox.exec();
    }
}

const QString& RecordWizard::getOniFile() const
{
    return oniFile;
}
const QString& RecordWizard::getSklFile() const
{
    return sklFile;
}
