QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += main.cpp \
           mainwindow.cpp \
           ghost.cpp \
           pacman.cpp \
           networkmanager.cpp \
           startwindow.cpp

HEADERS += mainwindow.h \
           ghost.h \
           pacman.h \
           networkmanager.h \
           startwindow.h

FORMS += mainwindow.ui \
         startwindow.ui

RESOURCES += Resources.qrc
