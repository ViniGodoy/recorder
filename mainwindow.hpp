#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "cvwidget.hpp"
#include <XnCv/xncv.hpp>

namespace Ui {
    class MainWindow;
}

enum MainWindowStates {NOFILE, PLAY, STOP, REC};

class MainWindow : public QMainWindow
{
        Q_OBJECT
    protected:
        void closeEvent(QCloseEvent *);

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        void setState(MainWindowStates newState);

    private slots:
        void on_btnPlay_clicked();

        void on_btnOpen_clicked();

        void on_btnPause_clicked();

        void on_btnFirst_clicked();

        void on_btnEnd_clicked();

        void on_btnRewind_clicked();

        void on_btnFastForward_clicked();

        void on_btnRecord_clicked();

        void on_btnAbout_clicked();

    private:
        Ui::MainWindow *ui;
        CVWidget* wgtVideo;
        CVWidget* wgtDepth;
        QScopedPointer<xncv::VideoSource> video;
        QScopedPointer<xncv::SkeletonReader> skeleton;

        MainWindowStates state;
        QLabel* lblFrame;
        QLabel* lblFrameCount;


        void setNavigationEnabled(bool enabled);
        void display();
        void loadVideo(QString oniFile, QString sklFile);
};

#endif // MAINWINDOW_HPP
