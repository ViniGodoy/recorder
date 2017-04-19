#include "cvwidget.hpp"

using namespace cv;

CVWidget::CVWidget() : QGLWidget(QGLFormat(QGL::SampleBuffers)) {
    setMinimumSize(320,240);
}

void CVWidget::initializeGL() {
    glClearColor(0.0f,0.0f,0.0f,1.0f);
}
void CVWidget::paintGL() {
    glClear (GL_COLOR_BUFFER_BIT);
    glClearColor (0.0,0.0,0.0,1.0);
    if (!qframe.isNull()) {
        qframe = qframe.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        glDrawPixels(qframe.width(),qframe.height(), GL_RGBA, GL_UNSIGNED_BYTE, qframe.bits());
        glFlush();
    }
}

void CVWidget::resizeGL(int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(0,w,0,h,-1,1);
    glMatrixMode (GL_MODELVIEW);
}

void CVWidget::sendImage(const Mat& img) {
    if (img.channels() == 3)
        qframe = QImage((const unsigned char*)(img.data), img.cols, img.rows, img.step, QImage::Format_RGB888).rgbSwapped();
    else
        qframe = QImage((const unsigned char*)(img.data), img.cols, img.rows, img.step, QImage::Format_Indexed8);
    qframe = QGLWidget::convertToGLFormat(qframe);
    this->updateGL();
}
