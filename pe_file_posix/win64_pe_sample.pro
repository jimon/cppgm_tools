
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += debug

SOURCES += win64_pe_sample.cpp

QMAKE_CXXFLAGS += -std=c++11
