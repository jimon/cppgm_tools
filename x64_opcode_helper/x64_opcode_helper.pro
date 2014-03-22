#-------------------------------------------------
#
# Project created by QtCreator 2014-03-22T17:09:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = x64_opcode_helper
TEMPLATE = app


SOURCES += main.cpp\
		mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


DEFINES += BEA_ENGINE_STATIC
INCLUDEPATH += beaengine beaengine/include beaengine/beaengineSources
SOURCES += beaengine/beaengineSources/BeaEngine.c
HEADERS += beaengine/include/beaengine/BeaEngine.h
