#ifndef RECORDWIZARD_HPP
#define RECORDWIZARD_HPP

#include <QWizard>
#include <xncv/xncv.hpp>
#include "cvwidget.hpp"

namespace Ui {
class RecordWizard;
}

enum {PAGE_ACTOR=1, PAGE_SKELETON, PAGE_FRAMING, PAGE_RECORDING};

class RecordWizard : public QWizard
{
        Q_OBJECT
        
    public:
        explicit RecordWizard(QWidget *parent = 0);
        ~RecordWizard();
        const QString& getOniFile() const;
        const QString& getSklFile() const;

    private slots:
        void updateVideo();
        void on_btnRecord_clicked();

    private:
        Ui::RecordWizard *ui;
        QScopedPointer<xncv::VideoSource> video;
        QScopedPointer<xncv::UserTracker> tracker;
        QScopedPointer<xncv::SkeletonWriter> skeletonWriter;

        QScopedPointer<QTimer> tmrVideo;

        CVWidget* wgtDepthSkeleton;
        CVWidget* wgtDepthFraming;
        CVWidget* wgtDepthRecording;
        void initializeSkeletonPage();

        QString oniFile;
        QString sklFile;

    protected:
        void initializePage(int id);        

};

#endif // RECORDWIZARD_HPP
