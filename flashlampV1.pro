#-------------------------------------------------
#
# Project created by QtCreator 2015-12-08T14:54:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flashlampV1
TEMPLATE = app


SOURCES += main.cpp\
        flashlamp.cpp \
    encoderthread.cpp \
    microservothread.cpp

HEADERS  += flashlamp.h \
    encoderthread.h \
    microservothread.h

FORMS    += flashlamp.ui

unix:!macx: LIBS += -lwiringPi
