TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS = -L/usr/local/lib/ -lcfitsio

SOURCES += \
        src/file_management.cpp \
        src/fits_ui.cpp \
        src/main.cpp

HEADERS += \
        src/file_management.h \
        src/fits_ui.h

DISTFILES +=
