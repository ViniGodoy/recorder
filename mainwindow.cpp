#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "cvwidget.hpp"

#include "recordwizard.hpp"
#include "about.hpp"

#include "xncv/xncv.hpp"

class Delay : public QThread
{
public:
    static void sleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    wgtVideo(new CVWidget()),
    wgtDepth(new CVWidget()),
    lblFrame(new QLabel("")),
    lblFrameCount(new QLabel(""))
{    
    ui->setupUi(this);
    ui->layVideo->addWidget(wgtVideo);
    ui->layVideo->addWidget(wgtDepth);
    lblFrame->setMinimumWidth(25);
    lblFrameCount->setMinimumWidth(25);
    ui->statusBar->addPermanentWidget(new QLabel(tr("Quadro")));
    ui->statusBar->addPermanentWidget(lblFrame);
    ui->statusBar->addPermanentWidget(new QLabel(tr("Total")));
    ui->statusBar->addPermanentWidget(lblFrameCount);
    setState(NOFILE);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setNavigationEnabled(bool enabled)
{
    ui->btnFirst->setEnabled(enabled);
    ui->btnRewind->setEnabled(enabled);
    ui->btnPlay->setEnabled(enabled);
    ui->btnPause->setEnabled(enabled);
    ui->btnFastForward->setEnabled(enabled);
    ui->btnEnd->setEnabled(enabled);
}

void MainWindow::setState(MainWindowStates newState)
{
    switch (newState)
    {
        case NOFILE:
            setNavigationEnabled(false);
            break;
        case STOP:
            setNavigationEnabled(true);
            ui->btnPause->setChecked(true);
            ui->btnPlay->setChecked(false);
            break;
        case PLAY:
            setNavigationEnabled(true);
            ui->btnPause->setChecked(false);
            ui->btnPlay->setChecked(true);
    }
    state = newState;
}

void MainWindow::display()
{
    video->update();
    lblFrame->setText(QString::number(video->currentFrame()));
    cv::Mat depthHist = video->calcDepthHist();
    cv::Mat depth = xncv::cvtDepthTo8UHist(video->captureDepth(), depthHist);
    if (!skeleton.isNull())
    {
        auto users = skeleton->getUsers(video->currentFrame());
        for (unsigned i = 0; i < users.size(); ++i)
            xncv::drawLimbs(depth, users[i].getLimbs());
    }

    wgtVideo->sendImage(video->captureBGR());
    wgtDepth->sendImage(depth);
}

void MainWindow::on_btnOpen_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Abrir video"), ".",
        tr("Arquivos de vídeo (*.oni)"));

    if (fileName.isNull())
        return;

    loadVideo(fileName, "");
}

void MainWindow::loadVideo(QString oniFile, QString sklFile)
{
    try
    {
        if (!QFile(oniFile).exists())
            return;

        video.reset(new xncv::VideoSource(oniFile.toStdString()));
        setState(STOP);

        int index = oniFile.replace("\\\\", "/").lastIndexOf("/");
        if (index != -1)
            oniFile = oniFile.mid(index+1);
        this->setWindowTitle(oniFile);
        lblFrameCount->setText(QString::number(video->size()));
        on_btnFirst_clicked();

        if (sklFile.isEmpty())
            sklFile = oniFile.replace(".oni", ".skl");
        if (!QFile(sklFile).exists())
            skeleton.reset();
        else
        {
            skeleton.reset(new xncv::SkeletonReader());
            skeleton->open(oniFile.toStdString());
        }

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

void MainWindow::on_btnPlay_clicked()
{
    setState(PLAY);
    while (state == PLAY)
    {
        display();
        QApplication::processEvents();
    }

    on_btnPause_clicked();
}

void MainWindow::on_btnPause_clicked()
{
    if (state == NOFILE)
        return;

    setState(STOP);
    video->stop();
}

void MainWindow::on_btnFirst_clicked()
{
    video->first();
    display();
}

void MainWindow::on_btnEnd_clicked()
{
    video->last();
    display();
}

void MainWindow::on_btnRewind_clicked()
{
    video->jump(-5);
    display();
}

void MainWindow::on_btnFastForward_clicked()
{
    video->jump(5);
    display();
}

void MainWindow::closeEvent(QCloseEvent * evt)
{
    state = STOP;
    if (video != nullptr) video->stop();
    evt->accept();
}

void MainWindow::on_btnRecord_clicked()
{
    on_btnPause_clicked();
    RecordWizard recwiz;
    if (recwiz.exec() != QDialog::Accepted)
        return;
    Delay::sleep(500);
    loadVideo(recwiz.getOniFile(), recwiz.getSklFile());
}

void MainWindow::on_btnAbout_clicked()
{
    About about;
    about.setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
    about.exec();
}
