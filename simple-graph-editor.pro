#-------------------------------------------------
#
# Project created by QtCreator 2012-10-14T13:18:59
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simple-graph-editor
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    node.cpp \
    edge.cpp \
    graphwidget.cpp \
    dialog.cpp \
    costdialog.cpp

HEADERS  += mainwindow.h \
    node.h \
    edge.h \
    graphwidget.h \
    dialog.h \
    costdialog.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++0x \
