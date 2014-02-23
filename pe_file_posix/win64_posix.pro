TEMPLATE = lib
CONFIG += dll
TARGET = win64_posix
CONFIG -= qt
DEFINES += LIBDLL_EXPORTS

SOURCES += win64_posix.c
HEADERS += win64_posix.h

SOURCES += mman.c
HEADERS += mman.h
