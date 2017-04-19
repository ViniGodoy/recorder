#-------------------------------------------------
#
# Project created by QtCreator 2012-06-07T18:57:13
#
#-------------------------------------------------

RC_FILE = icon.rc

QT       += core gui
QT       += opengl
QT       += xml

TARGET = Recorder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cvwidget.cpp \
    recordwizard.cpp \
    actorsetuppage.cpp \
    actor.cpp \
    word.cpp \
    about.cpp

HEADERS  += mainwindow.hpp \
    cvwidget.hpp \
    recordwizard.hpp \
    actorsetuppage.hpp \
    actor.hpp \
    word.hpp \
    qwizardcombobox.hpp \
    about.hpp

FORMS    += mainwindow.ui \
    recordwizard.ui \
    actorsetuppage.ui \
    about.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../XnCv/dist/lib -lXnCv
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../XnCv/dist/lib -lXnCv

INCLUDEPATH += $$PWD/../../XnCv/src
DEPENDPATH += $$PWD/../../XnCv/src

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../XnCv/dist/lib/xncv.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../XnCv/dist/lib/xncvd.lib

INCLUDEPATH += C:\Projetos\libs\opencv\build\include
LIBS += -LC:\Projetos\libs\opencv\build\x86\vc10\lib\
-lopencv_core241 \
-lopencv_highgui241 \
-lopencv_imgproc241 \
-lopencv_features2d241 \
-lopencv_calib3d241

INCLUDEPATH += C:\Projetos\libs\OpenNI\Include
LIBS += -LC:\Projetos\Libs\openni\lib\
-lopenni

RESOURCES += \
    icons.qrc

OTHER_FILES += \
    icon.rc \
    recorder.ico \
    recorder.xml
