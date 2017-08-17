#-------------------------------------------------
#
# Project created by QtCreator 2017-08-11T09:35:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencv_hist
TEMPLATE = app


SOURCES += main.cpp\
        histwidget.cpp \
    displayimgdialog.cpp

HEADERS  += histwidget.h \
    displayimgdialog.h

FORMS    += histwidget.ui \
    displayimgdialog.ui

INCLUDEPATH+=D:/lib/lib_src/opencv/build/include

LIBS+=D:\lib\lib_src\opencv\build\x86\vc11\lib\opencv_core2413d.lib
LIBS+=D:\lib\lib_src\opencv\build\x86\vc11\lib\opencv_imgproc2413d.lib
LIBS+=D:\lib\lib_src\opencv\build\x86\vc11\lib\opencv_highgui2413d.lib

