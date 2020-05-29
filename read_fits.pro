TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS = -L/usr/local/lib/ -lcfitsio

SOURCES += \
        src/fits_ui.cpp \
        src/main.cpp

HEADERS += \
        src/fits_ui.h

DISTFILES +=
