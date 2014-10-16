QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simple-graph-editor
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    node.cpp \
    edge.cpp \
    graphwidget.cpp

HEADERS  += mainwindow.h \
    node.h \
    edge.h \
    graphwidget.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11
