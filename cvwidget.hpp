#ifndef CVWIDGET_HPP
#define CVWIDGET_HPP

#include <QGLWidget>
#include <QtOpenGL>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

class CVWidget : public QGLWidget
{
    Q_OBJECT
public:
    CVWidget();
    void sendImage(const cv::Mat &img);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
private:
    QImage qframe;
};

#endif // CVWIDGET_HPP
