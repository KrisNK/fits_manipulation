TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEPENDPATH = dep/lib/
INCLUDEPATH = dep/header/

LIBS = -lcfitsio

SOURCES += \
        src/fits_manipulation.cpp \
        src/main.cpp

HEADERS += \
        src/fits_manipulation.h

DISTFILES +=
