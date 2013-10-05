#-------------------------------------------------
#
# Project created by QtCreator 2013-09-02T12:10:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConsoleDebugger
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp

CONFIG += static

HEADERS  += mainwindow.h \
    targetver.h \
    server.h
    
LIBS += -lws2_32

FORMS    += mainwindow.ui

OTHER_FILES += \
    b5s.jpg \
    b4s.jpg \
    b3s.jpg \
    b2s.jpg \
    B1s.jpg \
    Run.bat

RESOURCES += \
    images.qrc
