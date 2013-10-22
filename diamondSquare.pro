TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(defaults.pri)

HEADERS += \
    src/diamondSquare.h

SOURCES += \
    src/main.cpp \
    src/diamondSquare.cpp
